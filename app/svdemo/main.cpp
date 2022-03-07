#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"

using namespace caf;

void caf_main(actor_system& system) {
  std::cout << "hello world" << std::endl;
  system.spawn(supervisor_init);
}

CAF_MAIN()