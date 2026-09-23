# Traffic Simulation Model

A small C++17 traffic simulation model split into reusable classes.

## Features
- Cars with speed, acceleration, direction, position, braking and destinations
- Roads with multiple lanes, signs and cars
- Crossroads/intersections
- Traffic lights with phases
- Right-of-way rules
- Simple car-following and stopping behavior
- Routes made from roads
- Basic simulation loop

## Build

Using CMake:

```bash
cmake -S . -B build
cmake --build build
./build/traffic_sim
```

Or compile directly:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -o traffic_sim
```
