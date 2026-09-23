#pragma once

enum class Direction {
    North,
    East,
    South,
    West
};

enum class TurnDirection {
    Left,
    Straight,
    Right
};

enum class RoadSignType {
    Stop,
    GiveWay,
    SpeedLimit,
    NoEntry,
    PedestrianCrossing
};

enum class TrafficLightState {
    Red,
    Yellow,
    Green
};

enum class RightOfWay {
    None,
    Priority,
    Yield,
    Stop
};
