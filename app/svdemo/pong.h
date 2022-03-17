//
#ifndef ACTORDEMO_PONG_H
#define ACTORDEMO_PONG_H

#include <iostream>
#include "caf/all.hpp"

using namespace caf;

struct pong_state {
  int32_t received = 0;
};

behavior pong(stateful_actor<pong_state>* self) {
  return {
    [=](const std::string& what) -> std::string {
      self->state.received++;
      aout(self) << "message nr " << self->state.received << std::endl;
      if(self->state.received == 10) {
        throw std::bad_alloc();
      }
    }
  };
}

#endif //ACTORDEMO_PONG_H
