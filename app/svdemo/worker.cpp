#include "worker.h"

worker::worker(const std::string& process_name, uint32_t process_id,
std::chrono::milliseconds forward_message_delay_ms) {
  static_state = std::make_shared<worker_static_state>();
  static_state->process_name = process_name;
  static_state->process_id = process_id;
  static_state->forward_message_delay_ms = forward_message_delay_ms;
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

  message_handler keep_alive_handler {
    [static_state_ptr,dynamic_state_ptr,self](keep_alive) {
      dynamic_state_ptr->received_keep_alives++;
      if( static_state_ptr->max_keep_alive_until_worker_crash != 0 ) {
        if (dynamic_state_ptr->received_keep_alives ==
            static_state_ptr->max_keep_alive_until_worker_crash) {
          throw std::bad_alloc();
        }
      }
      self->delayed_send(self,
        static_state_ptr->keep_alive_delay_ms, keep_alive_v);
    },
  };

  // the forwarding handler receives a mail and checks if the sources comes
  // from the "previous" worker, e.g. "worker_0". Then it will forward that
  // message to the "next" worker in the chain/circle, e.g. "worker_2".

  message_handler forwarding_handler {
    [static_state_ptr, dynamic_state_ptr, self](const mail& request) {
    // forward msg to the next actor if it originates from the previous
      if(request.source == static_state_ptr->process_id-1) {
        auto target =
          self->home_system().registry().get<actor>(static_state_ptr->process_name +
          "_" + std::to_string(static_state_ptr->process_id+1) );
        // check if the "next" worker does actually exist.
        if( target->address() != nullptr ) {
          mail response;
          response.source = static_state_ptr->process_id;
          response.destination = static_state_ptr->process_id + 1;
          response.content = "hello world!";
          self->send(target, response);
        }
      }
    }
  };
  operational.assign(keep_alive_handler.or_else(forwarding_handler));
  // start keep_alive_cycle
  self->delayed_send(self,
  static_state_ptr->keep_alive_delay_ms, keep_alive_v);
  // register name, will be deleted automatically on actor death
  self->home_system().registry().put( static_state_ptr->process_name
  + "_" + std::to_string(static_state_ptr->process_id), self );
  // swtich into operational mode
  self->become(operational);
}
