#include <glm/vec3.hpp>
#include <stdexcept>
#include <vector>

#include "Global.h"

namespace eng {
struct Raster {
  int32 width{};
  int32 height{};
  std::vector<glm::vec3> data;

  Raster() = default;

  Raster(int32 w, int32 h, glm::vec3 clear = {}) noexcept
      : width(w), height(h), data(w * h, clear) {}

  void resize(int32 w, int32 h, glm::vec3 clear = {}) noexcept {
    width = w;
    height = h;
    data.assign(static_cast<size_t>(w * h), clear);
  }

  glm::vec3 &operator()(int32 x, int32 y) { return data[(y * width) + x]; }

  const glm::vec3 &operator()(int32 x, int32 y) const { return data[(y * width) + x]; }

  glm::vec3 &at(int32 x, int32 y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
      throw std::out_of_range("Raster::at out of bounds");
    }
    return data[(y * width) + x];
  }

  [[nodiscard]] const glm::vec3 &at(int32 x, int32 y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
      throw std::out_of_range("Raster::at out of bounds");
    }
    return data[(y * width) + x];
  }

  void clear(glm::vec3 color = {}) { std::fill(data.begin(), data.end(), color); }

  float *raw() { return reinterpret_cast<float *>(data.data()); }

  [[nodiscard]] const float *raw() const {
    return reinterpret_cast<const float *>(data.data());
  }
};
}  // namespace eng
