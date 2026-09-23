#pragma once

#include <cmath>

struct Vector2D {
    double x{0.0};
    double y{0.0};

    Vector2D() = default;
    Vector2D(double xValue, double yValue) : x(xValue), y(yValue) {}

    double length() const;
    Vector2D normalized() const;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    Vector2D& operator+=(const Vector2D& other);
};
