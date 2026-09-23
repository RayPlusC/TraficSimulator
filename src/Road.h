#pragma once

#include "RoadSign.h"
#include "TrafficTypes.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Car;
class Crossroad;

struct Lane {
    int id;
    double length;
    bool forward{true};
    std::vector<std::weak_ptr<Car>> cars;
    
    Lane(int laneId, double laneLength, bool isForward = true)
        : id(laneId), length(laneLength), forward(isForward) {}
};

class Road {
public:
    Road(
        int id,
        std::string name,
        Crossroad* start,
        Crossroad* end,
        double speedLimit,
        int laneCount = 1
    );

    int getId() const;
    const std::string& getName() const;

    Crossroad* getStart() const;
    Crossroad* getEnd() const;

    double getLength() const;
    double getSpeedLimit() const;

    void setSpeedLimit(double speedLimit);

    std::size_t getLaneCount() const;
    Lane* getLane(std::size_t index);
    const Lane* getLane(std::size_t index) const;

    void addSign(std::shared_ptr<RoadSign> sign);
    const std::vector<std::shared_ptr<RoadSign>>& getSigns() const;

    void addCar(const std::shared_ptr<Car>& car);
    void removeCar(int carId);
    const std::vector<std::shared_ptr<Car>>& getCars() const;

private:
    int id_;
    std::string name_;

    Crossroad* start_;
    Crossroad* end_;

    double length_;
    double speedLimit_;

    std::vector<Lane> lanes_;
    std::vector<std::shared_ptr<RoadSign>> signs_;
    std::vector<std::shared_ptr<Car>> cars_;
};
