#pragma once

#include <cstddef>
#include <vector>

class Road;

class Route {
public:
    void addRoad(Road* road);

    Road* getCurrentRoad() const;
    Road* getNextRoad() const;

    void advance();
    bool finished() const;
    std::size_t size() const;

private:
    std::vector<Road*> roads_;
    std::size_t currentIndex_{0};
};
