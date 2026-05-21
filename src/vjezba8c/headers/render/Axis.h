#ifndef AXIS_H
#define AXIS_H

#pragma once
#include "Global.h"
#include "render/Renderable.h"

namespace eng {
class Axis : public Renderable {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  Axis();
  ~Axis() override;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void draw() const override;
  [[nodiscard]] RenderableType get_type() const override;

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  uint32 _vbo;

  // clang-format off
  std::array<float, 3 * 12> _data = {
      // Axis start     Axis color        Axis end          Axis color
      0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,
      0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F,
      0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F};
  // clang-format on
};
}  // namespace eng

#endif
