#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

struct functor_state {
    int delay;
    int counter;
};

class functor {
public:
  functor(int tst) {
      std::cout << "create" << std::endl;
      ptr_ = std::make_shared<functor_state>();
      ptr_->delay = tst;
      ptr_->counter = 0;
  }

  functor(const functor& ref) {
      ptr_ = ref.ptr_;
  }

  ~functor() {
  }

  void operator()(event_based_actor* self) {
    std::shared_ptr<functor_state> ptr = ptr_;
    a_.assign([ptr,self](int a) {
        ptr->counter++;
        if( ptr->counter == 3) {
            throw std::bad_alloc();
        }
        aout(self) << "ongoing\n";
        self->delayed_send(self, std::chrono::seconds(ptr->delay), 3);
    });
    self->delayed_send(self, std::chrono::seconds(ptr->delay), 3);
    self->become(a_);
  }

  behavior a_;
  std::shared_ptr<functor_state> ptr_;
};

void startup_actor(event_based_actor* self) {
    /*auto pong_sv =
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
        type_name<never>::value );*/
    // please note, simply adding code here after the self->request, does no longer
    // guarantee order of execution.
    supervisor sv;
    functor tester(2);

    childspec spec;
    spec.child_id = "tester";
    spec.start = tester;

    std::vector<childspec> specs;
    specs.push_back(spec);

    sv.init(specs);
    self->home_system().spawn(sv);
};


void caf_main(actor_system& system) {
    system.spawn(startup_actor);
    system.await_actors_before_shutdown();
}

CAF_MAIN(id_block::supervisor)