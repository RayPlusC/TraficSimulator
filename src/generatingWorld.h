#pragma once

#include <memory>
#include <vector>

#include "Crossroad.h"
#include "Road.h"
#include "Car.h"

class Graphics;

class GenerateWorld
{
public:
    GenerateWorld();

    void draw(Graphics& graphics) const;
    void generateCars(int numberOfCars);
    void drawCars(Graphics& graphics) const;
    void updateCars(double deltaTime);
    Road* chooseNextRoad(Crossroad* crossroad, Road* currentRoad);
    
    const std::vector<std::unique_ptr<Crossroad>>& getCrossroads() const;
    const std::vector<std::unique_ptr<Road>>& getRoads() const;

private:
    std::vector<std::unique_ptr<Crossroad>> crossroads_;
    std::vector<std::unique_ptr<Road>> roads_;
    std::vector<std::shared_ptr<Car>> cars_;

    void createCrossroads();
    void createRoads();

    std::shared_ptr<Route> createRoute(int startingRoadIndex, int laneIndex);
};