#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <bit>
#include <utility>
#include "tsl/tsl.hpp"

#include "column.hpp"
#include "simd_utils.hpp"

template <unsigned SimdPar, typename FilterColumnT, typename AggregateColumnT = FilterColumnT>
auto filter_aggregate_impl(
  std::span<const AggregateColumnT> to_agg, 
  std::span<const FilterColumnT> to_filter, 
  FilterColumnT const filter_value
) -> AggregateColumnT {
  if (to_agg.size() != to_filter.size()) {
    throw std::runtime_error("Size mismatch");
  }

  /* initializing */


  for (auto i = 0; i < to_filter.size(); i += SimdPar) {
    /* load filter data */
    
    /* compare filter data --> return type is a mask (NEON/SSE/AVX: register, AVX512/SVE: integral value)*/
    
    /* load aggregation data */
    
    /* add valid values to result */

    
  }
  /* sum up all partial results */
  return 0;
}

template <typename FilterColumnT, typename AggregateColumnT, unsigned SimdPar = simd_helper::max_par<FilterColumnT>()>
auto filter_aggregate(std::span<const AggregateColumnT> to_agg, std::span<const FilterColumnT> to_filter, FilterColumnT const filter_value)
{
  /* calculate the number of elements that can be processed in parallel */
  auto to_filter_simd_count = (to_filter.size() / SimdPar) * SimdPar;
  auto to_agg_simd_count = (to_agg.size() / SimdPar) * SimdPar;

  /* create std::spans for simd and scalar processing */
  auto to_filter_simd = to_filter.subspan(0, to_filter_simd_count);
  auto to_agg_simd = to_agg.subspan(0, to_agg_simd_count);

  auto to_filter_scalar = to_filter.subspan(to_filter_simd_count);
  auto to_agg_scalar = to_agg.subspan(to_agg_simd_count);

  /* execute filter aggregation */
  auto result = filter_aggregate_impl<SimdPar, FilterColumnT, AggregateColumnT>(to_agg_simd, to_filter_simd, filter_value);
  result += filter_aggregate_impl<1, FilterColumnT, AggregateColumnT>(to_agg_scalar, to_filter_scalar, filter_value);
  return result;
}