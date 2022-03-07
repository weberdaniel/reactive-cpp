#ifndef ACTORDEMO_PING_H
#define ACTORDEMO_PING_H

#include <string>
#include <iostream>
#include "caf/all.hpp"

using namespace caf;

behavior ping(event_based_actor* self, const actor& buddy) {
  self->request(buddy, std::chrono::seconds(5), "hello!")
  .then( [=](const std::string& what) {
    self->become(ping(self,buddy));
  });
  return {};
}

#endif //ACTORDEMO_PING_H