#ifndef ACTORDEMO_PING_H
#define ACTORDEMO_PING_H
#pragma once

#include <string>
#include <iostream>
#include "supervisor_caf_types.h"

using namespace caf;

class ping : public event_based_actor {
 public:
  ping(actor_config& cfg): event_based_actor(cfg) {
    pinging_.assign([=](pinging_atom){
      auto buddy = this->home_system().registry().get<actor_addr>("pong");
      if( buddy.get() != nullptr ) {
        send(actor_cast<actor>(buddy), "a");
      }
      delayed_send(this, std::chrono::microseconds(200), pinging_atom_v);
    });
  }

protected:
  behavior make_behavior() override {
    std::string ping_string("ping");
    this->home_system().registry().erase("ping");
    this->home_system().registry().put("ping",this);
    send(this, pinging_atom_v);
    return pinging_;
  }

 private:
   behavior pinging_;
};

#endif //ACTORDEMO_PING_H