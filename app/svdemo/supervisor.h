#include <iostream>
#include "caf/all.hpp"

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(supervisor, caf::first_custom_type_id)
CAF_ADD_ATOM(supervisor, one_for_one);
CAF_ADD_ATOM(supervisor, one_for_all);
CAF_ADD_ATOM(supervisor, rest_for_one);
CAF_ADD_ATOM(supervisor, simple_one_for_one);
CAF_ADD_ATOM(supervisor, pinging_atom);
CAF_ADD_ATOM(supervisor, get_child);
CAF_END_TYPE_ID_BLOCK(supervisor)


template<class TClassActor, class TClassActorStaticArgs>
class supervisor: public event_based_actor {
 public:
  supervisor(actor_config& cfg, caf::string_view restart_strategy,
             int32_t intensity, std::chrono::microseconds period, std::string
  process_name, TClassActorStaticArgs class_actor_static_args) :
  event_based_actor(cfg),
  restart_strategy(restart_strategy),
  intensity(intensity),
  period(period),
  process_name(process_name),
  class_actor_static_args_(class_actor_static_args)
  {
    return_child_.assign(
      [=](get_child) -> actor {
        return child;
      }
    );
  }

  behavior make_behavior() override {
    this->home_system().registry().put(process_name, this);
    child = spawn<TClassActor>(class_actor_static_args_);
    return return_child_;
  }

 protected:

 private:
  actor child;
  caf::string_view restart_strategy;
  int32_t intensity;
  std::chrono::microseconds period;
  std::string process_name;
  behavior init_;
  behavior supervising_;
  behavior return_child_;
  TClassActorStaticArgs class_actor_static_args_;
};

/*template<class F>
behavior supervisor( stateful_actor<supervisor_state>* self,
                     caf::string_view restart_strategy,
                     int32_t intensity,
                     std::chrono::microseconds period,
                     std::string process_name,
                     F fun ) {
  aout(self) << "Supervisor started (Id " << self->id() << ")" << std::endl;
  self->home_system().registry().put(process_name, self);
  aout(self) << "Actor started (Id " << self->id() << ")" << std::endl;
  auto child = self->home_system().spawn(fun);
  self->monitor(child);
  actor_id mon_id = child->id();
  aout(self) << "Supervisor (Id " << self->id() << ") monitors Actor (Id " << mon_id << ")" << std::endl;
  self->set_down_handler([=](down_msg& msg) {
    if (self->state.restart_strategy == type_name<one_for_one>::value) {
      self->home_system().spawn(fun);
    }
  });
  return {
    [=](int32_t val) { self->state.restart_strategy = type_name<simple_one_for_one>::value; }
  };
}*/