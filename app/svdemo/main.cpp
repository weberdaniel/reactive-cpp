#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

void startup_actor(event_based_actor* self) {
  auto pong_sv =
    self->home_system().spawn< supervisor<pong,void> >(
      type_name<one_for_all>::value,
      1,
      std::chrono::milliseconds(10)
    );

    self->home_system().spawn< supervisor<ping,void> >(
      type_name<one_for_all>::value,
      1,
      std::chrono::milliseconds(10) );
  // please note, simply adding code here after the self->request, does no longer
  // guarantee order of execution.
};

void caf_main(actor_system& system) {

    system.spawn(startup_actor);
  //actor po = system.spawn(pong);
  //anon_send(po, get_child_v);

  //auto pi = system.spawn<ping>(po);
  /*auto sv = system.spawn< supervisor<ping,actor&> >(
    type_name<one_for_all>::value, 1, std::chrono::milliseconds(10),
    "hello", po);*/
}

CAF_MAIN(id_block::supervisor)