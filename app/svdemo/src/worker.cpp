// Copyright 2022 Daniel Weber
#include "svdemo/worker.h"

worker::worker(const std::string& process_name, uint32_t process_id,
std::chrono::milliseconds create_new_message_delay_ms, bool worker_creates_messages ) {
  static_state = std::make_shared<worker_static_state>();
  static_state->process_name = process_name;
  static_state->process_id = process_id;
  static_state->create_new_message_delay_ms = create_new_message_delay_ms;
  static_state->worker_creates_messages = worker_creates_messages;
}

// while worker::operator() is the entry point for spawning an actor,
// the operator() will call just init(...) to do the real work, since
// the name is more intuitive.

void worker::init(event_based_actor* self) {
  std::shared_ptr<worker_static_state> static_state_ptr = static_state;
  std::shared_ptr<worker_dynamic_state> dynamic_state_ptr =
  std::make_shared<worker_dynamic_state>();

  // takes a keep_alive message to let the worker stay alive. if maximum
  // amount of keep_alive messages is reached, the worker throws a
  // bad_alloc. this shall illustrate that even after throwing a bad_alloc,
  // the worker can be restarted by a supervisor.

  operational = {
    [static_state_ptr, dynamic_state_ptr, self](trigger_create_new_message) {
        if(static_state_ptr->worker_creates_messages) {
            self->delayed_send(self,
              static_state_ptr->create_new_message_delay_ms,
              trigger_create_new_message_v);
            mail new_message;
            new_message.content = std::move(std::string("a", static_state_ptr->create_new_message_size_bytes));
            new_message.time_to_live = 64;
            new_message.source = static_state_ptr->process_id;
            auto target = self->home_system().registry().get<actor>(static_state_ptr->process_name +
              "_" + std::to_string(static_state_ptr->process_id + 1));
            if (target->address() == nullptr) {
              CAF_LOG_INFO("Message Dropped");
            } else {
              CAF_LOG_INFO("Message sent");
              self->send(target, new_message);
            }
        }
      },
      [static_state_ptr, dynamic_state_ptr, self](keep_alive) {
      dynamic_state_ptr->received_keep_alives++;
      if (static_state_ptr->max_keep_alive_until_worker_crash != 0) {
        if (dynamic_state_ptr->received_keep_alives ==
            static_state_ptr->max_keep_alive_until_worker_crash) {
          if (static_state_ptr->process_id == 2) {
              throw std::bad_alloc();
          }
        }
      }
      self->delayed_send(self,
        static_state_ptr->keep_alive_delay_ms, keep_alive_v);
    },
    [static_state_ptr, dynamic_state_ptr, self](const mail& request) {
      // forward msg to the next actor if it originates from the previous
      if (request.source == static_state_ptr->process_id-1) {
        auto target =
        self->home_system().registry().
          get<actor>(static_state_ptr->process_name +
          "_" + std::to_string(static_state_ptr->process_id+1) );
          // check if the "next" worker does actually exist.
          mail response;
          response.source = static_state_ptr->process_id;
          response.content = request.content;
          response.time_to_live = request.time_to_live;
          if(response.time_to_live == 0) {
            CAF_LOG_INFO("end of life for message reached.");
          } else {
            if (target.address() != nullptr) {
              response.destination = static_state_ptr->process_id + 1;
            } else {
              response.destination = 0;
              target = self->home_system().registry().
                get<actor>(static_state_ptr->process_name + "_0");
            }
            if (target->address() != nullptr) {
              CAF_LOG_INFO("Message forwarded");
              self->send(target, response);
            } else {
              CAF_LOG_INFO("Message to be forwarded has been dropped");
            }
          }
      }
    }
  };

  // start keep_alive_cycle
  self->delayed_send(self,
  static_state_ptr->keep_alive_delay_ms, keep_alive_v);
  if(static_state_ptr->worker_creates_messages) {
    self->delayed_send(self,
    static_state_ptr->create_new_message_delay_ms, trigger_create_new_message_v);
  }
  // register name, will be deleted automatically on actor death
  self->home_system().registry().put(static_state_ptr->process_name
  + "_" + std::to_string(static_state_ptr->process_id), self);
  // swtich into operational mode
  self->become(operational);
}
