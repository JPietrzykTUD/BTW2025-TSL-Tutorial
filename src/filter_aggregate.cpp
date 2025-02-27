#include <iostream>
#include <numeric>
#include <algorithm>

#include "column.hpp"

#ifdef LIVE_SESSION
#include "filter_aggregate_live.hpp"
#else
#include "filter_aggregate.hpp"
#endif

int main(void) {
  using FilterColumnT = int;
  using AggregateColumT = int16_t;
  
  column_t<FilterColumnT>   to_filter(100);
  column_t<AggregateColumT> to_aggregate(100);

  /* Initialize filter data with a sequence [0, 99] */
  std::iota(to_filter.span().begin(), to_filter.span().end(), 0);
  /* Initialize aggregation data with a constant value 4 */
  std::fill(to_aggregate.span().begin(), to_aggregate.span().end(), 4);

  /* Accumulate all values from to_aggregate, if the corresponding element in to_filter is greater or equal to 50. */
  auto result = filter_aggregate<FilterColumnT,AggregateColumT>(to_aggregate.span(), to_filter.span(), (int)50);
// 
  std::cout << "Result: " << result << ". (should be: " << 50*4 << ")" << std::endl;
  return 0;
}