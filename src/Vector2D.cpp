#include "Vector2D.h"

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return {x + other.x, y + other.y};
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return {x - other.x, y - other.y};
}

Vector2D Vector2D::operator*(double scalar) const {
    return {x * scalar, y * scalar};
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

double Vector2D::length() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalized() const {
    const double len = length();
    if (len == 0.0) {
        return {};
    }
    return {x / len, y / len};
}
