#include "math/BezierBuilder.h"

namespace eng {
// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
[[nodiscard]] const std::vector<glm::vec3> &BezierBuilder::get_control_points()
    const noexcept {
  return _control_points;
}
void BezierBuilder::set_control_points(const std::vector<glm::vec3> &control_points) {
  _control_points = control_points;
}

// ----------------------------------
// METHODS
// ----------------------------------
void BezierBuilder::add_control_point(const glm::vec3 &point) {
  _control_points.push_back(point);
}
void BezierBuilder::clear_control_points() { _control_points.clear(); }

[[nodiscard]] std::vector<CurveVertex> BezierBuilder::build_control() const {
  if (_control_points.empty()) {
    return {{}};
  }

  glm::vec3 color = {1.0F, 0.0F, 0.0F};
  std::vector<CurveVertex> vertices(_control_points.size(), {glm::vec3(0.0F), color});
  for (size_t i = 0; i < _control_points.size(); ++i) {
    vertices[i].coords = _control_points[i];
  }
  return vertices;
}
[[nodiscard]] std::vector<CurveVertex> BezierBuilder::build_approximate(
    size_t num_segments) const {
  if (_control_points.empty()) {
    return {{}};
  }

  auto binomial_coefficients =
      _precalculate_binomial_coefficients(_control_points.size() - 1);
  glm::vec3 color = {1.0F, 0.0F, 1.0F};

  std::vector<CurveVertex> vertices;
  vertices.reserve(num_segments + 1);

  float delta = 1.0F / num_segments;

  for (size_t i = 0; i <= num_segments; ++i) {
    glm::vec3 point(0.0F);
    size_t n = _control_points.size() - 1;
    float t = i * delta;
    for (size_t j = 0; j <= n; ++j) {
      float coeff = binomial_coefficients[j] * std::pow(t, j) * std::pow(1 - t, n - j);
      point += coeff * _control_points[j];
    }
    vertices.emplace_back(point, color);
  }
  return vertices;
}

[[nodiscard]] std::vector<CurveVertex> BezierBuilder::build_interpolate(
    size_t num_segments) const {
  if (_control_points.size() < 4) {
    return {{}};
  }
  size_t n = _control_points.size();
  glm::vec3 p0 = _control_points[n - 4];
  glm::vec3 p1 = _control_points[n - 3];
  glm::vec3 p2 = _control_points[n - 2];
  glm::vec3 p3 = _control_points[n - 1];

  // A = M * P => A^T = P^T * M^T
  glm::mat4 M = (1.F / 18) * glm::mat4(glm::vec4(18.0F, -33.0F, 21.0F, -6.0F),
                                       glm::vec4(0.0F, 54.0F, -81.0F, 27.0F),
                                       glm::vec4(0.0F, -27.0F, 81.0F, -54.0F),
                                       glm::vec4(0.0F, 6.0F, -21.0F, 33.0F));

  glm::mat4 P = glm::mat4(glm::vec4(p0, 0.0F), glm::vec4(p1, 0.0F), glm::vec4(p2, 0.0F),
                          glm::vec4(p3, 0.0F));

  glm::mat4 A = P * glm::transpose(M);
  auto a0 = glm::vec3(A[0]);
  auto a1 = glm::vec3(A[1]);
  auto a2 = glm::vec3(A[2]);
  auto a3 = glm::vec3(A[3]);

  glm::vec3 color = {0.0F, 0.0F, 1.0F};

  std::vector<CurveVertex> vertices;
  vertices.reserve(num_segments + 1);

  float delta = 1.0F / num_segments;

  for (size_t i = 0; i <= num_segments; ++i) {
    float t = i * delta;

    glm::vec3 point = (a0) +
                      (((3 * t) - (3 * std::pow(t, 2.F)) + (std::pow(t, 3.F))) * a1) +
                      (((3 * std::pow(t, 2.F)) - (2 * std::pow(t, 3.F))) * a2) +
                      (std::pow(t, 3.F) * a3);

    vertices.emplace_back(point, color);
  }

  return vertices;
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
std::vector<uint64> BezierBuilder::_precalculate_binomial_coefficients(uint64 n) {
  std::vector<uint64> binomial_coefficients(n + 1, 1);
  for (uint64 i = 1; i <= n; ++i) {
    binomial_coefficients[i] = binomial_coefficients[i - 1] * (n - i + 1) / i;
  }
  return binomial_coefficients;
}

}  // namespace eng
