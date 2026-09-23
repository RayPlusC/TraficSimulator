#pragma once

#include <memory>
#include <vector>

class Car;
class Road;
class Crossroad;

class Simulation {
public:
    void addCar(std::shared_ptr<Car> car);
    void addRoad(std::shared_ptr<Road> road);
    void addCrossroad(std::shared_ptr<Crossroad> crossroad);

    const std::vector<std::shared_ptr<Car>>& getCars() const;

    void update(double deltaTime);

private:
    void updateTrafficLights(double deltaTime);
    void updateCars(double deltaTime);
    void applySimpleCarFollowing(Car& car);

    std::vector<std::shared_ptr<Car>> cars_;
    std::vector<std::shared_ptr<Road>> roads_;
    std::vector<std::shared_ptr<Crossroad>> crossroads_;
};
