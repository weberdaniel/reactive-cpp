//
#ifndef ACTORDEMO_PONG_H
#define ACTORDEMO_PONG_H

#include <iostream>
#include "caf/all.hpp"

using namespace caf;

struct pong_state {
  int32_t received = 0;
};


class pong : public event_based_actor {
 public:
  pong(actor_config& cfg): event_based_actor(cfg) { }

 protected:
  behavior make_behavior() override {
    delayed_send(this, std::chrono::seconds(3), 3);
    return {
      [=](const std::string& what) {
        received_++;
        aout(this) << "message nr " << received_ << std::endl;
        if(received_ == 3) {
          received_ = 0;
          throw std::bad_alloc();
        }
      },
      [=](const int keep_alive) {
          aout(this) << "keep alive" << received_ << std::endl;
          delayed_send(this, std::chrono::seconds(3), 3);
      }
    };
  }

 private:
    int received_ {0};

};

/*behavior pong(stateful_actor<pong_state>* self) {
  return {
    [=](const std::string& what) -> std::string {
      self->state.received++;
      aout(self) << "message nr " << self->state.received << std::endl;
      if(self->state.received == 10) {
        throw std::bad_alloc();
      }
    }
  };
}*/

#endif //ACTORDEMO_PONG_H