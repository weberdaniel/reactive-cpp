#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

void caf_main(actor_system& system) {
  //std::cout << "hello world" << std::endl;
  auto po = system.spawn(pong);
  ping_actor = ping(po);
  auto pi = system.spawn(ping, );

  /*auto sv = system.spawn(supervisor, pi, type_name<simple_one_for_one>::value,
                               10, std::chrono::microseconds(30), sv_actor);*/
}

CAF_MAIN()