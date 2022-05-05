#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

struct worker_static_state {
    int delay;
};

struct worker_dynamic_state {
    int counter;
};

class worker {
public:
  explicit worker(int tst) {
    static_state = std::make_shared<worker_static_state>();
    static_state->delay = tst;
  }

  worker(const worker& ref) {
    static_state = ref.static_state;
  }

  ~worker() { }

  void init(event_based_actor* self) {
    std::shared_ptr<worker_static_state> config = static_state;
    std::shared_ptr<worker_dynamic_state> state = std::make_shared<worker_dynamic_state>();

    working.assign([config,state,self](int a) {
        state->counter++;
        if( state->counter == 3) {
            throw std::bad_alloc();
        }
        aout(self) << "ongoing\n";
        self->delayed_send(self, std::chrono::seconds(config->delay), 3);
    });
    self->delayed_send(self, std::chrono::seconds(config->delay), 3);
    self->become(working);
  }

  void operator()(event_based_actor* self) {
      init(self);
  }

  behavior working;
  std::shared_ptr<worker_static_state> static_state;
};

void application(event_based_actor* self) {
    supervisor supervisor;
    worker worker(2);

    childspec child_specification;
    child_specification.child_id = "worker";
    child_specification.start = worker;

    std::vector<childspec> childspec_list;
    childspec_list.push_back(child_specification);

    supervisor.init(childspec_list);
    self->home_system().spawn(supervisor);
};


void caf_main(actor_system& system) {
    system.spawn(application);
    system.await_actors_before_shutdown();
}

CAF_MAIN(id_block::supervisor)