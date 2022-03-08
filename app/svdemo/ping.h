#ifndef ACTORDEMO_PING_H
#define ACTORDEMO_PING_H

#include <string>
#include <iostream>
#include "caf/all.hpp"

using namespace caf;

behavior ping_fun(event_based_actor* self, actor buddy) {
  self->request(buddy, std::chrono::seconds(5), "hello!")
  .then( [=](const std::string& what) {
    self->become(ping_fun(self,buddy));
  });
  return {};
}

class ping : public event_based_actor {
 public:
  ping(actor_config& cfg, actor buddy):
     event_based_actor(cfg), m_buddy(buddy)  {
    // nop
  }

  behavior make_behavior() override {
      request(m_buddy, std::chrono::seconds(5), "hello!")
              .then( [=](const std::string& what) {
                  become(ping);
              });
      return {};
  }

  actor m_buddy;
  behavior

};

#endif //ACTORDEMO_PING_H