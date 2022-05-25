#include <iostream>
#include "types.h"
#pragma once

using namespace caf;

// this information is passed to a supervisor to specify all information
// on a child process. this is the function entry point "start", but also
// a unique name of the child "child_id". then there options like if the
// child process is significant. all this information is similiar to the
// erlang implementation "childspec"

struct child_specification {
  std::string child_id;
  std::function<void(event_based_actor*)> start;  // replaces erlang MFA args
  caf::string_view restart;
  bool significant;
  caf::string_view shutdown;
  std::chrono::milliseconds wait_time;
  caf::string_view type;

  child_specification() : start(), restart(type_name<transient>::value),
  significant(false), shutdown(type_name<wait>::value),
  wait_time(0), type(type_name<work>::value) { }
};

// this data structure actually holds a child. meaning there is an actor inside
// it. so this implies that the struct should not be copied but at best be moved.
// the actor should not be duplicated. in this data structure also dynamic
// state information is present. like the restart_count may change during
// execution. if a maximum is reached, the child will no longer be restarted and
// the supervisor will terminate all children and terminate itself.

struct child {
  std::string child_id;
  actor_addr address;
  actor process;
  int restart_count;
  std::chrono::time_point<std::chrono::system_clock> restart_period_start;

  child() : child_id(), address(), restart_count(0),
  restart_period_start(std::chrono::system_clock::time_point::min()) {}

  child( const child& ) = delete;
  child& operator=(const child& ) = delete;
  child( child&& copy) noexcept;
  child& operator=(child&& copy) noexcept;
};

// The supervisor flags, define how the children of a supervisor will
// be restarted. There is an intensity, a restart_period and also a
// strategy like "one_for_one", one_for_all, etc. - please have a look
// on the Erlang supervisor which kind of serves like a blueprint for
// this implementation

struct supervisor_flags {
  caf::string_view restart_strategy;
  uint32_t restart_intensity;
  std::chrono::seconds restart_period;
  caf::string_view auto_shutdown;

  supervisor_flags(caf::string_view restart_strategy,
  uint32_t restart_intensity, std::chrono::seconds restart_period,
  caf::string_view auto_shutdown) : restart_strategy(restart_strategy),
  restart_intensity(restart_intensity), restart_period(restart_period),
  auto_shutdown(auto_shutdown) {};
  supervisor_flags() : restart_strategy(type_name<one_for_one>::value),
  restart_intensity(1), restart_period(std::chrono::seconds(5)),
  auto_shutdown(type_name<never>::value) {};
};

// this resembles the dyamic state of the supervisor, the dynamic state
// should never be copied or moved, since it is unique to only this
// specific supervisor. so we disable all copy/move semantics.

struct supervisor_dynamic_state {
  std::vector<child_specification> specs_;
  std::vector<child> children_;
  supervisor_flags flags_;

  supervisor_dynamic_state() = default;
  supervisor_dynamic_state( const supervisor_dynamic_state&)
  = delete;
  supervisor_dynamic_state& operator=(const supervisor_dynamic_state&)
  = delete;
  supervisor_dynamic_state( supervisor_dynamic_state&& copy) noexcept
  = delete;
  supervisor_dynamic_state& operator=(supervisor_dynamic_state&& copy) noexcept
  = delete;
};

// this is the supervisor itself. it is a functor, i.e. function object.
// the entry point called by actor_system::spawn is the operator(). So
// the operator() will only call init() to make the supervisor start.
// the advantage here of the function object design is, that we can
// configure the function object to our needs before spawning it.

class supervisor {
 public:
  supervisor() { ptr_ = std::make_shared<supervisor_dynamic_state>(); }
  void init(const std::vector<child_specification>& specs,
            supervisor_flags flags = supervisor_flags());
  void operator()(event_based_actor* self);  // entry point

 protected:
  std::shared_ptr<supervisor_dynamic_state> ptr_;
};