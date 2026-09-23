#include "Route.h"
#include "Road.h"

void Route::addRoad(Road* road) {
    if (road != nullptr) {
        roads_.push_back(road);
    }
}

Road* Route::getCurrentRoad() const {
    if (finished()) {
        return nullptr;
    }
    return roads_[currentIndex_];
}

Road* Route::getNextRoad() const {
    if (currentIndex_ + 1 >= roads_.size()) {
        return nullptr;
    }
    return roads_[currentIndex_ + 1];
}

void Route::advance() {
    if (!finished()) {
        ++currentIndex_;
    }
}

bool Route::finished() const {
    return currentIndex_ >= roads_.size();
}

std::size_t Route::size() const {
    return roads_.size();
}
