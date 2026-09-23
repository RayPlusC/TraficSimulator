#include "Crossroad.h"

#include <algorithm>

Crossroad::Crossroad(int id, Vector2D position)
    : id_(id), position_(position) {}

int Crossroad::getId() const {
    return id_;
}

const Vector2D& Crossroad::getPosition() const {
    return position_;
}

void Crossroad::addRoad(Road* road) {
    if (road != nullptr &&
        std::find(roads_.begin(), roads_.end(), road) == roads_.end()) {
        roads_.push_back(road);
    }
}

const std::vector<Road*>& Crossroad::getRoads() const {
    return roads_;
}

void Crossroad::addConnection(Road* incoming,
                              Road* outgoing,
                              TurnDirection turn,
                              RightOfWay rightOfWay) {
    if (incoming == nullptr || outgoing == nullptr) {
        return;
    }

    connections_.push_back({
        incoming,
        outgoing,
        turn,
        rightOfWay
    });
}

const std::vector<CrossroadConnection>& Crossroad::getConnections() const {
    return connections_;
}

void Crossroad::setTrafficLight(
    Road* incoming,
    std::shared_ptr<TrafficLight> light) {

    if (incoming == nullptr || !light) {
        return;
    }

    for (auto& entry : trafficLights_) {
        if (entry.first == incoming) {
            entry.second = std::move(light);
            return;
        }
    }

    trafficLights_.emplace_back(incoming, std::move(light));
}

TrafficLight* Crossroad::getTrafficLight(Road* incoming) const {
    for (const auto& entry : trafficLights_) {
        if (entry.first == incoming) {
            return entry.second.get();
        }
    }

    return nullptr;
}
