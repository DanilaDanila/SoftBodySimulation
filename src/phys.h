#ifndef PHYS_H
#define PHYS_H

#include "vec2.h"
#include <utility>
#include <vector>

template <class T> void schreder(T *ptr) { delete[] ptr; }

typedef enum { BODY_TYPE_STATIC, BODY_TYPE_DYNAMIC } BodyType;

// Point Particle
struct Point {
  Vec2 current_position;
  Vec2 previous_position;

  Vec2 acceleration;

  Point(Vec2 pos)
      : current_position(pos), previous_position(pos),
        acceleration(Vec2(0.f, 0.f)) {}

  Point(Vec2 pos, Vec2 speed)
      : current_position(pos), previous_position(pos - speed),
        acceleration(Vec2(0.f, 0.f)) {}

  Point(Vec2 pos, Vec2 speed, Vec2 acc)
      : current_position(pos), previous_position(pos - speed),
        acceleration(acc) {}

  Point(const Point &) = default;

  Point(Point &&) = default;
};

struct Edge {
  struct i_promise_do_not_use_as_edge {};

  const Point *const first;
  const Point *const second;

  float length;
  float rigidity;

  Edge(const Point *const first, const Point *const second,
       i_promise_do_not_use_as_edge /**/)
      : first(first), second(second), length(-1.f), rigidity(-1.f) {}

  Edge(const Point *const first, const Point *const second)
      : first(first), second(second),
        length((first->current_position - second->current_position).abs()),
        rigidity(1.) {}

  Edge(const Point *const first, const Point *const second, float rigidity)
      : first(first), second(second),
        length((first->current_position - second->current_position).abs()),
        rigidity(rigidity) {}

  Edge(const Point *const first, const Point *const second, float length,
       float rigidity)
      : first(first), second(second), length(length), rigidity(rigidity) {}
};

struct Body {
  int points_count;
  Point *points;

  int edges_count;
  Edge *edges;

  BodyType type;

  ~Body() {
    schreder<Point>(points);
    schreder<Edge>(edges);
  }
};

bool isPointsOnSameSide(const Point *const p0, const Point *const p1,
                        const Edge *const line) {
  const Vec2 l0 = line->first->current_position;
  const Vec2 l1 = line->second->current_position;

  const Vec2 magic0 = (l0 - p0->current_position) + (l1 - p0->current_position);
  const Vec2 magic1 = (l0 - p1->current_position) + (l1 - p1->current_position);
  return magic0 * magic1 > 0;
}

// !!! STATIC BODY ONLY !!!
bool isPointInsideBody(const Point *const p0, const Body *const body) {
  Vec2 center = Vec2(0.f, 0.f);
  for (int i = 0; i < body->points_count; ++i) {
    center += body->points[i].current_position;
  }
  center /= body->points_count;
  const Point centerPoint(center);

  for (int i = 0; i < body->points_count; ++i) {
    for (int j = i + 1; j != i; j = (j + 1) % body->points_count) {
      const Edge edge(&body->points[i], &body->points[j]);
      if (not isPointsOnSameSide(&centerPoint, p0, &edge)) {
        return false;
      }
    }
  }
  return true;
}

class BodyCreator {
public:
  void addPoint(Vec2 point) { points.emplace_back(Point(point)); }

  void addEdge(int point0_id, int point1_id) {
    edges.emplace_back(std::make_pair(point0_id, point1_id));
  }

  void clear() {
    points.clear();
    edges.clear();
  }

  Body createBody() {
    Body body;

    body.points_count = points.size();
    body.points = (Point *)new char[sizeof(Point) * body.points_count];
    for (int i = 0; i < body.points_count; ++i)
      new (&body.points[i]) Point(points[i]);

    body.edges_count = edges.size();
    body.edges = (Edge *)new char[sizeof(Edge) * body.edges_count];
    for (int i = 0; i < body.edges_count; ++i)
      new (&body.edges[i])
          Edge(&body.points[edges[i].first], &body.points[edges[i].second]);

    return body;
  }

private:
  std::vector<Point> points;
  std::vector<std::pair<int, int>> edges;

  float rigidity = 1.f;
};

#endif // PHYS_H
