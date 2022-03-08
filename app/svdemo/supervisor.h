#include <iostream>
#include "caf/all.hpp"

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(supervisor, caf::first_custom_type_id)
CAF_ADD_ATOM(supervisor, one_for_one);
CAF_ADD_ATOM(supervisor, one_for_all);
CAF_ADD_ATOM(supervisor, rest_for_one);
CAF_ADD_ATOM(supervisor, simple_one_for_one);
CAF_END_TYPE_ID_BLOCK(supervisor)

struct supervisor_state  {
  // a list of actors to supervise
  std::vector<actor> children;
  // restart_strategy: assign with type_name<simple_one_for_one>::value;
  caf::string_view restart_strategy;
  // how often are we going to restart during a specific period
  int32_t intensity;
  std::chrono::microseconds period;
};

behavior supervisor(stateful_actor<supervisor_state>* self, const actor& buddy) {
  auto v = caf::put_atom_v;
  aout(self) << "Supervisor started (Id " << self->id() << ")" << std::endl;
  self->home_system().registry().put("sv", self);
  self->monitor(buddy);
  actor_id mon_id = buddy->id();
  aout(self) << "Supervisor (Id " << self->id() << ") monitors Actor (Id " << mon_id << ")" << std::endl;
  self->set_down_handler([=](down_msg&) {
    aout(self) << "down" << std::endl;
  });
  return {
    [=](int32_t val) { self->state.restart_strategy = type_name<simple_one_for_one>::value; }
  };
}