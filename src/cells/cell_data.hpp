#ifndef CELL_DATA_HPP
#define CELL_DATA_HPP

#include <algorithm>
#include <cstdint>
#include <variant>

struct Empty {
    constexpr inline Empty operator+(const Empty &other) const { return {}; }
};

struct Byte {
    int16_t v;

    constexpr inline Byte operator+(Byte other) const { return { norm(v + other.v) }; }
    constexpr inline Byte operator-(Byte other) const { return { norm(v - other.v) }; }
    constexpr inline Byte operator*(Byte other) const { return { norm(v * other.v) }; }
    constexpr inline Byte operator/(Byte other) const { return { norm(v / other.v) }; }
    constexpr inline operator uint8_t() const { return static_cast<uint8_t>(v); }

    constexpr static inline int16_t norm(int16_t x) {
        constexpr int16_t N0 = 0;
        constexpr int16_t N255 = 255;
        return std::clamp(x, N0, N255);
    }
};

struct Color {
    Byte r, g, b;

    constexpr Color(Byte r, Byte g, Byte b) :
        r(r),
        g(g),
        b(b) {}

    constexpr Color(char r, char g, char b) :
        r(r),
        g(g),
        b(b) {}

    constexpr inline Color operator+(Color o) const { return { r + o.r, g + o.g, b + o.b }; }
    constexpr inline Color operator-(Color o) const { return { r - o.r, g - o.g, b - o.b }; }
    constexpr inline Color operator*(Color o) const { return { r * o.r, g * o.g, b * o.b }; }
    constexpr inline Color operator/(Color o) const { return { r / o.r, g / o.g, b / o.b }; }
};

struct CellData {
    using Type = std::variant<Empty, int64_t, std::string, Byte, Color>;

    Type val;

    template <typename T>
    inline CellData operator+(const CellData& other) const {
        return { std::visit([](const T& arg1, const T& arg2) -> Type { return arg1 + arg2; }, val, other.val) };
    }
};

#endif  // CELL_DATA_HPP
