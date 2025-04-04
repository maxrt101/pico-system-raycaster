#pragma once

template <typename T>
struct Vec2 {
  T x, y;

  Vec2() : x(T{}), y(T{}) {}
  Vec2(T x, T y) : x(x), y(y) {}

  template <typename I>
  Vec2(I x, I y) : x(T(x)), y(T(y)) {}

  Vec2(const Vec2& rhs) : x(rhs.x), y(rhs.y) {}

  template <typename I>
  Vec2(const Vec2<I>& rhs) : x(T(rhs.x)), y(T(rhs.y)) {}

  ~Vec2() = default;

  Vec2& operator=(const Vec2& rhs) = default;

  T dot(const Vec2& rhs)   const { return x * rhs.x + y * rhs.y; }
  T cross(const Vec2& rhs) const { return x * rhs.y - y * rhs.x; }

  Vec2  operator-()                 const { return {-x, -y}; }
  Vec2  operator +(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
  Vec2  operator +(const T& rhs)    const { return {x + rhs, y + rhs}; }
  Vec2  operator -(const Vec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
  Vec2  operator -(const T& rhs)    const { return {x - rhs, y - rhs}; }
  Vec2  operator *(const Vec2& rhs) const { return {x * rhs.x, y * rhs.y}; }
  Vec2  operator *(const T& rhs)    const { return {x * rhs, y * rhs}; }
  Vec2  operator /(const Vec2& rhs) const { return {x / rhs.x, y / rhs.y}; }
  Vec2  operator /(const T& rhs)    const { return {x / rhs, y / rhs}; }
  Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
  Vec2& operator+=(const T& rhs)    { x += rhs; y = rhs; return *this; }
  Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
  Vec2& operator-=(const T& rhs)    { x -= rhs; y -= rhs; return *this; }
  Vec2& operator*=(const Vec2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
  Vec2& operator*=(const T& rhs)    { x *= rhs; y *= rhs; return *this; }
  Vec2& operator/=(const Vec2& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
  Vec2& operator/=(const T& rhs)    { x /= rhs; y /= rhs; return *this; }
  bool  operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }
  bool  operator!=(const Vec2& rhs) const { return x != rhs.x || y != rhs.y; }

  std::string toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

  template <typename I>
  operator Vec2<I>() const { return {I(x), I(y)}; }
};
