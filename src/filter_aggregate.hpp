#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>

#include "column.hpp"
#include "tsl/tsl.hpp"


template <typename T, class >
auto filter_aggregate(std::span<const T> to_agg, std::span<const T> to_filter, T const filter_value ) -> T {
  if (to_agg.size() != to_filter.size()) {
    throw std::runtime_error("Size mismatch");
  }
  for (auto i = 0; i < to_filter.size(); i+=)
  for (for (size_t i = 0; i < span.size(); ++i) {)
}