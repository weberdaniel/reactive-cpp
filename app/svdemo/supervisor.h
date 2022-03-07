#include <iostream>
#include "caf/all.hpp"

using namespace caf;

struct svstate  {
  int32_t svid = 0;
};

behavior supervisor_init(const actor& buddy, stateful_actor<svstate>* self) {
    self->home_system().registry().put("sv", self);
    self->monitor(buddy);
  return {
    [=](int32_t val) { self->state.svid= val; }
  };
}