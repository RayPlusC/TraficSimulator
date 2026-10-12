#include "Car.h"
#include "Road.h"
#include "Crossroad.h"

#include <algorithm>

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
    if (currentRoad_ == nullptr)
        return position_;

    Crossroad* start = currentRoad_->getStart();
    Crossroad* end = currentRoad_->getEnd();

    if (start == nullptr || end == nullptr)
        return position_;

    Vector2D startPosition = start->getPosition();
    Vector2D endPosition = end->getPosition();

    Vector2D roadDirection =
        (endPosition - startPosition).normalized();

    Vector2D right(
        -roadDirection.y,
         roadDirection.x
    );

    // Position along the road.
    Vector2D worldPosition =
        startPosition +
        roadDirection * roadPosition_;

    // ---------------------------------------------------------
    // Lane positioning
    //
    // lane 0 = right lane
    // lane 1 = left lane
    //
    // For a two-lane road:
    //
    //        lane 1
    //          |
    // road ----+---->
    //          |
    //        lane 0
    //
    // ---------------------------------------------------------

    constexpr double laneWidth = 10.0;

    const std::size_t laneCount =
        currentRoad_->getLaneCount();

    const double roadWidth =
        static_cast<double>(laneCount) * laneWidth;

    const double laneCenter =
        (static_cast<double>(laneIndex_) + 0.5) * laneWidth;

    // lane 0 is on the right side of the road.
    const double offsetFromCenter =
        roadWidth / 2.0 - laneCenter;

    worldPosition +=
        right * offsetFromCenter;

    return worldPosition;
}

Direction Car::getDirection() const
{
    if (currentRoad_ == nullptr)
        return direction_;

    Vector2D start =
        currentRoad_->getStart()->getPosition();

    Vector2D end =
        currentRoad_->getEnd()->getPosition();

    Vector2D delta = end - start;

    if (std::abs(delta.x) > std::abs(delta.y))
    {
        return delta.x > 0
            ? Direction::East
            : Direction::West;
    }

    return delta.y > 0
        ? Direction::South
        : Direction::North;
}

std::shared_ptr<Car> Car::getCarAhead() const
{
    if (currentRoad_ == nullptr)
        return nullptr;

    constexpr double detectionDistance = 150.0;

    std::shared_ptr<Car> closestCar = nullptr;
    double closestDistance = detectionDistance;

    for (const auto& other : currentRoad_->getCars())
    {
        if (!other)
            continue;

        // Ignore myself
        if (other.get() == this)
            continue;

        // Ignore cars in another lane
        if (other->getLaneIndex() != laneIndex_)
            continue;

        // Distance along this road
        double distance =
            other->getRoadPosition() - roadPosition_;

        // Car is behind us
        if (distance <= 0.0)
            continue;

        // Find closest car ahead
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

void Car::moveToNextRoad()
{
    if (currentRoad_ == nullptr)
        return;

    Crossroad* destination =
        currentRoad_->getEnd();

    if (destination == nullptr)
        return;

    Road* oldRoad = currentRoad_;
    Road* nextRoad = nullptr;

    // Find the next road connected to the destination.
    for (Road* road : destination->getRoads())
    {
        if (road == nullptr)
            continue;

        if (road == oldRoad)
            continue;

        nextRoad = road;
        break;
    }

    // No next road.
    if (nextRoad == nullptr)
        return;

    // Remove the car from the road it just left.
    oldRoad->removeCar(id_);

    // Reset position on the new road.
    roadPosition_ = 0.0;

    // Keep the lane index valid for the new road.
    if (nextRoad->getLaneCount() > 0)
    {
        laneIndex_ = std::clamp(
            laneIndex_,
            0,
            static_cast<int>(nextRoad->getLaneCount()) - 1
        );
    }
    else
    {
        laneIndex_ = 0;
    }

    // Add the car to the new road.
    nextRoad->addCar(shared_from_this());
}

void Car::setRoute(std::shared_ptr<Route> route) {
    route_ = std::move(route);
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

    if (overtaking_)
    {
        auto overtakenCar = overtakenCar_.lock();

        if (!overtakenCar)
        {
            overtaking_ = false;
        }
        else
        {
            // Overtaken car is now behind us.
            if (overtakenCar->getRoadPosition() < roadPosition_)
            {
                // Move one lane toward the right when safe.
                if (canReturnToRight())
                {
                    returnToRightLane();
                }
            }
        }
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
    if (currentRoad_ == nullptr)
        return false;

    const int mostLeftLane =
        static_cast<int>(
            currentRoad_->getLaneCount()
        ) - 1;

    // No multiple lanes.
    if (mostLeftLane <= 0)
        return false;

    // Already at the leftmost lane.
    if (laneIndex_ >= mostLeftLane)
        return false;

    auto carAhead = getCarAhead();

    if (!carAhead)
        return false;

    // Only overtake a slower vehicle.
    if (carAhead->getSpeed() >= speed_)
        return false;

    // There must be enough space in the lane to our left.
    if (!isLaneClear(laneIndex_ + 1))
        return false;

    return true;
}

void Car::overtake()
{
    if (!canOvertake())
        return;

    auto carAhead = getCarAhead();

    if (!carAhead)
        return;

    overtakenCar_ = carAhead;

    laneIndex_++;

    overtaking_ = true;
}

bool Car::canReturnToRight() const
{
    if (!overtaking_)
        return false;

    if (laneIndex_ <= 0)
        return false;

    // We only return if the lane immediately
    // to our right is clear.
    return isLaneClear(laneIndex_ - 1);
}

void Car::returnToRightLane()
{
    if (!canReturnToRight())
        return;

    laneIndex_--;

    if (laneIndex_ == 0)
        overtaking_ = false;
}