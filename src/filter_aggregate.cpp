#include <iostream>
#include "tsl/tsl.hpp"


int main(void) {
  std::cout << tsl::type_name<int>() << std::endl;
  return 0;
}