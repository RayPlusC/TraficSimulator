#include "RoadSign.h"

#include <algorithm>

RoadSign::RoadSign(RoadSignType type, Vector2D position)
    : type_(type), position_(position) {}

RoadSignType RoadSign::getType() const {
    return type_;
}

const Vector2D& RoadSign::getPosition() const {
    return position_;
}

StopSign::StopSign(Vector2D position)
    : RoadSign(RoadSignType::Stop, position) {}

std::string StopSign::getName() const {
    return "STOP";
}

GiveWaySign::GiveWaySign(Vector2D position)
    : RoadSign(RoadSignType::GiveWay, position) {}

std::string GiveWaySign::getName() const {
    return "Give Way";
}

SpeedLimitSign::SpeedLimitSign(Vector2D position, double speedLimit)
    : RoadSign(RoadSignType::SpeedLimit, position),
      speedLimit_(std::max(0.0, speedLimit)) {}

double SpeedLimitSign::getSpeedLimit() const {
    return speedLimit_;
}

void SpeedLimitSign::setSpeedLimit(double speedLimit) {
    speedLimit_ = std::max(0.0, speedLimit);
}

std::string SpeedLimitSign::getName() const {
    return "Speed Limit";
}

NoEntrySign::NoEntrySign(Vector2D position)
    : RoadSign(RoadSignType::NoEntry, position) {}

std::string NoEntrySign::getName() const {
    return "No Entry";
}

PedestrianCrossingSign::PedestrianCrossingSign(Vector2D position)
    : RoadSign(RoadSignType::PedestrianCrossing, position) {}

std::string PedestrianCrossingSign::getName() const {
    return "Pedestrian Crossing";
}
