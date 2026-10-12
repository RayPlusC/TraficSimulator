#pragma once

#include "Vector2D.h"
#include "TrafficTypes.h"
#include <memory>

class Road;
class Route;

class Car : public std::enable_shared_from_this<Car>
{
public:
    Car(int id, Vector2D position, Direction direction);

    int getId() const;

    const Vector2D& getPosition() const;
    void setPosition(Vector2D position);

    Vector2D getWorldPosition() const;

    void setCurrentRoad(Road* road);
    Road* getCurrentRoad() const;

    void moveToNextRoad();

    bool canOvertake() const;
    bool isLaneClear(int lane) const;
    void overtake();
    bool canReturnToRight() const;
    void returnToRightLane();
    
    void setLaneIndex(int lane);
    int getLaneIndex() const;

    void setRoadPosition(double position);
    double getRoadPosition() const;

    double getSpeed() const;
    void setMaxSpeed(double speed);
    double getMaxSpeed() const;

    void setAcceleration(double acceleration);
    double getAcceleration() const;

    double getBrakeDeceleration() const;
    void setBrakeDeceleration(double deceleration);

    Direction getDirection() const;
    void setDirection(Direction direction);

    bool isStopped() const;
    void stop();
    void start();

    void setRoute(std::shared_ptr<Route> route);
    Route* getRoute() const;

    void accelerate(double deltaTime);
    void brake(double deltaTime);
    void update(double deltaTime);

    std::shared_ptr<Car> getCarAhead() const;

private:
    int id_;
    Vector2D position_;
    Direction direction_;

    Road* currentRoad_ = nullptr;

    int laneIndex_ = 0;
    double roadPosition_ = 0.0;

    bool overtaking_ = false;
    std::weak_ptr<Car> overtakenCar_;

    double speed_ = 0.0;
    double maxSpeed_ = 0.0;
    double acceleration_ = 0.0;
    double brakeDeceleration_ = 5.0;

    bool stopped_ = false;

    std::shared_ptr<Route> route_;
};