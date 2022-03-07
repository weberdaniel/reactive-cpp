#include <iostream>
#include "caf/all.hpp"

using namespace caf;

struct svstate  {
  int32_t example = 0;
};

behavior supervisor_init(stateful_actor<svstate>* self) {
  return {
    [=](int32_t val) { self->state.example = val; }
  };
}