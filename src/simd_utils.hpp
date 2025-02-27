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

  template <typename T>
  static consteval auto available_parallelism() {
    return executor::template available_parallelism<T>();
  } 
};

using simd_helper = simd_helper_t<>;

template <typename T>
consteval auto available_parallelism() {
  return simd_helper::template available_parallelism<T>();
}

namespace tsl {
  template <typename T, int SimdPar>
  struct simd_ext_by_parallelism {
    using type = tsl::simd<T, typename simd_helper::executor::template simd_ext_t<T, SimdPar>>;
  };
  template <typename T>
  struct simd_ext_by_parallelism<T, 1> {
    using type = tsl::simd<T, tsl::scalar>;
  };
  template <typename T, int SimdPar>
  using simd_ext = typename simd_ext_by_parallelism<T, SimdPar>::type;


  
}
