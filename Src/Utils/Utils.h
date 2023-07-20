#pragma once
#include <random>

#define PI 3.14159265359f

template<typename T>
class Vector2 {
public:
    T x, y;

    // Constructor
    Vector2(T x = 0, T y = 0) : x(x), y(y) {}

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
        return Color(red + right.red, green + right.green, blue + right.blue);
    }

    // Overload of the binary - operator
    Color operator-(const Color &right) const {
        return Color(red - right.red, green - right.green, blue - right.blue);
    }

    // Overload of the binary * operator
    Color operator*(const Color &right) const {
        return Color(red * right.red, green * right.green, blue * right.blue);
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

class NumberGenerator
{
protected:
    std::random_device rd;
    std::mt19937 gen;

    NumberGenerator()
            : gen(rd())
    {}
};


template<typename T>
class RealNumberGenerator : public NumberGenerator
{
private:
    std::uniform_real_distribution<T> dis;

public:
    RealNumberGenerator()
            : NumberGenerator()
            , dis(0.0f, 1.0f)
    {}

    // random_device is not copyable
    RealNumberGenerator(const RealNumberGenerator<T>& right)
            : NumberGenerator()
            , dis(right.dis)
    {}

    float get()
    {
        return dis(gen);
    }

    float getUnder(T max)
    {
        return get() * max;
    }

    float getRange(T min, T max)
    {
        return min + get() * (max - min);
    }

    float getRange(T width)
    {
        return getRange(-width * 0.5f, width * 0.5f);
    }
};


template<typename T>
class RNG
{
private:
    static RealNumberGenerator<T> gen;

public:
    static T get()
    {
        return gen.get();
    }

    static float getUnder(T max)
    {
        return gen.getUnder(max);
    }

    static uint64_t getUintUnder(uint64_t max)
    {
        return static_cast<uint64_t>(gen.getUnder(static_cast<float>(max) + 1.0f));
    }

    static float getRange(T min, T max)
    {
        return gen.getRange(min, max);
    }

    static float getRange(T width)
    {
        return gen.getRange(width);
    }

    static float getFullRange(T width)
    {
        return gen.getRange(static_cast<T>(2.0f) * width);
    }

    static bool proba(float threshold)
    {
        return get() < threshold;
    }
};

using RNGf = RNG<float>;

template<typename T>
RealNumberGenerator<T> RNG<T>::gen = RealNumberGenerator<T>();


template<typename T>
class IntegerNumberGenerator : public NumberGenerator
{
public:
    IntegerNumberGenerator()
            : NumberGenerator()
    {}

    // random_device is not copyable
    IntegerNumberGenerator(const IntegerNumberGenerator<T>& right)
            : NumberGenerator()
    {}

    T getUnder(T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, max);
        return dist(gen);
    }

    T getRange(T min, T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(gen);
    }
};


template<typename T>
class RNGi
{
private:
    static IntegerNumberGenerator<T> gen;

public:
    static T getUnder(T max)
    {
        return gen.getUnder(max);
    }

    static T getRange(T min, T max)
    {
        return gen.getRange(min, max);
    }
};

template<typename T>
IntegerNumberGenerator<T> RNGi<T>::gen;

using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;
