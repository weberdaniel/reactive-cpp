#include <iostream>
#include "types.h"
#pragma once

using namespace caf;

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
  // only allow move semantics
};

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

class supervisor {
 public:
  supervisor() { ptr_ = std::make_shared<supervisor_dynamic_state>(); }
  void init(const std::vector<child_specification>& specs,
            supervisor_flags flags = supervisor_flags());
  void operator()(event_based_actor* self);  // entry point

 protected:
  std::shared_ptr<supervisor_dynamic_state> ptr_;
};