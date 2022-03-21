#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

void caf_main(actor_system& system) {
  actor po = system.spawn(pong);
  anon_send(po, get_child_v);

  //auto pi = system.spawn<ping>(po);
  auto sv = system.spawn< supervisor<ping,actor&> >(
    type_name<one_for_all>::value, 1, std::chrono::milliseconds(10),
    "hello", po);
}

CAF_MAIN(id_block::supervisor)