#pragma once

#include "tsl/tsl.hpp"

template <class _Target = tsl::runtime::cpu>
struct simd_helper_t {
  using executor = tsl::executor<_Target>;
  using target = _Target;
  
  template <typename T>
  static consteval auto max_par() {
    return tsl::simd<T, typename target::max_width_extension_t>::vector_element_count();
  }

  template <typename T>
  static consteval auto min_par() {
    return tsl::simd<T, typename target::min_width_extension_t>::vector_element_count();
  }
};

using simd_helper = simd_helper_t<>;