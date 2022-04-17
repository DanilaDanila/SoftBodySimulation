#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "phys.h"

TEST_CASE("mock test") { CHECK(true); }

TEST_CASE("point constructor") {
  const float eps = 1e-4;
  {
    float x = 123.f;
    float y = 456.f;
    const Point p(Vec2(x, y));
    CHECK((p.current_position - Vec2(x, y)).sqr_abs() < eps);
    CHECK((p.previous_position - Vec2(x, y)).sqr_abs() < eps);
    CHECK((p.acceleration - Vec2(0.f, 0.f)).sqr_abs() < eps);
  }
  {
    float x = 123.f;
    float y = 456.f;
    float speed_x = 8.f;
    float speed_y = 9.f;
    const Point p(Vec2(x, y), Vec2(speed_x, speed_y));
    CHECK((p.current_position - Vec2(x, y)).sqr_abs() < eps);
    CHECK((p.previous_position - Vec2(x - speed_x, y - speed_y)).sqr_abs() <
          eps);
    CHECK((p.acceleration - Vec2(0.f, 0.f)).sqr_abs() < eps);
  }
  {
    float x = 123.f;
    float y = 567.f;
    float speed_x = 8.f;
    float speed_y = 9.f;
    float acc_x = 4.f;
    float acc_y = 6.f;
    const Point p(Vec2(x, y), Vec2(speed_x, speed_y), Vec2(acc_x, acc_y));
    CHECK((p.current_position - Vec2(x, y)).sqr_abs() < eps);
    CHECK((p.previous_position - Vec2(x - speed_x, y - speed_y)).sqr_abs() <
          eps);
    CHECK((p.acceleration - Vec2(acc_x, acc_y)).sqr_abs() < eps);
  }
}

TEST_CASE("edge constructor") {
  const float eps = 1e-4;
  CHECK(true);
}
