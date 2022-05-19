#include <iostream>
#include <functional>
#include "types.h"
#include "supervisor.h"
#include "worker.h"

CAF_PUSH_WARNINGS
#include <QApplication>
#include <QMainWindow>
CAF_POP_WARNINGS

using namespace caf;

class config : public actor_system_config {
public:
    config() {
        opt_group{custom_options_, "global"}
                .add<std::string>("name,n", "set name")
                .add<std::string>("group,g", "join group");
    }
};

std::vector<childspec> init_workers(int n) {
  std::vector<childspec> children;
  for( int i = 1; i <= n; i++) {
    childspec child_specification;
    child_specification.child_id = std::string("worker_") + std::to_string(i);
    child_specification.start = std::move(
      worker("worker",i, std::chrono::milliseconds(100))
    );
    children.push_back(std::move(child_specification));
  }
  return children;
};

void application(event_based_actor* self) {
  supervisor supervisor;
  std::vector<childspec> child_specifications = init_workers(5);
  supervisor.init(child_specifications);
  self->home_system().spawn(supervisor);

};

int caf_main(actor_system& system, const config& cfg) {
  auto [argc, argv] = cfg.c_args_remainder();
  system.spawn(application);
  QApplication app{argc,argv};
  app.setQuitOnLastWindowClosed(true);
  QMainWindow mw;
  mw.show();
  return app.exec();
  //system.await_actors_before_shutdown();
}


CAF_MAIN(id_block::supervisor)