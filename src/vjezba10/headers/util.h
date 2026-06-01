#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <type_traits>
#pragma once

namespace eng {
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
T random_in_range(T min, T max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(gen);
  } else {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(gen);
  }
}
}  // namespace eng

#endif  // UTIL_H
