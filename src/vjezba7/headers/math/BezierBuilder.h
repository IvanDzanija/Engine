#ifndef BEZIER_BUILDER_H
#define BEZIER_BUILDER_H

#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Global.h"
#include "render/CurveVertex.h"

namespace eng {
class BezierBuilder {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  BezierBuilder() = default;
  ~BezierBuilder() = default;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] const std::vector<glm::vec3> &get_control_points() const noexcept;
  void set_control_points(const std::vector<glm::vec3> &control_points);

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void add_control_point(const glm::vec3 &point);
  void clear_control_points();

  [[nodiscard]] std::vector<CurveVertex> build_control() const;
  [[nodiscard]] std::vector<CurveVertex> build_approximate(
      size_t num_segments = 100) const;
  [[nodiscard]] std::vector<CurveVertex> build_interpolate(
      size_t num_segments = 100) const;

 private:
  std::vector<glm::vec3> _control_points;

  // ----------------------------------
  // PRIVATE METHODS
  // ----------------------------------
  static std::vector<uint64> _precalculate_binomial_coefficients(uint64 n);
};
}  // namespace eng

#endif  // BEZIER_BUILDER_H
