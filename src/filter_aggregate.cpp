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
  using AggregateColumT = uint8_t;
  auto const N = 129;
  
  column_t<FilterColumnT>   to_filter(N);
  column_t<AggregateColumT> to_aggregate(N);

  /* Initialize filter data with a sequence [0, N-1] */
  std::iota(to_filter.span().begin(), to_filter.span().end(), 0);
  /* Initialize aggregation data with a constant value 4 */
  std::fill(to_aggregate.span().begin(), to_aggregate.span().end(), 4);

  /* Accumulate all values from to_aggregate, if the corresponding element in to_filter is greater or equal to 50. */
  auto result = filter_aggregate<FilterColumnT,AggregateColumT>(to_aggregate.span(), to_filter.span(), (int)50);

  AggregateColumT reference_result = 0;
  for (size_t i = 0; i < N; ++i) {
    if (to_filter[i] >= 50) {
      reference_result += to_aggregate[i];
    }
  }
  std::cout << "Result: " << +result << ". (should be: " << +reference_result << ")" << std::endl;
  return 0;
}