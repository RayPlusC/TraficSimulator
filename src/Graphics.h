#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2D.h"

class Car;

class Graphics
{
public:
    Graphics(unsigned int width, unsigned int height, const char* title);

    bool isOpen() const;
    void processEvents();

    void clear();
    void display();

    void drawPixel(float x, float y, sf::Color color = sf::Color::Black);
    void drawLine(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::Black);
    void drawRoad(const Vector2D& start, const Vector2D& end);
    void drawCrossroad(const Vector2D& position);
    void drawCar(const Car* car);
    
    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window;
};