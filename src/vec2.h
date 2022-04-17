#ifndef VEC2_H
#define VEC2_H

#include <cmath>

struct Vec2 {
  float x = 0.;
  float y = 0.;

  Vec2() = default;

  Vec2(const Vec2 &) = default;

  Vec2(Vec2 &&) = default;

  Vec2 &operator=(const Vec2 &) = default;

  Vec2(float x, float y) : x(x), y(y) {}

  float abs() const { return std::sqrt(x * x + y * y); }

  float sqr_abs() const { return x * x + y * y; }

  const Vec2 operator-() const { return Vec2(-x, -y); }

  Vec2 &operator+=(const Vec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vec2 &operator-=(const Vec2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vec2 &operator*=(const float &alpha) {
    x *= alpha;
    y *= alpha;
    return *this;
  }

  Vec2 &operator/=(const float &alpha) {
    x /= alpha;
    y /= alpha;
    return *this;
  }

  // implace normalization
  Vec2 &inormalize() {
    const float _abs = this->abs();
    this->x /= _abs;
    this->y /= _abs;
    return *this;
  }

  // normalization
  const Vec2 normalize() const {
    const float _abs = this->abs();
    return Vec2(this->x / _abs, this->y / _abs);
  }
};

inline const Vec2 operator+(const Vec2 &first, const Vec2 &second) {
  return Vec2(first.x + second.x, first.y + second.y);
}

inline const Vec2 operator-(const Vec2 &first, const Vec2 &second) {
  return Vec2(first.x - second.x, first.y - second.y);
}

inline const Vec2 operator*(const Vec2 &vec, const float &alpha) {
  return Vec2(vec) *= alpha;
}

inline const Vec2 operator*(const float &alpha, const Vec2 &vec) {
  return Vec2(vec) *= alpha;
}

inline const Vec2 operator/(const Vec2 &vec, const float &alpha) {
  return Vec2(vec) /= alpha;
}

// dot product
inline float operator*(const Vec2 &first, const Vec2 &second) {
  return first.x * second.x + first.y * second.y;
}

// z-coord of cross product
/* |i j k| |
 * |a b 0| | = (a * y - b * x)k
 * |x y 0| |k
 */
inline float operator^(const Vec2 &first, const Vec2 &second) {
  return first.x * second.y - first.y * second.x;
}

inline Vec2 normal(const Vec2 &point, const Vec2 &line0, const Vec2 &line1) {
  const Vec2 pl0 = line0 - point;
  const Vec2 pl1 = line1 - point;
  const Vec2 l0l1 = line1 - line0;

  return (pl0 ^ pl1) * Vec2(l0l1.y, -l0l1.x) / l0l1.sqr_abs();
}

#endif // VEC2_H
