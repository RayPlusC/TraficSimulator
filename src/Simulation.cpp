#include "Simulation.h"

#include "Car.h"
#include "Crossroad.h"
#include "Road.h"
#include "TrafficLight.h"

#include <algorithm>
#include <limits>

void Simulation::addCar(std::shared_ptr<Car> car) {
    if (car) {
        cars_.push_back(std::move(car));
    }
}

void Simulation::addRoad(std::shared_ptr<Road> road) {
    if (road) {
        roads_.push_back(std::move(road));
    }
}

void Simulation::addCrossroad(std::shared_ptr<Crossroad> crossroad) {
    if (crossroad) {
        crossroads_.push_back(std::move(crossroad));
    }
}

const std::vector<std::shared_ptr<Car>>& Simulation::getCars() const {
    return cars_;
}

void Simulation::update(double deltaTime) {
    if (deltaTime <= 0.0) {
        return;
    }

    updateTrafficLights(deltaTime);
    updateCars(deltaTime);
}

void Simulation::updateTrafficLights(double deltaTime) {
    for (const auto& crossroad : crossroads_) {
        for (Road* road : crossroad->getRoads()) {
            if (auto* light = crossroad->getTrafficLight(road)) {
                light->update(deltaTime);
            }
        }
    }
}

void Simulation::applySimpleCarFollowing(Car& car) {
    Road* road = car.getCurrentRoad();
    if (!road) {
        return;
    }

    double closestDistance = std::numeric_limits<double>::max();

    for (const auto& other : road->getCars()) {
        if (!other || other.get() == &car) {
            continue;
        }

        if (other->getLaneIndex() != car.getLaneIndex()) {
            continue;
        }

        const double distance =
            other->getRoadPosition() - car.getRoadPosition();

        if (distance > 0.0 && distance < closestDistance) {
            closestDistance = distance;
        }
    }

    // Keep approximately 5 m of space.
    if (closestDistance < 8.0) {
        car.brake(0.2);
    } else {
        car.start();
        car.accelerate(0.2);
    }
}

void Simulation::updateCars(double deltaTime) {
    for (const auto& car : cars_) {
        if (!car) {
            continue;
        }

        applySimpleCarFollowing(*car);
        car->update(deltaTime);
    }
}
