#include <iostream>
#include <functional>
#include "supervisor_caf_types.h"
#include "supervisor.h"
#include "ping.h"
#include "pong.h"
#include "worker.h"

using namespace caf;

void application(event_based_actor* self) {
  supervisor supervisor;
  int n = 5;

  // start n workers and let them be supervised by a central supervisor
  std::vector<childspec> childspec_list;
  // i = 0 is invalid id by defintion in messages.h
  for( int i = 1; i <= n; i++) {
    childspec child_specification;
    child_specification.child_id =
      std::string("worker") + "_" + std::to_string(i);
    child_specification.start =
      std::move(worker("worker",i,
      std::chrono::milliseconds(100)) );
    childspec_list.push_back(std::move(child_specification));
  }

  supervisor.init(childspec_list);
  self->home_system().spawn(supervisor);
};

void caf_main(actor_system& system) {
  system.spawn(application);
  system.await_actors_before_shutdown();
}

CAF_MAIN(id_block::supervisor)