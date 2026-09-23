#pragma once

#include "TrafficTypes.h"

class TrafficLight {
public:
    explicit TrafficLight(double greenDuration = 20.0,
                          double yellowDuration = 3.0,
                          double redDuration = 20.0);

    void update(double deltaTime);

    TrafficLightState getState() const;
    double getTimeInState() const;
    void setState(TrafficLightState state);

private:
    TrafficLightState state_{TrafficLightState::Red};
    double timeInState_{0.0};

    double greenDuration_;
    double yellowDuration_;
    double redDuration_;
};
