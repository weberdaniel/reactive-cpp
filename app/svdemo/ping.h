#ifndef ACTORDEMO_PING_H
#define ACTORDEMO_PING_H
#include <string>
#include <iostream>
#include "caf/all.hpp"

using namespace caf;

class ping : public event_based_actor {
 public:
  ping(actor_config& cfg, actor buddy):
    event_based_actor(cfg), m_buddy(buddy)
    {
      pinging_.assign([=](pinging_atom){
          aout(this) << "Actor " << "starts pinging" << std::endl;
          delayed_send(this,std::chrono::seconds(2), pinging_atom_v);
        });
    }

protected:
  behavior make_behavior() override {
    send(this, pinging_atom_v);
    return {
      [=](pinging_atom) {
          aout(this) << "Actor " << "starts pinging\n";
          delayed_send(this,std::chrono::seconds(2), pinging_atom_v);
          //become(pinging_);
        },
    };
  }

 private:
   behavior pinging_;
   actor m_buddy;
};

#endif //ACTORDEMO_PING_H