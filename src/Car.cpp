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

    Vector2D direction =
        (endPosition - startPosition).normalized();

    return startPosition + direction * roadPosition_;
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

void Car::setCurrentRoad(Road* road) {
    currentRoad_ = road;

    if (currentRoad_) {
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

    Crossroad* destination = currentRoad_->getEnd();

    if (destination == nullptr)
        return;

    const auto& roads = destination->getRoads();

    for (Road* road : roads)
    {
        if (road == currentRoad_)
            continue;

        currentRoad_ = road;
        roadPosition_ = 0.0;

        return;
    }
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

    if (currentRoad_)
    {
        speed_ = std::min(speed_, currentRoad_->getSpeedLimit());

        roadPosition_ += speed_ * deltaTime;

        if (roadPosition_ >= currentRoad_->getLength())
        {
            moveToNextRoad();
        }
    }
}
