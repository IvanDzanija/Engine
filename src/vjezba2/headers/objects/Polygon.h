#pragma once
#include "Bresenham.h"
#include "Edge.h"
#include "Global.h"
#include "Graphics.h"
#include "Point.h"

namespace eng {
struct PolyElement {
  Point2D point;
  Edge2D edge;
  bool left = false;

  PolyElement() = default;
  PolyElement(const Point2D &point, const Edge2D &edge) : point(point), edge(edge) {}
  PolyElement(const Point2D &point, const Edge2D &edge, bool left)
      : point(point), edge(edge), left(left) {}
};
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

  /* Adds a vertex to the polygon. If it's the first vertex, it initializes the
   * polygon. For subsequent vertices, it creates an edge from the last vertex to
   * the new vertex and determines if the edge is a left edge based on the
   * y-coordinates of the vertices. Note: Every new addition has side effects on
   * previous vertex (i.e. the last vertex's left status is updated).
   */
  void add_vertex(const Point2D &point) {
    if (_elements.empty()) {
      _elements.emplace_back(point, Edge2D(), false);
    } else {
      auto &last = _elements.back();
      _elements.emplace_back(point, Edge2D(last.point, point));
      // Side effect on last vertex
      bool last_left = last.point.y < point.y;
      last.left = last_left;
    }
  }

  /** Draws the outline of the polygon by connecting the vertices with lines.
   * Note: Does not fill the polygon, only draws the edges.
   */
  void draw_outline(eng::Graphics &screen) const {
    for (size_t i = 0; i < _elements.size(); ++i) {
      const auto &curr = _elements[i];
      const auto &next = _elements[(i + 1) % _elements.size()];
      eng::draw_line(screen, curr.point.x, curr.point.y, next.point.x, next.point.y);
    }
  }

  /* Checks if adding a new vertex would close the polygon by comparing the new
   * vertex with the first vertex. If there are less than 3 vertices, it cannot
   * form a polygon, so it returns false. Otherwise, it checks if the new vertex
   * is near the first vertex using the is_near function from the Point module.
   */
  [[nodiscard]] bool would_close(const Point2D &point, int32 radius = 1) const {
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
   */
  [[nodiscard]] bool is_closed(int32 radius = 1) {
    // Should probably pop back if closed for better UX, but this is fine for now
    if (_closed) {
      return true;
    }
    if (_elements.size() < 3) {
      return false;
    }
    const auto &first = _elements.front();
    const auto &last = _elements.back();
    _closed = eng::is_near(first.point, last.point, radius);
    return _closed;
  }

  void draw_filled(eng::Graphics &screen) const { return; }
  [[nodiscard]] bool test_convex() const { return true; }
  [[nodiscard]] bool test_point(const Point2D &point) const { return false; }

 private:
  std::vector<PolyElement> _elements;
  bool _closed = false;
};
}  // namespace eng
