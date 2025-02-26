#include <iostream>
#include <numeric>
#include <algorithm>

#include "column.hpp"
#include "filter_aggregate.hpp"


int main(void) {
  column_t<int> to_filter(100);
  column_t<int> to_aggregate(100);

  std::iota(to_filter.span().begin(), to_filter.span().end(), 0);
  std::fill(to_aggregate.span().begin(), to_aggregate.span().end(), 4);

  auto result = filter_aggregate<int>(to_aggregate.span(), to_filter.span(), (int)50);

  std::cout << "Result: " << result << ". (should be: " << 50*4 << ")" << std::endl;
  return 0;
}