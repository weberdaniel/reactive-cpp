#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

void caf_main(actor_system& system) {
  auto po = system.spawn(pong);
  auto pi = system.spawn<ping>(po);
}

CAF_MAIN(id_block::supervisor)