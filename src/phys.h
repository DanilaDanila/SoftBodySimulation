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

  Point *const first;
  Point *const second;

  float length;
  float rigidity;

  Edge(Point *const first, Point *const second,
       i_promise_do_not_use_as_edge /**/)
      : first(first), second(second), length(-1.f), rigidity(-1.f) {}

  Edge(Point *const first, Point *const second)
      : first(first), second(second),
        length((first->current_position - second->current_position).abs()),
        rigidity(1.) {}

  Edge(Point *const first, Point *const second, float rigidity)
      : first(first), second(second),
        length((first->current_position - second->current_position).abs()),
        rigidity(rigidity) {}

  Edge(Point *const first, Point *const second, float length, float rigidity)
      : first(first), second(second), length(length), rigidity(rigidity) {}
};

struct Body {
  int points_count;
  Point *points;

  int edges_count;
  Edge *edges;

  BodyType type;

  Body() = default;
  Body(const Body &) = default;
  Body(Body &&) = default;

  ~Body() {
    // TODO: don't forget to remove it нахрен
    // schreder<Point>(points);
    // schreder<Edge>(edges);
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
/*
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
*/

bool isPointsOnSameSideOfLine(Vec2 p0, Vec2 p1, Vec2 line_beg, Vec2 line_end) {
  Vec2 n0 = normal(p0, line_beg, line_end);
  Vec2 n1 = normal(p1, line_beg, line_end);

  return n0 * n1 > 0;
}

bool isPointInsideBody(const Point *const p0, const Body *const body) {
  Vec2 p = p0->current_position;
  const Body *const b = body;
  Vec2 b_center;
  for (int i = 0; i < b->points_count; i++)
    b_center = b_center + b->points[i].current_position;
  b_center = b_center / b->points_count;

  bool check = true;

  for (int i = 0; i < b->points_count - 1; i++)
    check =
        check &&
        (isPointsOnSameSideOfLine(p, b_center, b->points[i].current_position,
                                  b->points[i + 1].current_position));
  check = check && (isPointsOnSameSideOfLine(
                       p, b_center, b->points[0].current_position,
                       b->points[b->points_count - 1].current_position));

  return check;
}

class BodyCreator {
public:
  void addPoint(Vec2 point) { points.push_back(Point(point)); }

  void addEdge(int point0_id, int point1_id) {
    edges.push_back(std::make_pair(point0_id, point1_id));
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

class Sandbox {
public:
  Sandbox() {}

  ~Sandbox() {}

  void setAcceleration(const Vec2 &acc) { acceleration = acc; }

  void addDynamicBody(Body &&body) {
    dynamic_bodys.push_back(body);

    Body &last = *dynamic_bodys.rbegin();
    for (int i = 0; i < last.points_count; ++i) {
      last.points[i].acceleration = acceleration;
    }
  }

  void addStaticBody(Body &&body) { static_bodys.push_back(body); }

  void update(float dtime) {
    for (Body &body : dynamic_bodys) {
      updateBody(body, dtime);
    }

    for (Body &body : dynamic_bodys) {
      updateEdges(body, dtime);
    }

    for (Body &body : dynamic_bodys) {
      updateCollisions(body, dtime);
    }
  }

  const std::vector<Body> getStaticBodys() const { return static_bodys; }

  const std::vector<Body> getDynamicBodys() const { return dynamic_bodys; }

private:
  const Vec2 getNormalToBody(const Point &point, const Body &body) {
    Vec2 norm = normal(point.current_position, body.points[0].current_position,
                       body.points[1].current_position);

    for (int i = 1; i < body.points_count; ++i) {
      int j = (i + 1) % body.points_count;

      Vec2 candidate =
          normal(point.current_position, body.points[i].current_position,
                 body.points[j].current_position);

      if (candidate.sqr_abs() < norm.sqr_abs()) {
        norm = candidate;
      }
    }

    return norm;
  }

  void updateBody(Body &body, float dtime) {
    for (int i = 0; i < body.points_count; ++i) {
      Point &p = body.points[i];

      // TODO: fix time error
      const Vec2 new_pos =
          p.current_position +
          (p.current_position - p.previous_position) + // should be 'V * dtime'
          p.acceleration * dtime * dtime;
      p.previous_position = p.current_position;
      p.current_position = new_pos;
    }
  }

  void updateEdges(Body &body, float dtime) {
    // TODO: update time error
    for (int i = 0; i < body.edges_count; ++i) {
      Edge &edge = body.edges[i];

      // TODO: fix prev_position error
      const Vec2 edge_center =
          .5 * (edge.first->current_position + edge.second->current_position);

      const Vec2 edge_cener_to_first =
          edge.first->current_position - edge_center;
      const Vec2 edge_cener_to_second =
          edge.second->current_position - edge_center;

      edge.first->current_position =
          edge_center +
          edge_cener_to_first.normalize() * edge.length * .5 * edge.rigidity;
      edge.second->current_position =
          edge_center +
          edge_cener_to_second.normalize() * edge.length * .5 * edge.rigidity;
    }
  }

  void updateCollisions(Body &body, float dtime) {
    for (int i = 0; i < body.points_count; ++i) {
      for (const Body &static_body : static_bodys) {
        if (not isPointInsideBody(&body.points[i], &static_body)) {
          continue;
        }

        const Vec2 curr_shift =
            getNormalToBody(body.points[i].current_position, static_body);
        const Vec2 prev_shift =
            getNormalToBody(body.points[i].previous_position, static_body);

        // TODO: fix previous positoin error
        body.points[i].current_position += 2.f * curr_shift;
        body.points[i].previous_position += 2.f * prev_shift;
      }
    }
  }

  Vec2 acceleration;

  std::vector<Body> static_bodys;
  std::vector<Body> dynamic_bodys;
};

#endif // PHYS_H
