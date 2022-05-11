#include <iostream>
#include "caf/all.hpp"
#include "supervisor.h"
#include <functional>
#include "ping.h"
#include "pong.h"

using namespace caf;

struct data {
  int source;
  int destination;
  std::string content;
};

template <class Inspector>
bool inspect(Inspector& f, data x) {
  return f.object(x).fields(
    f.field("source", x.source),
    f.field("destination", x.destination),
    f.field("content", x.content));
}

struct worker_static_state {
  int instance_id;
  std::string name;

  int delay;
};

struct worker_dynamic_state {
  int counter;
};

class worker {
public:
  explicit worker(std::string name, int instance_id, int delay) {
    static_state = std::make_shared<worker_static_state>();
    static_state->instance_id = instance_id;
    static_state->delay = delay;
    static_state->name = name;
  }

  worker(const worker& ref) {
    static_state = ref.static_state;
  }

  ~worker() { }

  void init(event_based_actor* self) {
    std::shared_ptr<worker_static_state> config = static_state;
    std::shared_ptr<worker_dynamic_state> state = std::make_shared<worker_dynamic_state>();

    working.assign(
      [config,state,self](int a) {
        state->counter++;
        if( state->counter == 3) {
          throw std::bad_alloc();
        }
        aout(self) << "ongoing\n";
        self->delayed_send(self, std::chrono::seconds(config->delay), 3);
      },
      [config,state,self]() {

      }
    );

    self->delayed_send(self, std::chrono::seconds(config->delay), 3);
    self->home_system().registry().put(config->name,self);
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
  worker worker( "circle", 0, 2);

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