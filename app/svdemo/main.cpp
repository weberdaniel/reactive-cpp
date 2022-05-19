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