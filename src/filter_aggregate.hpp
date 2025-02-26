#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <bit>
#include "tsl/tsl.hpp"

#include "column.hpp"
#include "simd_utils.hpp"




template <typename T, unsigned SimdPar>
auto filter_aggregate_impl(std::span<const T> to_agg, std::span<const T> to_filter, T const filter_value) -> T {
  if (to_agg.size() != to_filter.size()) {
    throw std::runtime_error("Size mismatch");
  }

  using procStyle = tsl::simd_ext<T, SimdPar>;
  std::cout << "Executing filter_aggregate_impl with " << tsl::type_name<procStyle>() << std::endl;

  /* initializing */
  auto result = tsl::set_zero<procStyle>();
  auto filter_val = tsl::set1<procStyle>(filter_value);

  
  for (auto i = 0; i < to_filter.size(); i+=SimdPar) {
    /* load filter data */
    auto const filter_data = tsl::loadu<procStyle>(&to_filter[i]);
    /* compare filter data */
    auto const valid = tsl::greater_than<procStyle>(filter_data, filter_val);
    /* load aggregation data */
    auto const aggregate = tsl::loadu<procStyle>(&to_agg[i]);
    /* add valid values to result */
    result = tsl::add<procStyle>(valid, result, aggregate);
  }
  /* sum up all partial results */
  return tsl::hadd<procStyle>(result);
}


template <typename T, unsigned SimdPar = simd_helper::max_par<T>()>
auto filter_aggregate(std::span<const T> to_agg, std::span<const T> to_filter, T const filter_value) {
  auto ld = std::bit_width(SimdPar) - 1;
  auto to_agg_simd_count = (to_agg.size() >> ld) << ld;
  auto to_filter_simd_count = (to_filter.size() >> ld) << ld;

  auto to_agg_simd    = to_agg.subspan(0, to_agg_simd_count);
  auto to_filter_simd = to_filter.subspan(0, to_filter_simd_count);

  auto to_agg_scalar  = to_agg.subspan(to_agg_simd_count);
  auto to_filter_scalar = to_filter.subspan(to_filter_simd_count);

  auto result = filter_aggregate_impl<T, SimdPar>(to_agg_simd, to_filter_simd, filter_value);
  result += filter_aggregate_impl<T, 1>(to_agg_scalar, to_filter_scalar, filter_value);
  return result;
}