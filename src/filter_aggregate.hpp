#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "tsl/tsl.hpp"

template<typename T>
using column_t = std::unique_ptr<T[]>;

template <typename FilterT, typename AggT>
auto filter_aggregate(column_t<AggT> const & to_agg, column_t<FilterT> const & to_filter, FilterT const filter_value ) -> AggT {
    
}