#ifndef ACTORDEMO_WORKER_H
#define ACTORDEMO_WORKER_H
#pragma once

#include <string>
#include <memory>
#include "supervisor_caf_types.h"

using namespace caf;

struct worker_static_state {
  uint32_t process_id {0};
  std::string process_name;
  uint32_t delay {0};
};

struct worker_dynamic_state {
  uint32_t counter {0};
};

class worker {
 public:
  explicit worker(std::string process_name, uint32_t process_id, uint32_t delay) {
    static_state = std::make_shared<worker_static_state>();
    // registry_name = process_name + "_" + process_id
    static_state->process_name = process_name;
    static_state->process_id = process_id;
    static_state->delay = delay;
  }

  worker(const worker& ref) {
    static_state = ref.static_state;
  }

  ~worker() { }

  void init(event_based_actor* self) {
    std::shared_ptr<worker_static_state> static_state_ptr = static_state;
    std::shared_ptr<worker_dynamic_state> dynamic_state_ptr =
      std::make_shared<worker_dynamic_state>();

    working.assign(

      // handle keep_alive messages to let this actor stay alive
      [static_state_ptr,dynamic_state_ptr,self](keep_alive) {
        dynamic_state_ptr->counter++;
          if( dynamic_state_ptr->counter == 3) {
            throw std::bad_alloc();
          }
          self->delayed_send(self,
            std::chrono::seconds(static_state_ptr->delay), keep_alive_v);
      },

      // handle mail from previous worker in circle and forward it to next
      [static_state_ptr, dynamic_state_ptr, self](mail msg) {
        if(msg.source == static_state_ptr->process_id-1) {
          // forward msg if possible
          auto target =
            self->home_system().registry().get<actor>(static_state_ptr->process_name +
              "_" + std::to_string(static_state_ptr->process_id+1) );
          if( target->address() != nullptr ) {
            mail msg;
            msg.source = static_state_ptr->process_id;
            msg.destination = static_state_ptr->process_id + 1;
            msg.content = "hello world!";
            self->send(target, msg);
          }
        }
      }

    );

    // start keep_alive_cycle
    self->delayed_send(self, std::chrono::seconds(static_state_ptr->delay),
      keep_alive_v);
    // register name, will be deleted automatically on actor death
    self->home_system().registry().put( static_state_ptr->process_name
      + "_" + std::to_string(static_state_ptr->process_id), self );
    // swtich into working mode
    self->become(working);
  }

  // entry point for functor based actors
  void operator()(event_based_actor* self) {
    init(self);
  }

  behavior working;
  std::shared_ptr<worker_static_state> static_state;
};

#endif //ACTORDEMO_WORKER_H
