#pragma once

#include "TrafficTypes.h"
#include "Vector2D.h"

#include <memory>
#include <string>

class RoadSign {
public:
    RoadSign(RoadSignType type, Vector2D position);
    virtual ~RoadSign() = default;

    RoadSignType getType() const;
    const Vector2D& getPosition() const;

    virtual std::string getName() const = 0;

private:
    RoadSignType type_;
    Vector2D position_;
};

class StopSign final : public RoadSign {
public:
    explicit StopSign(Vector2D position);
    std::string getName() const override;
};

class GiveWaySign final : public RoadSign {
public:
    explicit GiveWaySign(Vector2D position);
    std::string getName() const override;
};

class SpeedLimitSign final : public RoadSign {
public:
    SpeedLimitSign(Vector2D position, double speedLimit);

    double getSpeedLimit() const;
    void setSpeedLimit(double speedLimit);

    std::string getName() const override;

private:
    double speedLimit_;
};

class NoEntrySign final : public RoadSign {
public:
    explicit NoEntrySign(Vector2D position);
    std::string getName() const override;
};

class PedestrianCrossingSign final : public RoadSign {
public:
    explicit PedestrianCrossingSign(Vector2D position);
    std::string getName() const override;
};
