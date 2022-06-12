// Copyright 2022 Daniel Weber
#include <iostream>
#include <functional>
#include "svdemo/types.h"
#include "svdemo/supervisor.h"
#include "svdemo/worker.h"

using caf::event_based_actor;
using caf::actor;
using caf::actor_system;

// this function will create specifications for number n of
// workers

std::vector<child_specification> init_workers(int n) {
  std::vector<child_specification> children;
  for (int i = 1; i <= n; i++) {
    child_specification specification;
    specification.child_id = std::string("worker_") + std::to_string(i);
    specification.start = std::move(
      worker("worker", i, std::chrono::milliseconds(100)));
    children.push_back(std::move(specification));
  }
  return children;
}

// this actor function will setup the application itself.
// this resembles kind of the erlang application behaviour.

void application(event_based_actor* self) {
  CAF_LOG_INFO("Application Start");
  supervisor supervisor;
  std::vector<child_specification> specifications = init_workers(5);
  supervisor.init(specifications);
  self->home_system().spawn(supervisor);
  CAF_LOG_INFO("Application End");
}

// caf_main will start the application and than will await
// all actors

void caf_main(actor_system& system) {  // NOLINT
  system.spawn(application);
  system.await_actors_before_shutdown();
}

CAF_MAIN(caf::id_block::supervisor)
