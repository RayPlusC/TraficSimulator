#pragma once

#include "TrafficLight.h"
#include "TrafficTypes.h"
#include "Vector2D.h"

#include <memory>
#include <vector>

class Road;

struct CrossroadConnection {
    Road* incomingRoad;
    Road* outgoingRoad;
    TurnDirection turn;
    RightOfWay rightOfWay;
};

class Crossroad {
public:
    Crossroad(int id, Vector2D position);

    int getId() const;
    const Vector2D& getPosition() const;

    void addRoad(Road* road);
    const std::vector<Road*>& getRoads() const;

    void addConnection(Road* incoming,
                       Road* outgoing,
                       TurnDirection turn,
                       RightOfWay rightOfWay);

    const std::vector<CrossroadConnection>& getConnections() const;

    void setTrafficLight(Road* incoming,
                         std::shared_ptr<TrafficLight> light);

    TrafficLight* getTrafficLight(Road* incoming) const;

private:
    int id_;
    Vector2D position_;

    std::vector<Road*> roads_;
    std::vector<CrossroadConnection> connections_;

    std::vector<std::pair<Road*, std::shared_ptr<TrafficLight>>> trafficLights_;
};
