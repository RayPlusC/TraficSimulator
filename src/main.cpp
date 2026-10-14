#include <iostream>
#include <memory>

#include "Graphics.h"
#include "Vector2D.h"
#include "Road.h"
#include "Route.h"
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

    graphics.setFramerateLimit(60);

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
            3,
            Vector2D(700, 100)
        );
    auto crossroads4 =
        std::make_shared<Crossroad>(
            4,
            Vector2D(100, 100)
        );

    auto road1 =
        std::make_shared<Road>(
            1,
            "Main Street",
            crossroads1.get(),
            crossroads2.get(),
            130
        );

    auto road2 =
        std::make_shared<Road>(
            2,
            "2nd Street",
            crossroads2.get(),
            crossroads3.get(),
            40
        );
    
    auto road3 =
        std::make_shared<Road>(
            3,
            "3rd Street",
            crossroads3.get(),
            crossroads4.get(),
            60
        );

    auto road4 =
        std::make_shared<Road>(
            4,
            "4th Street",
            crossroads4.get(),
            crossroads1.get(),
            140
        );

    crossroads1->addRoad(road1.get());
    crossroads2->addRoad(road1.get());
    
    crossroads2->addRoad(road2.get());
    crossroads3->addRoad(road2.get());

    crossroads3->addRoad(road3.get());
    crossroads4->addRoad(road3.get());
    
    crossroads4->addRoad(road4.get());
    crossroads1->addRoad(road4.get());

    std::cout << "\nCrossroad 2 connections:\n";

    for (Road* road : crossroads2->getRoads())
    {
        if (road)
        {
            std::cout << "Road ID: " << road->getId()
                    << " | Start: "
                    << (road->getStart()
                        ? road->getStart()->getId() : -1)
                    << " | End: "
                    << (road->getEnd()
                        ? road->getEnd()->getId() : -1)
                    << '\n';
        }
    }
    
    auto route = std::make_shared<Route>();

    route->addRoad(road1.get());
    route->addRoad(road2.get());
    route->addRoad(road3.get());
    route->addRoad(road4.get());

    // --------------------------------------------------------
    // Road signs
    // --------------------------------------------------------
/*
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

*/
    // --------------------------------------------------------
    // Cars
    // --------------------------------------------------------

    // Car 1: travelling east
    auto car1 = std::make_shared<Car>(
        1, Vector2D(100, 300), Direction::East
    );

    car1->setLaneIndex(0);
    car1->setRoadPosition(0.0);
    car1->setMaxSpeed(120);
    car1->setAcceleration(1.2);
    car1->setCurrentRoad(road1.get());
    car1->setRoute(route);
    
    road1->addCar(car1);


    // Car 2: travelling west
    auto car2 = std::make_shared<Car>(
        2, Vector2D(400, 300), Direction::West
    );

    car2->setLaneIndex(1);
    car2->setRoadPosition(300.0);
    car2->setMaxSpeed(130);
    car2->setAcceleration(1.0);
    car2->setCurrentRoad(road1.get());
    car2->setRoute(route);

    road1->addCar(car2);


    // Car 3: travelling east
    auto car3 = std::make_shared<Car>(
        3, Vector2D(600, 300), Direction::East
    );

    car3->setLaneIndex(0);
    car3->setRoadPosition(500.0);
    car3->setMaxSpeed(140);
    car3->setAcceleration(1.6);
    car3->setCurrentRoad(road1.get());
    car3->setRoute(route);

    road1->addCar(car3);


    // Car 4: travelling east
    auto car4 = std::make_shared<Car>(
        4, Vector2D(600, 300), Direction::East
    );

    car4->setLaneIndex(0);
    car4->setRoadPosition(550.0);
    car4->setMaxSpeed(150);
    car4->setAcceleration(2.0);
    car4->setCurrentRoad(road1.get());
    car4->setRoute(route);

    road1->addCar(car4);

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
    simulation.addCar(car3);
    simulation.addCar(car4);


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
        graphics.drawCar(car3.get());
        graphics.drawCar(car4.get());

        graphics.display();
/*
        std::system("clear");
        std::cout << "speed" << "\t|\t" << std::endl;
        std::cout << car1->getSpeed() << "\t|\t" << std::endl;
        std::cout << car2->getSpeed() << "\t|\t" << std::endl;
        std::cout << car3->getSpeed() << "\t|\t" << std::endl;
        std::cout << car4->getSpeed() << "\t|\t" << std::endl;
  */    
    }


    return 0;
}
