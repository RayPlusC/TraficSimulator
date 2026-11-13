#include <iostream>
#include <memory>
#include <initializer_list>

#include "Graphics.h"
#include "Vector2D.h"
#include "Road.h"
#include "Route.h"
#include "Crossroad.h"
#include "Car.h"
#include "Simulation.h"
#include "RoadSign.h"
#include "TrafficLight.h"

#include "generatingWorld.h"

int main()
{
    Graphics graphics(
        800,
        600,
        "Traffic Simulator"
    );

    graphics.setFramerateLimit(60);

    // Create the world and generate 20 cars.
    GenerateWorld world;
    world.generateCars(25);

    sf::Clock clock;

    while (graphics.isOpen())
    {
        graphics.processEvents();

        double deltaTime = clock.restart().asSeconds();

        world.updateCars(deltaTime);

        graphics.clear();
        world.draw(graphics);
        world.drawCars(graphics);
        graphics.display();
    }

    return 0;
}