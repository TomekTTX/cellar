#ifndef POSITIONS_HPP
#define POSITIONS_HPP

#include <array>

#include <QRect>

enum Dir { NONE, UP, RIGHT, DOWN, LEFT, COUNT };

constexpr inline std::array<Dir, Dir::COUNT - Dir::UP> DIRS{ UP, RIGHT, DOWN, LEFT };

constexpr inline Dir operator-(Dir d) {
    return static_cast<Dir>(d != Dir::NONE ? 1 + ((d + 1) & 3) : 0);
}

template <typename T>
struct VecT {
    T x, y;

    constexpr inline bool operator==(const VecT &other) const = default;

    constexpr inline VecT operator-() const { return { -x, -y }; }
    constexpr inline VecT operator+(const VecT &other) const { return { x + other.x, y + other.y }; }
    constexpr inline VecT operator-(const VecT &other) const { return operator+(-other); }
    constexpr inline VecT operator*(int mul) const { return { mul * x, mul * y }; }

    constexpr inline VecT operator+=(const VecT &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    constexpr inline VecT operator-=(const VecT &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    constexpr inline VecT operator*=(int mul) {
        x *= mul;
        y *= mul;
        return *this;
    }

    constexpr inline VecT abs() const { return { std::abs(x), std::abs(y) }; }

    constexpr inline VecT operator-(Dir d) const { return operator+(-d); }
    constexpr inline VecT operator-=(Dir d) { return operator+=(-d); }
    constexpr inline VecT operator+(Dir d) const {
        VecT v = *this;
        return v += d;
    }
    constexpr inline VecT operator+=(Dir d) {
        switch (d) {
        case Dir::UP: --y; break;
        case Dir::RIGHT: ++x; break;
        case Dir::DOWN: ++y; break;
        case Dir::LEFT: --x; break;
        default: break;
        }
        return *this;
    }
    constexpr inline Dir toDir() const {
        if (x == 1) return Dir::RIGHT;
        if (x == -1) return Dir::LEFT;
        if (y == 1) return Dir::DOWN;
        if (y == -1) return Dir::UP;
        return Dir::NONE;
    }
    inline std::string str() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }
};

using Vec = VecT<int>;

#endif  // POSITIONS_HPP
