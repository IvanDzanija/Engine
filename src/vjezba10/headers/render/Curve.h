#ifndef CURVE_H
#define CURVE_H

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "CurveVertex.h"
#include "Renderable.h"

namespace eng {
class Curve : public Renderable {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  // Copy constructor
  explicit Curve(const std::vector<CurveVertex> &vertices);

  // Move constructor
  explicit Curve(std::vector<CurveVertex> &&vertices);

  // Rule of 5
  Curve(const Curve &other) = delete;
  Curve &operator=(const Curve &other) = delete;
  Curve(Curve &&other) noexcept;
  Curve &operator=(Curve &&other) noexcept;
  ~Curve() override;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] const std::vector<CurveVertex> &get_vertices() const noexcept;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void draw(const std::shared_ptr<Shader> &shader) const override;
  [[nodiscard]] RenderableType get_type() const override;
  void update_vertices(std::vector<CurveVertex> &&vertices);

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  std::vector<CurveVertex> _vertices;
  GLuint _vbo = 0;

  // ----------------------------------
  // PRIVATE METHODS
  // ----------------------------------
  void _setup_curve();
};
}  // namespace eng

#endif  // CURVE_H
