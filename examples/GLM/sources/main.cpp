// Local Headers

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "glm/ext.hpp"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// static constexpr int mWidth = 1280;
// static constexpr int mHeight = 800;

int main(int argc, char *argv[]) {
  glm::vec3 v1 = glm::vec3(2, 3, -4) + glm::vec3(-1, 4, -3);
  std::cout << "v1= " << glm::to_string(v1) << std::endl;
  std::cout << "ex= " << "vec3(1, 7, -7)" << std::endl;

  glm::vec3 s = v1 * glm::vec3(-1, 4, -3);
  std::cout << "s= " << glm::to_string(s) << std::endl;
  std::cout << "ex= " << "vec3(-1, 28, 21)" << std::endl;

  glm::vec3 v2 = glm::cross(v1, glm::vec3(2, 2, 4));
  std::cout << "v2= " << glm::to_string(v2) << std::endl;
  std::cout << "ex= " << "vec3(42, -18, -12)" << std::endl;

  glm::vec3 v3 = glm::normalize(v2);
  std::cout << "v3= " << glm::to_string(v3) << std::endl;

  glm::vec3 v4 = -v2;
  std::cout << "v4= " << glm::to_string(v4) << std::endl;
  std::cout << "ex= " << "vec3(-42, 18, 12)" << std::endl;

  glm::mat3 m1 =
      glm::mat3(1, 2, 3, 2, 1, 3, 4, 5, 1) + glm::mat3(-1, 2, -3, 5, -2, 7, -4, -1, 3);
  std::cout << "m1= " << glm::to_string(m1) << std::endl;
  std::cout << "ex= " << "mat3(0, 4, 0, 7, -1, 10, 0, 4, 4)" << std::endl;

  glm::mat3 m2 = glm::mat3(1, 2, 3, 2, 1, 3, 4, 5, 1) *
                 glm::transpose(glm::mat3(-1, 2, -3, 5, -2, 7, -4, -1, 3));
  std::cout << "m2= " << glm::to_string(m2) << std::endl;

  glm::mat3 m3 = glm::inverse(glm::mat3(-24, 18, 5, 20, -15, -4, -5, 4, 1)) *
                 glm::inverse(glm::mat3(1, 2, 3, 0, 1, 4, 5, 6, 0));
  std::cout << "m3= " << glm::to_string(m3) << std::endl;

  // Shared buffers for next 2 tasks
  glm::mat3 A;
  glm::vec3 b;
  glm::vec3 x;
  // Solving linear equations Ax=b
  std::cout << "Load parameters of the equation Ax=b:" << std::endl;
  std::cout << "A:" << std::endl;
  for (short i = 0; i < 3; ++i) {
    for (short j = 0; j < 3; ++j) {
      std::cin >> A[j][i];
    }
  }
  std::cout << "b:" << std::endl;
  for (short i = 0; i < 3; ++i) {
    std::cin >> b[i];
  }

  x = glm::inverse(A) * b;
  std::cout << "Solution: " << glm::to_string(x) << std::endl;
  assert(A * x == b);

  // Baricentric coordinates
  std::cout << "Load points of the triangle:" << std::endl;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cin >> A[i][j];
    }
  }
  std::cout << glm::to_string(A) << std::endl;
  std::cout << "Point P:" << std::endl;
  for (int i = 0; i < 3; i++) {
    std::cin >> b[i];
  }
  x = glm::inverse(A) * b;
  std::cout << "Baricentric coordinates of P: " << glm::to_string(x) << std::endl;

  return EXIT_SUCCESS;
}
