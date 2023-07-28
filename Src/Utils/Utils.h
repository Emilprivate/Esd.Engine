#pragma once
#include <random>

#define PI 3.14159265359f

template<typename T>
class Vector2 {
public:
    T x, y;

    // Constructor
    Vector2(T x = 0, T y = 0) : x(x), y(y) {}

    // Dot product
    [[nodiscard]] T Dot(const Vector2<T>& other) const {
        return x * other.x + y * other.y;
    }

    // Length squared (magnitude squared)
    [[nodiscard]] T LengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] Vector2<float> Normalized() const {
        float length = Length();
        if (length != 0.0f) {
            return Vector2<float>(x / length, y / length);
        } else {
            return Vector2<float>(0.0f, 0.0f);
        }
    }

    // Length (magnitude)
    [[nodiscard]] float Length() const {
        return std::sqrt(LengthSquared());
    }

    // Unary operator -
    Vector2<T> operator-() const {
        return Vector2<T>(-x, -y);
    }

    // Binary operator +=
    Vector2<T>& operator+=(const Vector2<T> &right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    // Binary operator -=
    Vector2<T>& operator-=(const Vector2<T> &right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    // Binary operator +
    Vector2<T> operator+(const Vector2<T> &right) const {
        return Vector2<T>(x + right.x, y + right.y);
    }

    // Binary operator -
    Vector2<T> operator-(const Vector2<T> &right) const {
        return Vector2<T>(x - right.x, y - right.y);
    }

    // Binary operator * (Vector2 * T)
    Vector2<T> operator*(T right) const {
        return Vector2<T>(x * right, y * right);
    }

    // Binary operator *= (Vector2 *= T)
    Vector2<T>& operator*=(T right) {
        x *= right;
        y *= right;
        return *this;
    }

    // Binary operator / (Vector2 / T)
    Vector2<T> operator/(T right) const {
        assert(right != 0);
        return Vector2<T>(x / right, y / right);
    }

    // Binary operator /= (Vector2 /= T)
    Vector2<T>& operator/=(T right) {
        assert(right != 0);
        x /= right;
        y /= right;
        return *this;
    }

    // Binary operator ==
    bool operator==(const Vector2<T> &right) const {
        return x == right.x && y == right.y;
    }

    // Binary operator !=
    bool operator!=(const Vector2<T> &right) const {
        return !(*this == right);
    }
};

// Binary operator * (T * Vector2)
template<typename T>
Vector2<T> operator*(T left, const Vector2<T> &right) {
    return Vector2<T>(right.x * left, right.y * left);
}

class Color {
public:
    // Assuming Color has three components: red, green, blue
    float red, green, blue;

    Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f) : red(red), green(green), blue(blue) {}

    // Overload of the == operator
    bool operator==(const Color &right) const {
        return red == right.red && green == right.green && blue == right.blue;
    }

    // Overload of the != operator
    bool operator!=(const Color &right) const {
        return !(*this == right);
    }

    // Overload of the binary + operator
    Color operator+(const Color &right) const {
        return {red + right.red, green + right.green, blue + right.blue};
    }

    // Overload of the binary - operator
    Color operator-(const Color &right) const {
        return {red - right.red, green - right.green, blue - right.blue};
    }

    // Overload of the binary * operator
    Color operator*(const Color &right) const {
        return {red * right.red, green * right.green, blue * right.blue};
    }

    // Overload of the binary += operator
    Color& operator+=(const Color &right) {
        red += right.red;
        green += right.green;
        blue += right.blue;
        return *this;
    }

    // Overload of the binary -= operator
    Color& operator-=(const Color &right) {
        red -= right.red;
        green -= right.green;
        blue -= right.blue;
        return *this;
    }

    // Overload of the binary *= operator
    Color& operator*=(const Color &right) {
        red *= right.red;
        green *= right.green;
        blue *= right.blue;
        return *this;
    }
};