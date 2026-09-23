#include <iostream>
#include <memory>

#include "Graphics.h"
#include "Vector2D.h"
#include "Road.h"
#include "Crossroad.h"
#include "Car.h"
#include "Simulation.h"
#include "RoadSign.h"
#include "TrafficLight.h"

int main()
{
    // --------------------------------------------------------
    // Graphics
    // --------------------------------------------------------

    Graphics graphics(800, 600, "Traffic Simulator");


    // --------------------------------------------------------
    // Build the road network
    // --------------------------------------------------------

    auto crossroads1 =
        std::make_shared<Crossroad>(
            1,
            Vector2D(100, 300)
        );

    auto crossroads2 =
        std::make_shared<Crossroad>(
            2,
            Vector2D(700, 300)
        );
    auto crossroads3 =
        std::make_shared<Crossroad>(
            2,
            Vector2D(700, 100)
        );
    auto crossroads4 =
        std::make_shared<Crossroad>(
            2,
            Vector2D(100, 100)
        );

    auto road1 =
        std::make_shared<Road>(
            1,
            "Main Street",
            crossroads1.get(),
            crossroads2.get(),
            5,
            2
        );

    auto road2 =
        std::make_shared<Road>(
            2,
            "2nd Street",
            crossroads2.get(),
            crossroads3.get(),
            20,
            2
        );
    
    auto road3 =
        std::make_shared<Road>(
            2,
            "3rd Street",
            crossroads3.get(),
            crossroads4.get(),
            10,
            2
        );

    auto road4 =
        std::make_shared<Road>(
            2,
            "4th Street",
            crossroads4.get(),
            crossroads1.get(),
            13.89,
            2
        );

    crossroads1->addRoad(road1.get());
    crossroads2->addRoad(road1.get());
    
    crossroads2->addRoad(road2.get());
    crossroads3->addRoad(road2.get());

    crossroads3->addRoad(road3.get());
    crossroads4->addRoad(road3.get());
    
    crossroads4->addRoad(road4.get());
    crossroads1->addRoad(road4.get());



    // --------------------------------------------------------
    // Road signs
    // --------------------------------------------------------

    road1->addSign(
        std::make_shared<SpeedLimitSign>(
            Vector2D(300, 300),
            5
        )
    );

    road2->addSign(
        std::make_shared<SpeedLimitSign>(
            Vector2D(400, 300),
            1
        )
    );

    road1->addSign(
        std::make_shared<PedestrianCrossingSign>(
            Vector2D(500, 300)
        )
    );


    // --------------------------------------------------------
    // Traffic light
    // --------------------------------------------------------

    auto trafficLight =
        std::make_shared<TrafficLight>(
            10.0,
            3.0,
            10.0
        );

    crossroads2->setTrafficLight(
        road2.get(),
        trafficLight
    );


    // --------------------------------------------------------
    // Cars
    // --------------------------------------------------------

    auto car1 =
    std::make_shared<Car>(
        1,
        Vector2D(100, 290),
        Direction::East
    );

    car1->setCurrentRoad(road1.get());
    car1->setLaneIndex(1);
    car1->setRoadPosition(0.0);
    car1->setMaxSpeed(8.89);
    car1->setAcceleration(1.2);

    road1->addCar(car1);


    auto car2 =
        std::make_shared<Car>(
            2,
            Vector2D(700, 290),
            Direction::South
        );

    car2->setCurrentRoad(road2.get());
    car2->setLaneIndex(0);
    car2->setRoadPosition(0.0);
    car2->setMaxSpeed(5.89);
    car2->setAcceleration(1.0);

    road2->addCar(car2);

    // --------------------------------------------------------
    // Simulation
    // --------------------------------------------------------

    Simulation simulation;

    simulation.addCrossroad(crossroads1);
    simulation.addCrossroad(crossroads2);
    simulation.addCrossroad(crossroads3);
    simulation.addCrossroad(crossroads4);

    simulation.addRoad(road1);
    simulation.addRoad(road2);
    simulation.addRoad(road3);
    simulation.addRoad(road4);

    simulation.addCar(car1);
    simulation.addCar(car2);


    // --------------------------------------------------------
    // Graphics loop
    // --------------------------------------------------------

    while (graphics.isOpen())
    {
        graphics.processEvents();

        // Update simulation first
        simulation.update(0.016);

        // Then draw
        graphics.clear();

        graphics.drawRoad(
            crossroads1->getPosition(),
            crossroads2->getPosition()
        );

        graphics.drawRoad(
            crossroads2->getPosition(),
            crossroads3->getPosition()
        );

        graphics.drawRoad(
            crossroads3->getPosition(),
            crossroads4->getPosition()
        );

        graphics.drawRoad(
            crossroads4->getPosition(),
            crossroads1->getPosition()
        );

        graphics.drawCrossroad(
            crossroads1->getPosition()
        );

        graphics.drawCrossroad(
            crossroads2->getPosition()
        );

        graphics.drawCrossroad(
            crossroads3->getPosition()
        );

        graphics.drawCrossroad(
            crossroads4->getPosition()
        );

        graphics.drawCar(car1.get());
        graphics.drawCar(car2.get());

        graphics.display();
    }


    return 0;
}
