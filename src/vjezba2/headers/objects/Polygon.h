#pragma once

#include "Bresenham.h"
#include "Edge.h"
#include "Global.h"
#include "Graphics.h"
#include "Point.h"
#include "Util.h"
#include "glm/ext/vector_float3.hpp"

namespace eng {
/**
 * PolyElement stores an edge of the polygon and the the starting point of the edge.
 * The left field indicates if the edge is a left edge
 */
struct PolyElement {
  Point2D point;  //
  Edge2D edge;
  bool left = false;

  PolyElement() = default;
  PolyElement(const Point2D &point, const Edge2D &edge) : point(point), edge(edge) {}
  PolyElement(const Point2D &point, const Edge2D &edge, bool left)
      : point(point), edge(edge), left(left) {}
};

enum class PolyOrientation : uint8 { CLOCKWISE, COUNTERCLOCKWISE, NONE };
// enum class PolyConvexity : uint8 { CONVEX, CONCAVE, NONE };

class Polygon {
 public:
  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] size_t vertex_count() const { return _elements.size(); }
  [[nodiscard]] const std::vector<PolyElement> &elements() const { return _elements; }
  [[nodiscard]] const PolyElement &operator[](size_t index) const {
    return _elements[index];
  }
  [[nodiscard]] PolyElement &operator[](size_t index) { return _elements[index]; }
  [[nodiscard]] const PolyElement &front() const { return _elements.front(); }
  [[nodiscard]] PolyElement &front() { return _elements.front(); }
  [[nodiscard]] const PolyElement &back() const { return _elements.back(); }
  [[nodiscard]] PolyElement &back() { return _elements.back(); }

  /* Adds a vertex to the polygon. If it's the first vertex, it initializes the
   * polygon. For subsequent vertices, it creates an edge from the last vertex to
   * the new vertex and determines if the edge is a left edge based on the
   * y-coordinates of the vertices. Note: Every new addition has side effects on
   * previous vertex (i.e. the last vertex's left status is updated).
   */
  void add_vertex(const Point2D &point) {
    if (!_elements.empty()) {
      auto &last = _elements.back();
      // Side effect on last vertex
      last.edge = Edge2D(last.point, point);
      bool last_left = last.point.y < point.y;
      last.left = last_left;
    }
    _elements.emplace_back(point, Edge2D(), false);
    std::println("Last vertex: {}, {}, {}", _elements.back().edge.a,
                 _elements.back().edge.b, _elements.back().edge.c);
  }

  // ----------------------------------
  // DRAWING METHODS
  // ----------------------------------

  /** Draws the outline of the polygon by connecting the vertices with lines.
   * Note: Does not fill the polygon, only draws the edges.
   */
  void draw_outline(eng::Graphics &screen) const {
    for (size_t i = 0; i < _elements.size(); ++i) {
      const auto &curr = _elements[i];
      const auto &next = _elements[(i + 1) % _elements.size()];
      eng::color_line(screen, curr.point.x, curr.point.y, next.point.x, next.point.y,
                      glm::vec3(0, 0, 1));
    }
  }

  void draw_filled(eng::Graphics &screen) {
    if (_elements.empty()) {
      // std::cerr << "Polygon has no vertices to draw." << std::endl;
      return;  // No vertices to draw
    }

    int32 x_min = _elements[0].point.x;
    int32 x_max = _elements[0].point.x;
    int32 y_min = _elements[0].point.y;
    int32 y_max = _elements[0].point.y;
    double L;
    double R;

    for (const auto &element : _elements) {
      x_min = std::min(x_min, element.point.x);
      x_max = std::max(x_max, element.point.x);
      y_min = std::min(y_min, element.point.y);
      y_max = std::max(y_max, element.point.y);
    }

    for (int32 y = y_min; y <= y_max; ++y) {
      L = static_cast<double>(x_max);
      R = static_cast<double>(x_min);
      for (const auto &curr : _elements) {
        //        std::cout << curr.point.x << " " << curr.point.y << " " << curr.edge.a
        //        << " "
        //                  << curr.edge.b << " " << curr.edge.c << std::endl;
        if (curr.edge.a == 0) {
          continue;  // Skip horizontal edges
        }
        double x = (-curr.edge.b * y - curr.edge.c) / static_cast<double>(curr.edge.a);
        if (test_orientation() == PolyOrientation::CLOCKWISE) {
          if (curr.left) {
            L = std::min(L, x);
          } else {
            R = std::max(R, x);
          }
        } else {
          if (curr.left) {
            R = std::max(R, x);
          } else {
            L = std::min(L, x);
          }
        }
      }
      eng::draw_line(screen, static_cast<int32>(L + 0.5F), y,
                     static_cast<int32>(std::round(R + 0.5F)), y);
    }
    // Draw starting vertex in red and outline in blue for clarity
    screen.shade_fragment(_elements[0].point.x, _elements[0].point.y,
                          glm::vec3(1, 0, 0));
    draw_outline(screen);
  }

  // ----------------------------------
  // TESTERS AND CHECKERS
  // ----------------------------------

  /* Checks if adding a new vertex would close the polygon by comparing the new
   * vertex with the first vertex. If there are less than 3 vertices, it cannot
   * form a polygon, so it returns false. Otherwise, it checks if the new vertex
   * is near the first vertex using the is_near function from the Point module.
   */
  [[nodiscard]] bool would_close(const Point2D &point, int32 radius = 1) const {
    if (_closed) {
      return false;
    }
    if (_elements.size() < 3) {
      return false;
    }
    const auto &first = _elements.front();
    return eng::is_near(first.point, point, radius);
  }

  /** Checks if the polygon should be closed by comparing the first and last vertices.
   * If there are less than 3 vertices, it cannot form a polygon, so it returns false.
   * Otherwise, it checks if the first and last vertices are near each other using
   * the is_near function from the Point module.
   * Note: This method has a side effect of setting the edge of the last vertex to
   * connect it to the first vertex if the polygon is closed.
   */
  [[nodiscard]] bool is_closed(int32 radius = 1) {
    if (_closed) {
      return true;
    }
    if (_elements.size() < 3) {
      return false;
    }
    _closed = eng::is_near(_elements.front().point, _elements.back().point, radius);
    if (_closed) {
      _elements.pop_back();  // Remove the last vertex as it is near the first vertex
      // Fix the last edge to connect to the first vertex
      auto &last = _elements.back();
      last.edge = Edge2D(last.point, _elements.front().point);
      last.left = last.point.y < _elements.front().point.y;
    }
    return _closed;
  }

  [[nodiscard]] PolyOrientation test_orientation() {
    if (_orientation != PolyOrientation::NONE) {
      return _orientation;
    }
    if (_elements.size() < 3) {
      return PolyOrientation::NONE;  // Not enough vertices to determine orientation
    }

    // Calculate the signed area using the shoelace formula
    // https://en.wikipedia.org/wiki/Shoelace_formula#Manipulations_of_a_polygon
    // Why is shoelace so goated?
    double area = 0.0;
    for (size_t i = 0; i < _elements.size(); ++i) {
      const auto &curr = _elements[i].point;
      const auto &next = _elements[(i + 1) % _elements.size()].point;
      area += (curr.y + next.y) * (next.x - curr.x);
    }
    // Note: y-axis is inverted so the sign of the area is reversed
    if (area < 0) {
      _orientation = PolyOrientation::CLOCKWISE;
    } else if (area > 0) {
      _orientation = PolyOrientation::COUNTERCLOCKWISE;
    } else {
      _orientation = PolyOrientation::NONE;
    }
    return _orientation;
  }

  [[nodiscard]] std::optional<bool> test_convex() const {
    if (_elements.size() < 3) {
      return std::nullopt;  // Not enough vertices to determine convexity
    }
    uint8 negative = 0;
    uint8 positive = 0;
    for (size_t i = 0; i < _elements.size(); ++i) {
      const auto &curr_edge =
          _elements[(i + _elements.size() - 1) % _elements.size()].edge;
      const auto &next_point = _elements[(i + 1) % _elements.size()].point;
      double dot =
          (curr_edge.a * next_point.x) + (curr_edge.b * next_point.y) + curr_edge.c;
      if (dot < 0) {
        ++negative;
      }
      if (dot > 0) {
        ++positive;
      }
    }
    return (negative == 0 || positive == 0);
  }

  [[nodiscard]] bool test_point(const Point2D &point, eng::Graphics &screen) {
    if (!is_closed()) {
      throw std::runtime_error("Cannot test point containment on an open polygon.");
    }
    bool inside = false;
    // We use raycasting algorithm and choose y = point.y as the ray.
    for (size_t i = 0; i < _elements.size(); ++i) {
      const auto &curr = _elements[i];
      const auto &next = _elements[(i + 1) % _elements.size()];

      const auto &curr_point = curr.point;
      const auto &end_point = next.point;
      const auto &edge = curr.edge;

      // We say that the point does not belong inside the polygon if its on the edge
      if (eng::point_on_segment(point, curr_point, end_point)) {
        return false;
      }
      // We choose to shoot the ray to the right of the point at y = point.y
      if ((curr_point.y > point.y) != (end_point.y > point.y)) {
        // a * x = - b * y - c
        double x_intersect =
            (-(edge.b * point.y) - edge.c) / static_cast<double>(edge.a);

        if (x_intersect > point.x) {
          inside = !inside;
          eng::color_line(screen, curr_point.x, curr_point.y, end_point.x, end_point.y,
                          glm::vec3(0, 0.0, 1));
        }
      }
    }
    return inside;
  }

  [[nodiscard]] double calculate_angle() {
    if (_elements.size() < 3) {
      return 0.0;
    }
    const auto &last = _elements[_elements.size() - 1];
    const auto &second_last = _elements[_elements.size() - 2];
    const auto &third_last = _elements[_elements.size() - 3];

    glm::vec2 v2 = glm::vec2(last.point.x - second_last.point.x,
                             last.point.y - second_last.point.y);
    glm::vec2 v1 = glm::vec2(second_last.point.x - third_last.point.x,
                             second_last.point.y - third_last.point.y);
    double dot_product = glm::dot(v1, v2);
    double norm_v1 = glm::length(v1);
    double norm_v2 = glm::length(v2);
    if (norm_v1 == 0 || norm_v2 == 0) {
      return 0.0;
    }
    double cos_alpha = dot_product / (norm_v1 * norm_v2);
    return glm::degrees(std::acos(cos_alpha));
  }

 private:
  std::vector<PolyElement> _elements;
  bool _closed = false;
  PolyOrientation _orientation = PolyOrientation::NONE;
};
}  // namespace eng
