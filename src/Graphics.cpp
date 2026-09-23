#include "Graphics.h"
#include "Car.h"
#include <cmath>
#include <algorithm>

Graphics::Graphics(unsigned int width, unsigned int height, const char* title)
    : window(sf::VideoMode(width, height), title)
{
}

bool Graphics::isOpen() const
{
    return window.isOpen();
}

void Graphics::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void Graphics::clear()
{
    window.clear(sf::Color::White);
}

void Graphics::display()
{
    window.display();
}

void Graphics::drawPixel(float x, float y, sf::Color color)
{
    sf::Vertex pixel(
        sf::Vector2f(x, y),
        color
    );

    window.draw(&pixel, 1, sf::Points);
}

void Graphics::drawLine(float x1, float y1, float x2, float y2, sf::Color color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1), color),
        sf::Vertex(sf::Vector2f(x2, y2), color)
    };

    window.draw(line, 2, sf::Lines);
}

void Graphics::drawRoad(
    const Vector2D& start,
    const Vector2D& end
)
{
    constexpr float roadWidth = 20.0f;

    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float length = std::sqrt(
        dx * dx + dy * dy
    );

    if (length == 0.0f)
        return;

    // Perpendicular unit vector
    float px = -dy / length;
    float py =  dx / length;

    float halfWidth = roadWidth / 2.0f;

    // Two sides of the road
    drawLine(
        start.x + px * halfWidth,
        start.y + py * halfWidth,

        end.x + px * halfWidth,
        end.y + py * halfWidth
    );

    drawLine(
        start.x - px * halfWidth,
        start.y - py * halfWidth,

        end.x - px * halfWidth,
        end.y - py * halfWidth
    );


    // Center dashed line
    float dashLength = 20.0f;
    float gapLength  = 20.0f;

    float ux = dx / length;
    float uy = dy / length;

    for (
        float distance = 0.0f;
        distance < length;
        distance += dashLength + gapLength
    )
    {
        float endDistance =
            std::min(
                distance + dashLength,
                length
            );

        drawLine(
            start.x + ux * distance,
            start.y + uy * distance,

            start.x + ux * endDistance,
            start.y + uy * endDistance
        );
    }
}


void Graphics::drawCrossroad(
    const Vector2D& position
)
{
    constexpr float size = 30.0f;

    // Horizontal road
    drawLine(
        position.x - size,
        position.y - 10,

        position.x + size,
        position.y - 10
    );

    drawLine(
        position.x - size,
        position.y + 10,

        position.x + size,
        position.y + 10
    );

    // Vertical road
    drawLine(
        position.x - 10,
        position.y - size,

        position.x - 10,
        position.y + size
    );

    drawLine(
        position.x + 10,
        position.y - size,

        position.x + 10,
        position.y + size
    );
}


void Graphics::drawCar(const Car* car)
{
    if (car == nullptr)
        return;

    Vector2D position = car->getWorldPosition();
    Direction direction = car->getDirection();

    float width = 10.0f;
    float height = 5.0f;

    if (direction == Direction::North ||
        direction == Direction::South)
    {
        std::swap(width, height);
    }

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            drawPixel(
                position.x + x - width / 2,
                position.y + y - height / 2,
                sf::Color::Red
            );
        }
    }
}

sf::RenderWindow& Graphics::getWindow()
{
    return window;
}