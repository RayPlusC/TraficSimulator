#include "TrafficLight.h"

#include <algorithm>

TrafficLight::TrafficLight(double greenDuration,
                           double yellowDuration,
                           double redDuration)
    : greenDuration_(std::max(0.0, greenDuration)),
      yellowDuration_(std::max(0.0, yellowDuration)),
      redDuration_(std::max(0.0, redDuration)) {}

void TrafficLight::update(double deltaTime) {
    if (deltaTime <= 0.0) {
        return;
    }

    timeInState_ += deltaTime;

    double duration = 0.0;
    switch (state_) {
        case TrafficLightState::Green:
            duration = greenDuration_;
            break;
        case TrafficLightState::Yellow:
            duration = yellowDuration_;
            break;
        case TrafficLightState::Red:
            duration = redDuration_;
            break;
    }

    if (duration <= 0.0) {
        return;
    }

    if (timeInState_ >= duration) {
        timeInState_ = 0.0;

        switch (state_) {
            case TrafficLightState::Green:
                state_ = TrafficLightState::Yellow;
                break;
            case TrafficLightState::Yellow:
                state_ = TrafficLightState::Red;
                break;
            case TrafficLightState::Red:
                state_ = TrafficLightState::Green;
                break;
        }
    }
}

TrafficLightState TrafficLight::getState() const {
    return state_;
}

double TrafficLight::getTimeInState() const {
    return timeInState_;
}

void TrafficLight::setState(TrafficLightState state) {
    state_ = state;
    timeInState_ = 0.0;
}
