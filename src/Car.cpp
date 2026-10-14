#include "Car.h"
#include "Road.h"
#include "Crossroad.h"
#include "Route.h"
#include <cmath>
#include <utility>
#include <algorithm>
#include <iostream>

Car::Car(int id, Vector2D position, Direction direction)
    : id_(id),
      position_(position),
      direction_(direction) {}

int Car::getId() const {
    return id_;
}

const Vector2D& Car::getPosition() const {
    return position_;
}

void Car::setPosition(Vector2D position) {
    position_ = position;
}

Vector2D Car::getWorldPosition() const
{
    if (!currentRoad_)
        return position_;

    Crossroad* start = currentRoad_->getStart();
    Crossroad* end = currentRoad_->getEnd();

    if (!start || !end)
        return position_;

    Vector2D roadDirection =
        end->getPosition() - start->getPosition();

    const double length = roadDirection.length();

    if (length <= 0.0)
        return start->getPosition();

    roadDirection = roadDirection * (1.0 / length);

    const bool forward = isTravellingForward();

    const Vector2D origin =
        forward ? start->getPosition() : end->getPosition();

    const Vector2D travelDirection =
        forward ? roadDirection : roadDirection * -1.0;

    Vector2D worldPosition =
        origin + travelDirection * roadPosition_;

    // A 20-pixel road has two 10-pixel lanes.
    // Place each car in the center of its lane.
    const Vector2D right(
        -travelDirection.y,
         travelDirection.x
    );

    constexpr double laneWidth = 10.0;

    const double laneOffset =
        (static_cast<double>(laneIndex_) - 0.5) * laneWidth;

    worldPosition = worldPosition + right * laneOffset;

    return worldPosition;
}

Direction Car::getDirection() const
{
    if (!currentRoad_)
        return Direction::East;

    Crossroad* start = currentRoad_->getStart();
    Crossroad* end = currentRoad_->getEnd();

    if (!start || !end)
        return Direction::East;

    Vector2D delta = end->getPosition() - start->getPosition();

    if (!isTravellingForward())
        delta = delta * -1.0;

    if (std::abs(delta.x) > std::abs(delta.y))
        return delta.x >= 0.0 ? Direction::East : Direction::West;

    return delta.y >= 0.0 ? Direction::South : Direction::North;
}

std::shared_ptr<Car> Car::getCarAhead() const
{
    if (!currentRoad_)
        return nullptr;

    constexpr double detectionDistance = 150.0;

    std::shared_ptr<Car> closestCar;
    double closestDistance = detectionDistance;

    for (const auto& other : currentRoad_->getCars())
    {
        if (!other || other.get() == this)
            continue;

        if (other->getLaneIndex() != laneIndex_)
            continue;

        const double distance =
            other->getRoadPosition() - roadPosition_;

        if (distance <= 0.0)
            continue;

        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestCar = other;
        }
    }

    return closestCar;
}

double Car::getSpeed() const {
    return speed_;
}

double Car::getMaxSpeed() const {
    return maxSpeed_;
}

double Car::getAcceleration() const {
    return acceleration_;
}

double Car::getBrakeDeceleration() const {
    return brakeDeceleration_;
}

void Car::setMaxSpeed(double speed) {
    maxSpeed_ = std::max(0.0, speed);
    speed_ = std::min(speed_, maxSpeed_);
}

void Car::setAcceleration(double acceleration) {
    acceleration_ = std::max(0.0, acceleration);
}

void Car::setBrakeDeceleration(double deceleration) {
    brakeDeceleration_ = std::max(0.0, deceleration);
}

void Car::setDirection(Direction direction) {
    direction_ = direction;
}

double Car::getRoadPosition() const {
    return roadPosition_;
}

void Car::setRoadPosition(double position) {
    if (currentRoad_) {
        roadPosition_ = std::clamp(
            position,
            0.0,
            currentRoad_->getLength());
    } else {
        roadPosition_ = std::max(0.0, position);
    }
}

int Car::getLaneIndex() const {
    return laneIndex_;
}

void Car::setLaneIndex(int lane) {
    if (currentRoad_) {
        laneIndex_ = std::clamp(
            lane,
            0,
            static_cast<int>(currentRoad_->getLaneCount()) - 1);
    } else {
        laneIndex_ = std::max(0, lane);
    }
}

bool Car::isStopped() const {
    return stopped_;
}

void Car::stop() {
    speed_ = 0.0;
    stopped_ = true;
}

void Car::start() {
    stopped_ = false;
}

void Car::setCurrentRoad(Road* road)
{
    currentRoad_ = road;

    if (currentRoad_)
    {
        setRoadPosition(roadPosition_);
        setLaneIndex(laneIndex_);
    }
}

Road* Car::getCurrentRoad() const {
    return currentRoad_;
}

bool Car::isTravellingForward() const
{
    if (!currentRoad_)
        return true;

    const int lane = getLaneIndex();

    if (lane < 0 ||
        lane >= static_cast<int>(currentRoad_->getLaneCount()))
    {
        return true;
    }

    const Lane* laneInfo =
        currentRoad_->getLane(static_cast<std::size_t>(lane));

    return laneInfo ? laneInfo->forward : true;
}

void Car::moveToNextRoad()
{
    if (!currentRoad_)
        return;

    Road* previousRoad = currentRoad_;
    const bool forward = isTravellingForward();

    Crossroad* reachedCrossroad =
        forward ? previousRoad->getEnd()
                : previousRoad->getStart();

    if (!reachedCrossroad)
    {
        speed_ = 0.0;
        stopped_ = true;
        return;
    }

    std::cout << "Car " << id_
              << " reached crossroad " << reachedCrossroad->getId()
              << " from road " << previousRoad->getId() << '\n';

    // The route determines which road the car should take next.
    Road* nextRoad = route_ ? route_->getNextRoad() : nullptr;

    // Verify that the requested road actually connects to this crossroad.
    if (nextRoad &&
        (nextRoad == previousRoad ||
         (nextRoad->getStart() != reachedCrossroad &&
          nextRoad->getEnd() != reachedCrossroad)))
    {
        std::cout << "Invalid next road in route for car "
                  << id_ << '\n';

        nextRoad = nullptr;
    }

    // Don't arbitrarily choose a different road if the route is invalid.
    if (!nextRoad)
    {
        std::cout << "Car " << id_
                  << " has no valid next road at crossroad "
                  << reachedCrossroad->getId() << '\n';

        speed_ = 0.0;
        stopped_ = true;
        return;
    }

    // Remove the car from its previous road.
    previousRoad->removeCar(id_);

    // Enter the new road from the endpoint connected to the crossroad.
    currentRoad_ = nextRoad;

    if (nextRoad->getStart() == reachedCrossroad)
    {
        laneIndex_ = 0; // Start -> end
    }
    else
    {
        laneIndex_ = 1; // End -> start
    }

    roadPosition_ = 0.0;
    position_ = reachedCrossroad->getPosition();

    nextRoad->addCar(shared_from_this());

    route_->advance();

    stopped_ = false;

    const Vector2D worldPosition = getWorldPosition();

    std::cout << "Car " << id_
              << " entered road " << nextRoad->getId()
              << " from crossroad " << reachedCrossroad->getId()
              << ", lane " << laneIndex_
              << ", world position ("
              << worldPosition.x << ", "
              << worldPosition.y << ")\n";
}

void Car::setRoute(const std::shared_ptr<Route>& route)
{
    route_ = route;
}

Route* Car::getRoute() const {
    return route_.get();
}

void Car::accelerate(double deltaTime) {
    if (stopped_ || deltaTime <= 0.0) {
        return;
    }

    speed_ += acceleration_ * deltaTime;
    speed_ = std::min(speed_, maxSpeed_);
}

void Car::brake(double deltaTime) {
    if (deltaTime <= 0.0) {
        return;
    }

    speed_ -= brakeDeceleration_ * deltaTime;

    if (speed_ <= 0.0) {
        speed_ = 0.0;
        stopped_ = true;
    }
}

void Car::update(double deltaTime)
{
    if (stopped_ || deltaTime <= 0.0)
        return;

    if (currentRoad_ == nullptr)
        return;

    // ---------------------------------------------------------
    // Calculate the effective speed limit for this road.
    // The car cannot exceed its own maximum speed or the road
    // speed limit.
    // ---------------------------------------------------------

    double roadSpeedLimit = currentRoad_->getSpeedLimit();

    double effectiveMaxSpeed =
        std::min(maxSpeed_, roadSpeedLimit);

    // ---------------------------------------------------------
    // Start overtaking.
    // ---------------------------------------------------------

    auto carAhead = getCarAhead();

    if (carAhead && canOvertake())
    {
        overtake();

    }

    // ---------------------------------------------------------
    // Check whether we have passed the overtaken car.
    // ---------------------------------------------------------

    // If we are already overtaking, finish the manoeuvre.
    if (overtaking_)
    {
        if (canReturnToRight())
            returnToRightLane();
    }
    else
    {
        auto carAhead = getCarAhead();

        if (carAhead && canOvertake())
            overtake();
    }

    // ---------------------------------------------------------
    // Get the car currently ahead in our lane.
    // ---------------------------------------------------------

    carAhead = getCarAhead();

    // Start with the effective speed limit.
    double targetSpeed = effectiveMaxSpeed;

    // ---------------------------------------------------------
    // Follow the car ahead.
    // ---------------------------------------------------------

    if (carAhead)
    {
        double distance =
            carAhead->getRoadPosition() - roadPosition_;

        constexpr double detectionDistance = 150.0;
        constexpr double desiredDistance = 25.0;

        if (distance < detectionDistance)
        {
            if (distance <= desiredDistance)
            {
                targetSpeed = 0.0;
            }
            else
            {
                double gapFactor =
                    (distance - desiredDistance) /
                    (detectionDistance - desiredDistance);

                gapFactor = std::clamp(
                    gapFactor,
                    0.0,
                    1.0
                );

                double followSpeed =
                    carAhead->getSpeed() * gapFactor;

                targetSpeed = std::min(
                    targetSpeed,
                    followSpeed
                );
            }
        }
    }

    // ---------------------------------------------------------
    // Ensure target speed never exceeds the effective limit.
    // ---------------------------------------------------------

    targetSpeed = std::clamp(
        targetSpeed,
        0.0,
        effectiveMaxSpeed
    );

    // ---------------------------------------------------------
    // Accelerate / brake.
    // ---------------------------------------------------------

    if (speed_ < targetSpeed)
    {
        speed_ += acceleration_ * deltaTime;

        if (speed_ > targetSpeed)
            speed_ = targetSpeed;
    }
    else if (speed_ > targetSpeed)
    {
        speed_ -= brakeDeceleration_ * deltaTime;

        if (speed_ < targetSpeed)
            speed_ = targetSpeed;
    }

    // ---------------------------------------------------------
    // Final safety check: prevent exceeding the speed limit.
    // ---------------------------------------------------------

    if (speed_ > effectiveMaxSpeed)
        speed_ = effectiveMaxSpeed;

    if (speed_ < 0.0)
        speed_ = 0.0;

    // ---------------------------------------------------------
    // Move the car.
    // ---------------------------------------------------------

    roadPosition_ += speed_ * deltaTime;

    // ---------------------------------------------------------
    // Move to the next road if we reach the end.
    // ---------------------------------------------------------

    if (roadPosition_ >= currentRoad_->getLength())
    {
        roadPosition_ = currentRoad_->getLength();
        moveToNextRoad();
    }
}

bool Car::isLaneClear(int lane) const
{
    if (currentRoad_ == nullptr)
        return false;

    constexpr double frontClearDistance = 30.0;
    constexpr double rearClearDistance = 30.0;

    for (const auto& other : currentRoad_->getCars())
    {
        if (!other)
            continue;

        if (other.get() == this)
            continue;

        if (other->getLaneIndex() != lane)
            continue;

        double distance =
            other->getRoadPosition() - roadPosition_;

        // Car ahead in target lane.
        if (distance >= 0.0 &&
            distance < frontClearDistance)
        {
            return false;
        }

        // Car behind in target lane.
        if (distance < 0.0 &&
            std::abs(distance) < rearClearDistance)
        {
            return false;
        }
    }

    return true;
}

bool Car::canOvertake() const
{
    if (!currentRoad_ || stopped_ || overtaking_)
        return false;

    // We need two lanes for overtaking.
    if (currentRoad_->getLaneCount() < 2)
        return false;

    const int currentLane = getLaneIndex();
    const int oppositeLane = 1 - currentLane;

    const Lane* currentLaneInfo =
        currentRoad_->getLane(static_cast<std::size_t>(currentLane));

    const Lane* oppositeLaneInfo =
        currentRoad_->getLane(static_cast<std::size_t>(oppositeLane));

    if (!currentLaneInfo || !oppositeLaneInfo)
        return false;

    // Overtaking requires an actual lane travelling in the
    // opposite direction.
    if (currentLaneInfo->forward == oppositeLaneInfo->forward)
        return false;

    // Find the slower car ahead in our current lane.
    auto carAhead = getCarAhead();

    if (!carAhead)
        return false;

    if (carAhead->getCurrentRoad() != currentRoad_)
        return false;

    // There must be a speed advantage.
    const double mySpeed = getSpeed();
    const double targetSpeed = carAhead->getSpeed();

    if (getMaxSpeed() <= targetSpeed + 1.0)
        return false;

    constexpr double carLength = 15.0;
    constexpr double safetyGap = 15.0;
    constexpr double minSpeed = 1.0;

    // Estimate the distance needed to pass the slower car.
    const double distanceToTarget =
        carAhead->getRoadPosition() - getRoadPosition();

    if (distanceToTarget <= 0.0)
        return false;

    const double passingDistance =
        distanceToTarget + carLength + 2.0 * safetyGap;

    // Estimate the speed we can achieve during the manoeuvre.
    const double roadSpeedLimit = currentRoad_->getSpeedLimit();

    const double passingSpeed = std::min(
        getMaxSpeed(),
        std::max(mySpeed, targetSpeed + 5.0)
    );

    const double effectiveSpeed = std::min(
        passingSpeed,
        roadSpeedLimit
    );

    if (effectiveSpeed <= minSpeed)
        return false;

    // Conservative estimate: how long will the manoeuvre take?
    const double manoeuvreTime =
        passingDistance / effectiveSpeed;

    // Convert the car's entry-relative position into a common
    // coordinate measured from the road's start.
    const double roadLength = currentRoad_->getLength();

    const double myRoadCoordinate =
        isTravellingForward()
            ? getRoadPosition()
            : roadLength - getRoadPosition();

    // Required space ahead of us for the whole manoeuvre.
    constexpr double extraDistance = 30.0;

    const double requiredDistance =
        passingDistance + extraDistance;

    // Check all cars travelling in the opposite direction.
    for (const auto& other : currentRoad_->getCars())
    {
        if (!other || other.get() == this)
            continue;

        if (other->getLaneIndex() != oppositeLane)
            continue;

        // Ignore cars travelling in the same direction as us.
        if (other->isTravellingForward() == isTravellingForward())
            continue;

        const double otherCoordinate =
            other->isTravellingForward()
                ? other->getRoadPosition()
                : roadLength - other->getRoadPosition();

        // Distance from us to the oncoming car, measured in
        // our direction of travel.
        const double distanceToOncoming =
            (otherCoordinate - myRoadCoordinate) *
            (isTravellingForward() ? 1.0 : -1.0);

        // This car is behind us, or is not relevant to the
        // upcoming manoeuvre.
        if (distanceToOncoming <= 0.0)
            continue;

        // Account for both cars moving towards each other.
        const double closingSpeed =
            effectiveSpeed + other->getSpeed();

        const double distanceNeeded =
            closingSpeed * manoeuvreTime +
            extraDistance + carLength;

        // Not enough room to finish overtaking before the
        // oncoming car reaches the conflict area.
        if (distanceToOncoming < std::max(
                requiredDistance, distanceNeeded))
        {
            return false;
        }
    }

    // Check that there is enough room to return to our lane
    // before reaching the end of the road.
    const double remainingRoad =
        roadLength - getRoadPosition();

    if (remainingRoad < requiredDistance)
        return false;

    return true;
}

void Car::overtake()
{
    if (!canOvertake())
        return;

    const int oppositeLane = 1 - getLaneIndex();

    // Store the car we intend to pass.
    auto target = getCarAhead();

    if (!target)
        return;

    setLaneIndex(oppositeLane);

    overtaking_ = true;
    overtakenCar_ = target;
}

bool Car::canReturnToRight() const
{
    if (!currentRoad_ || !overtaking_)
        return false;

    auto target = overtakenCar_.lock();

    if (!target)
        return true;

    if (target->getCurrentRoad() != currentRoad_)
        return true;

    // The target must be behind us in the same travel direction.
    const double gap =
        getRoadPosition() - target->getRoadPosition();

    constexpr double carLength = 15.0;
    constexpr double safetyGap = 15.0;

    if (gap < carLength + safetyGap)
        return false;

    // Check for another car in the lane we want to re-enter.
    const int originalLane =
        isTravellingForward() ? 0 : 1;

    for (const auto& other : currentRoad_->getCars())
    {
        if (!other || other.get() == this ||
            other.get() == target.get())
        {
            continue;
        }

        if (other->getLaneIndex() != originalLane)
            continue;

        const double distance =
            other->getRoadPosition() - getRoadPosition();

        if (std::abs(distance) < carLength + safetyGap)
            return false;
    }

    return true;
}

void Car::returnToRightLane()
{
    if (!canReturnToRight())
        return;

    const int originalLane =
        isTravellingForward() ? 0 : 1;

    setLaneIndex(originalLane);

    overtaking_ = false;
    overtakenCar_.reset();
}