#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

/*void startup_actor(event_based_actor* self) {
  auto pong_sv =
    self->home_system().spawn< supervisor<pong, void> >(
      type_name<one_for_all>::value,
      1,
      std::chrono::seconds(10),
      type_name<never>::value
    );

    self->home_system().spawn< supervisor<ping, void> >(
      type_name<one_for_all>::value,
      1,
      std::chrono::seconds(10),
      type_name<never>::value );
  // please note, simply adding code here after the self->request, does no longer
  // guarantee order of execution.
};*/

class call {
public:

  call(int tst) : testvar(0) {
  }

  void operator()(event_based_actor* self) {
      a_.assign([=](int a) {
          testvar++;
          aout(self) << "testvar: " << testvar << "\n";
          return a;
      });
      self->delayed_send(self, std::chrono::seconds(3), 3);
      self->become(a_);
  }

  behavior a_;
  int testvar = 0;
};


void caf_main(actor_system& system) {
    call a(3);
    //std::function<void(event_based_actor*)> bla = a;
    //system.spawn(startup_actor);
    system.spawn(a);
  //actor po = system.spawn(pong);
  //anon_send(po, get_child_v);

  //auto pi = system.spawn<ping>(po);
  /*auto sv = system.spawn< supervisor<ping,actor&> >(
    type_name<one_for_all>::value, 1, std::chrono::milliseconds(10),
    "hello", po);*/
}

CAF_MAIN(id_block::supervisor)