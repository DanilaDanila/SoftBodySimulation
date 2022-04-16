#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "vec2.h"

TEST_CASE("mock test") { CHECK(true); }

TEST_CASE("just compiles") { Vec2 v(); }

TEST_CASE("constructor") {
  {
    Vec2 v;
    CHECK(v.x == 0.f);
    CHECK(v.y == 0.f);
  }
  {
    float x = 123.f;
    float y = 456.f;
    Vec2 v(x, y);
    CHECK(v.x == x);
    CHECK(v.y == y);
  }
}

TEST_CASE("abs") {
  const float eps = 1e-4f;
  CHECK(Vec2(0.f, 0.f).abs() == 0.f);
  CHECK(Vec2(1.f, 0.f).abs() - 1.f < eps);
  CHECK(Vec2(0.f, 1.f).abs() - 1.f < eps);
  CHECK(Vec2(3.f, 4.f).abs() - 5.f < eps);
  CHECK(Vec2(4.f, 3.f).abs() - 5.f < eps);
}

TEST_CASE("sqr_abs") {
  const float eps = 1e-4f;
  CHECK(Vec2(0.f, 0.f).sqr_abs() == 0.f);
  CHECK(Vec2(1.f, 0.f).sqr_abs() - 1.f < eps);
  CHECK(Vec2(0.f, 1.f).sqr_abs() - 1.f < eps);
  CHECK(Vec2(3.f, 4.f).sqr_abs() - 25.f < eps);
  CHECK(Vec2(4.f, 3.f).sqr_abs() - 25.f < eps);
}

TEST_CASE("operator-") {
  {
    Vec2 v(0.f, 0.f);
    Vec2 vv = -v;
    CHECK(v.x == -vv.x);
    CHECK(v.y == -vv.y);
  }
  {
    float x = 123.f;
    float y = 456.f;
    Vec2 v(x, y);
    Vec2 vv = -v;
    CHECK(v.x == -vv.x);
    CHECK(v.y == -vv.y);
  }
}

TEST_CASE("operator v+v") {
  const float eps = 1e-4;
  float fx = 123.f;
  float fy = 567.f;
  float sx = 345.f;
  float sy = 678.f;
  Vec2 v(fx, fy);
  Vec2 vv(sx, sy);
  Vec2 vvv = v + vv;
  CHECK(vvv.x - (fx + sx) < eps);
  CHECK(vvv.y - (fy + sy) < eps);
}

TEST_CASE("operator v-v") {
  const float eps = 1e-4;
  float fx = 123.f;
  float fy = 567.f;
  float sx = 345.f;
  float sy = 678.f;
  Vec2 v(fx, fy);
  Vec2 vv(sx, sy);
  Vec2 vvv = v - vv;
  CHECK(vvv.x - (fx - sx) < eps);
  CHECK(vvv.y - (fy - sy) < eps);
}

TEST_CASE("operator v*alpha") {
  const float eps = 1e-4;
  Vec2 v(123.f, 456.f);
  v = v * 3.1415;
  CHECK(v.x - 123.f * 3.1415f < eps);
  CHECK(v.y - 456.f * 3.1415f < eps);
}

TEST_CASE("dot product") {
  const float eps = 1e-4;
  Vec2 v0(2.f, 3.f);
  Vec2 v1(4.f, 5.f);

  CHECK(v0 * v1 - (2.f * 4.f + 3.f * 5.f) < eps);
}

TEST_CASE("cross product") {
  const float eps = 1e-4;
  {
    Vec2 v0(1.f, 0.f);
    Vec2 v1(0.f, 1.f);
    CHECK((v0 ^ v1) - 1. < eps);
    CHECK((v1 ^ v0) + 1. < eps);
  }
  {
    Vec2 v0(2.f, 0.f);
    Vec2 v1(0.f, 1.f);
    CHECK((v0 ^ v1) - 2. < eps);
    CHECK((v1 ^ v0) + 2. < eps);
  }
}
