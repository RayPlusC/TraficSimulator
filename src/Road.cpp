#include "Road.h"
#include "Car.h"
#include "Crossroad.h"

#include <algorithm>
#include <utility>

Road::Road(
    int id,
    std::string name,
    Crossroad* start,
    Crossroad* end,
    double speedLimit,
    int laneCount
)
    : id_(id),
      name_(std::move(name)),
      start_(start),
      end_(end),
      length_(0.0),
      speedLimit_(std::max(0.0, speedLimit))
{
    if (start_ && end_)
    {
        Vector2D difference =
            end_->getPosition() - start_->getPosition();

        length_ = difference.length();
    }

    laneCount = std::max(1, laneCount);

    for (int i = 0; i < laneCount; ++i)
    {
        lanes_.emplace_back(i, length_);
    }
}

int Road::getId() const {
    return id_;
}

const std::string& Road::getName() const {
    return name_;
}

Crossroad* Road::getStart() const {
    return start_;
}

Crossroad* Road::getEnd() const {
    return end_;
}

double Road::getLength() const {
    return length_;
}

double Road::getSpeedLimit() const {
    return speedLimit_;
}

void Road::setSpeedLimit(double speedLimit) {
    speedLimit_ = std::max(0.0, speedLimit);
}

std::size_t Road::getLaneCount() const {
    return lanes_.size();
}

Lane* Road::getLane(std::size_t index) {
    if (index >= lanes_.size()) {
        return nullptr;
    }
    return &lanes_[index];
}

const Lane* Road::getLane(std::size_t index) const {
    if (index >= lanes_.size()) {
        return nullptr;
    }
    return &lanes_[index];
}

void Road::addSign(std::shared_ptr<RoadSign> sign) {
    if (sign) {
        signs_.push_back(std::move(sign));
    }
}

const std::vector<std::shared_ptr<RoadSign>>& Road::getSigns() const {
    return signs_;
}

void Road::addCar(const std::shared_ptr<Car>& car) {
    if (car) {
        cars_.push_back(car);
    }
}

void Road::removeCar(int carId) {
    cars_.erase(
        std::remove_if(cars_.begin(), cars_.end(),
            [carId](const std::shared_ptr<Car>& car) {
                return !car || car->getId() == carId;
            }),
        cars_.end());
}

const std::vector<std::shared_ptr<Car>>& Road::getCars() const {
    return cars_;
}
