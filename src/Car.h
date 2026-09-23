#pragma once

#include "Vector2D.h"
#include "TrafficTypes.h"

#include <memory>

class Road;
class Route;

class Car
{
public:
    Car(
        int id,
        Vector2D position,
        Direction direction
    );

    // Basic information
    int getId() const;

    // Position
    const Vector2D& getPosition() const;
    void setPosition(Vector2D position);

    // World position calculated from road position
    Vector2D getWorldPosition() const;

    // Road
    void setCurrentRoad(Road* road);
    Road* getCurrentRoad() const;
    void moveToNextRoad();
    
    // Lane
    void setLaneIndex(int lane);
    int getLaneIndex() const;

    // Position on road
    void setRoadPosition(double position);
    double getRoadPosition() const;

    // Speed
    double getSpeed() const;

    void setMaxSpeed(double speed);
    double getMaxSpeed() const;

    void setAcceleration(double acceleration);
    double getAcceleration() const;

    double getBrakeDeceleration() const;
    void setBrakeDeceleration(double deceleration);

    // Direction
    Direction getDirection() const;
    void setDirection(Direction direction);

    // Stopped
    bool isStopped() const;

    void stop();
    void start();

    // Route
    void setRoute(std::shared_ptr<Route> route);
    Route* getRoute() const;

    // Movement
    void accelerate(double deltaTime);
    void brake(double deltaTime);

    void update(double deltaTime);

private:
    int id_;

    Vector2D position_;

    Direction direction_;

    Road* currentRoad_ = nullptr;

    int laneIndex_ = 0;

    double roadPosition_ = 0.0;

    double speed_ = 0.0;

    double maxSpeed_ = 0.0;

    double acceleration_ = 0.0;

    double brakeDeceleration_ = 5.0;

    bool stopped_ = false;

    std::shared_ptr<Route> route_;
};