#include <iostream>
#include "caf/all.hpp"

using namespace caf;

void caf_main(actor_system& system) {
  std::cout << "hello world" << std::endl;
}

CAF_MAIN()