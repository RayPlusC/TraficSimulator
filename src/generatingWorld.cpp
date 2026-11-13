#include "generatingWorld.h"
#include "Route.h"

#include "Graphics.h"
#include "Vector2D.h"

GenerateWorld::GenerateWorld()
{
    createCrossroads();
    createRoads();
}

void GenerateWorld::createCrossroads()
{
    // Rectangle layout:
    //
    // C4 ---------------- C3
    // |                    |
    // |                    |
    // |                    |
    // C1 ---------------- C2

    crossroads_.push_back(
        std::make_unique<Crossroad>(
            1, Vector2D(100.0, 450.0)
        )
    );

    crossroads_.push_back(
        std::make_unique<Crossroad>(
            2, Vector2D(700.0, 450.0)
        )
    );

    crossroads_.push_back(
        std::make_unique<Crossroad>(
            3, Vector2D(700.0, 150.0)
        )
    );

    crossroads_.push_back(
        std::make_unique<Crossroad>(
            4, Vector2D(100.0, 150.0)
        )
    );
}

void GenerateWorld::createRoads()
{
    Crossroad* c1 = crossroads_[0].get();
    Crossroad* c2 = crossroads_[1].get();
    Crossroad* c3 = crossroads_[2].get();
    Crossroad* c4 = crossroads_[3].get();

    // Road 1: bottom, C1 -> C2
    roads_.push_back(
        std::make_unique<Road>(
            1, "Main Street", c1, c2, 130.0
        )
    );

    // Road 2: right, C2 -> C3
    roads_.push_back(
        std::make_unique<Road>(
            2, "2nd Street", c2, c3, 40.0
        )
    );

    // Road 3: top, C3 -> C4
    roads_.push_back(
        std::make_unique<Road>(
            3, "3rd Street", c3, c4, 60.0
        )
    );

    // Road 4: left, C4 -> C1
    roads_.push_back(
        std::make_unique<Road>(
            4, "4th Street", c4, c1, 140.0
        )
    );

    // Register each road with both of its crossroads.
    for (const auto& road : roads_)
    {
        road->getStart()->addRoad(road.get());
        road->getEnd()->addRoad(road.get());
    }
}

std::shared_ptr<Route> GenerateWorld::createRoute(
    int startingRoadIndex,
    int laneIndex)
{
    auto route = std::make_shared<Route>();

    // Validate the starting road index.
    if (startingRoadIndex < 0 ||
        startingRoadIndex >= static_cast<int>(roads_.size()))
    {
        return route;
    }

    Road* currentRoad =
        roads_[static_cast<std::size_t>(startingRoadIndex)].get();

    // Determine which crossroad the car will reach first.
    // Lane 0: start -> end
    // Lane 1: end -> start
    Crossroad* nextCrossroad =
        (laneIndex == 0)
            ? currentRoad->getEnd()
            : currentRoad->getStart();

    // Build a route containing roads connected in sequence.
    for (std::size_t i = 0; i < roads_.size(); ++i)
    {
        // Add the current road to the route.
        route->addRoad(currentRoad);

        Road* nextRoad = nullptr;

        // Find a road connected to the crossroad we will reach.
        // Do not immediately return to the road we just used.
        for (const auto& candidate : roads_)
        {
            if (candidate.get() == currentRoad)
            {
                continue;
            }

            if (candidate->getStart() == nextCrossroad ||
                candidate->getEnd() == nextCrossroad)
            {
                nextRoad = candidate.get();
                break;
            }
        }

        // No connected road was found.
        if (nextRoad == nullptr)
        {
            break;
        }

        // The car enters nextRoad at nextCrossroad.
        // Therefore, it will leave through the opposite endpoint.
        Crossroad* followingCrossroad =
            (nextRoad->getStart() == nextCrossroad)
                ? nextRoad->getEnd()
                : nextRoad->getStart();

        // Continue building the route from the new road.
        currentRoad = nextRoad;
        nextCrossroad = followingCrossroad;
    }

    return route;
}

void GenerateWorld::generateCars(int numberOfCars)
{
    if (roads_.empty() || numberOfCars <= 0)
        return;

    for (int i = 0; i < numberOfCars; ++i)
    {
        const int roadIndex = std::rand() % roads_.size();
        const int laneIndex = 1;

        Road* road = roads_[roadIndex].get();

        auto car = std::make_shared<Car>(
            i + 1,
            Vector2D(0.0, 0.0),
            Direction::East
        );

        car->setCurrentRoad(road);
        car->setLaneIndex(laneIndex);
        car->setRoadPosition(20.0);

        car->setMaxSpeed(30.0);
        car->setAcceleration(5.0);

        car->setRoute(createRoute(roadIndex, laneIndex));

        road->addCar(car);
        cars_.push_back(car);
    }
}

void GenerateWorld::drawCars(Graphics& graphics) const
{
    for (const auto& road : roads_)
    {
        for (const auto& car : road->getCars())
        {
            if (car)
            {
                graphics.drawCar(car.get());
            }
        }
    }
}

void GenerateWorld::draw(Graphics& graphics) const
{
    // Draw roads using their actual endpoint positions.
    for (const auto& road : roads_)
    {
        const Vector2D& start =
            road->getStart()->getPosition();

        const Vector2D& end =
            road->getEnd()->getPosition();

        graphics.drawRoad(start, end);
    }

    // Draw the crossroads on top of the roads.
    for (const auto& crossroad : crossroads_)
    {
        graphics.drawCrossroad(
            crossroad->getPosition()
        );
    }
}

void GenerateWorld::updateCars(double deltaTime)
{
    for (const auto& car : cars_)
    {
        if (car)
        {
            car->update(deltaTime);
        }
    }
}

Road* GenerateWorld::chooseNextRoad(
    Crossroad* crossroad,
    Road* currentRoad)
{
    if (crossroad == nullptr)
        return nullptr;

    // Get all roads connected to this crossroad.
    const auto& connectedRoads = crossroad->getRoads();

    std::vector<Road*> availableRoads;

    // Prefer roads other than the road the car just came from.
    for (Road* road : connectedRoads)
    {
        if (road != nullptr && road != currentRoad)
        {
            availableRoads.push_back(road);
        }
    }

    // If there are no alternative roads, turn around.
    if (availableRoads.empty())
    {
        return currentRoad;
    }

    // Randomly select one of the available roads.
    const std::size_t index =
        static_cast<std::size_t>(std::rand()) %
        availableRoads.size();

    return availableRoads[index];
}

const std::vector<std::unique_ptr<Crossroad>>&
GenerateWorld::getCrossroads() const
{
    return crossroads_;
}

const std::vector<std::unique_ptr<Road>>&
GenerateWorld::getRoads() const
{
    return roads_;
}