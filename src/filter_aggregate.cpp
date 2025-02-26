#include <iostream>

#include "tsl/tsl.hpp"
#include "simd_utils.hpp"


int main(void) {
  std::cout << "Maximum degree of parallelism for "
            << tsl::type_name<unsigned long long int>() << ": "
            << simd_helper::max_par<unsigned long long int>() << " "
            << "(" << tsl::type_name<simd_helper::target::max_width_extension_t>() << ")" << std::endl;
  std::cout << "Minimum degree of parallelism for "
            << tsl::type_name<unsigned long long int>() << ": "
            << simd_helper::min_par<unsigned long long int>() << " "
            << "(" << tsl::type_name<simd_helper::target::min_width_extension_t>() << ")" << std::endl;
  return 0;
}