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
auto filter_aggregate_impl(std::span<const AggregateColumnT> to_agg, std::span<const FilterColumnT> to_filter, FilterColumnT const filter_value) -> AggregateColumnT
{
  if (to_agg.size() != to_filter.size())
  {
    throw std::runtime_error("Size mismatch");
  }

  using FilterProcStyle = tsl::simd_ext<FilterColumnT, SimdPar>;
  using AggProcStyle = tsl::simd_ext<AggregateColumnT, SimdPar>;
  std::cout << "Executing filter_aggregate_impl (Filter) with " << tsl::type_name<FilterProcStyle>() << std::endl;
  std::cout << "Executing filter_aggregate_impl (Aggregate) with " << tsl::type_name<AggProcStyle>() << std::endl;

  /* initializing */
  auto result = tsl::set_zero<AggProcStyle>();
  auto filter_val = tsl::set1<FilterProcStyle>(filter_value);

  for (auto i = 0; i < to_filter.size(); i += SimdPar)
  {
    /* load filter data */
    auto const filter_data = tsl::loadu<FilterProcStyle>(&to_filter[i]);
    /* compare filter data */
    auto const valid = tsl::greater_than_or_equal<FilterProcStyle>(filter_data, filter_val);
    /* load aggregation data */
    auto const aggregate = tsl::loadu<AggProcStyle>(&to_agg[i]);

    /* add valid values to result */
    if constexpr(
      (sizeof(FilterColumnT) == sizeof(AggregateColumnT)) 
      ||
      (std::is_same_v<decltype(valid),typename FilterProcStyle::imask_type>)
    ) {
      result = tsl::add<AggProcStyle>(valid, result, aggregate);
    } else {
      result = tsl::add<AggProcStyle>(tsl::to_integral<FilterProcStyle>(valid), result, aggregate);
    }
    
  }
  /* sum up all partial results */
  return tsl::hadd<AggProcStyle>(result);
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