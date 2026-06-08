#ifndef MODEL_H
#define MODEL_H

#pragma once
#include <memory>

#include "render/Material.h"
#include "render/Mesh.h"

namespace eng {
struct Model {
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::vector<std::shared_ptr<Material>> materials;
};
}  // namespace eng
#endif
