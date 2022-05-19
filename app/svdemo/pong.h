//
#ifndef ACTORDEMO_PONG_H
#define ACTORDEMO_PONG_H
#pragma once

#include <iostream>
#include "supervisor_caf_types.h"

using namespace caf;

struct pong_state {
  int32_t received = 0;
};

class pong : public event_based_actor {
 public:
  pong(actor_config& cfg): event_based_actor(cfg) { }

 protected:
  behavior make_behavior() override {
    this->home_system().registry().erase("pong");
    this->home_system().registry().put("pong",this);
    delayed_send(this, std::chrono::seconds(3), 3);
    return {
      [=](const std::string& what) {
        received_++;
        if(received_ == 3000) {
          received_ = 0;
          throw std::bad_alloc();
        }
      },
      [=](keep_alive) {
          aout(this) << "keep alive" << received_ << std::endl;
          delayed_send(this, std::chrono::seconds(3), keep_alive_v);
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