#ifndef ACTORDEMO_WORKER_H
#define ACTORDEMO_WORKER_H
#pragma once
#include <string>
#include <memory>
#include <utility>
#include "types.h"

using namespace caf;

// represents the state subset that never changes, even if the
// worker is restarted. instead of static state, you could also
// call this the configuration of the worker.

struct worker_static_state {
    uint32_t process_id {0};
    std::string process_name;
    std::chrono::milliseconds forward_message_delay_ms {0};
    std::chrono::seconds keep_alive_delay_sec {1};
    uint32_t max_keep_alive_until_worker_crash {3};
};

// represents the state subset of worker that may change during
// execution. the state should not be copied, so all copy/move
// semantics is disabled. move operations must be marked noexcept
// (see C++ Core Guideline C.66)

struct worker_dynamic_state {
    uint32_t received_keep_alives {0};
    uint32_t received_messages {0};

    worker_dynamic_state() = default;
    worker_dynamic_state( const worker_dynamic_state&)
    = delete;
    worker_dynamic_state& operator=(const worker_dynamic_state&)
    = delete;
    worker_dynamic_state( worker_dynamic_state&& copy) noexcept
    = delete;
    worker_dynamic_state& operator=(worker_dynamic_state&& copy) noexcept
    = delete;
};

// this class represents a worker. the worker has a keep-alive mechanism,
// so it will not get terminated due to inactivity by the actor framework.
// it registers itself with a name and a number. on receiving a message it
// will try to forward that message to the next worker with the same name.
// e.g. worker_0 -> worker_1

class worker {
 public:
  explicit worker(const std::string& process_name, uint32_t process_id,
  std::chrono::milliseconds forward_message_delay_ms);
  worker(const worker& ref) { static_state = ref.static_state; }
  ~worker() = default;
  void init(event_based_actor* self);
  void operator()(event_based_actor* self) { init(self); } // entry point

 protected:
  behavior operational;
  std::shared_ptr<worker_static_state> static_state;
};
#endif //ACTORDEMO_WORKER_H
