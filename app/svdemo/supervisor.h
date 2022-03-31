#include <iostream>
#include "caf/all.hpp"

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(supervisor, caf::first_custom_type_id)
// public interface messages
CAF_ADD_ATOM(supervisor, start_child);
CAF_ADD_ATOM(supervisor, terminate_child);
// process_function
CAF_ADD_ATOM(supervisor, work);
CAF_ADD_ATOM(supervisor, supervise);
// restart_type
CAF_ADD_ATOM(supervisor, permanent);
CAF_ADD_ATOM(supervisor, transient);
CAF_ADD_ATOM(supervisor, temporary);
// restart_strategy
CAF_ADD_ATOM(supervisor, one_for_one);
CAF_ADD_ATOM(supervisor, one_for_all);
CAF_ADD_ATOM(supervisor, rest_for_one);
CAF_ADD_ATOM(supervisor, simple_one_for_one);
// auto-shutdown
CAF_ADD_ATOM(supervisor, never);
CAF_ADD_ATOM(supervisor, any_significant);
CAF_ADD_ATOM(supervisor, all_significant);
// shutdown
CAF_ADD_ATOM(supervisor, brutal_kill);
CAF_ADD_ATOM(supervisor, infinity);
CAF_ADD_ATOM(supervisor, wait);
// other
CAF_ADD_ATOM(supervisor, pinging_atom);
CAF_ADD_ATOM(supervisor, get_child);
CAF_ADD_ATOM(supervisor, keep_alive);
CAF_END_TYPE_ID_BLOCK(supervisor)

template<class TStaticStartArgs>
struct childspec {
    std::string child_id;
    TStaticStartArgs start;
    caf::string_view restart;
    bool significant;
    caf::string_view shutdown;
    std::chrono::milliseconds wait_time;
    caf::string_view type;
};

template<class TStaticStartArgs>
class child {
    childspec<TStaticStartArgs> spec;
    actor child;
    int restart_count;
    std::chrono::time_point<std::chrono::system_clock> restart_period_start;
};

struct sup_flags {
  sup_flags(caf::string_view restart_strategy, uint32_t restart_intensity,
    std::chrono::seconds restart_period, caf::string_view auto_shutdown) :
    restart_strategy(restart_strategy), restart_intensity(restart_intensity),
    restart_period(restart_period), auto_shutdown(auto_shutdown) {};

  caf::string_view restart_strategy;
  uint32_t restart_intensity;
  std::chrono::seconds restart_period;
  caf::string_view auto_shutdown;
};

template<class TClassActor, class TStaticStartArgs>
class supervisor: public event_based_actor {
 public:
  supervisor(actor_config& cfg, sup_flags flags, TStaticStartArgs
  class_actor_static_args) : event_based_actor(cfg), flags(
  flags), class_actor_static_args_(class_actor_static_args) { }

  supervisor(actor_config& cfg, caf::string_view restart_strategy,
  uint32_t intensity, std::chrono::seconds period, caf::string_view
  auto_shutdown, TStaticStartArgs class_actor_static_args)
  : event_based_actor(cfg), flags(restart_strategy, intensity,
  period, auto_shutdown), class_actor_static_args_(class_actor_static_args) { }

 protected:
  behavior make_behavior() override {

    supervising_.assign(
      [=](keep_alive) {
        delayed_send(this, std::chrono::seconds(1), keep_alive_v);
      },
      [=](start_child, childspec<TStaticStartArgs> spec) {
        actor new_child = spawn<TClassActor>(class_actor_static_args_);
        this->monitor(new_child);
        ::child<TStaticStartArgs> storage;
      },
      [=](terminate_child, actor_addr child) {

      }
    );

    delayed_send(this, std::chrono::seconds(1), keep_alive_v);
    child = spawn<TClassActor>(class_actor_static_args_);
    this->monitor(child);
    this->set_down_handler([&](down_msg& msg) {
      child = spawn<TClassActor>(class_actor_static_args_);
      this->monitor(child);
      become(supervising_);
    } );
    return supervising_;
  }

 private:
  actor child;
  sup_flags flags;
  TStaticStartArgs class_actor_static_args_;
  behavior supervising_;
};

template<class TClassActor>
class supervisor<TClassActor, void> : public event_based_actor {
 public:
  supervisor(actor_config& cfg, caf::string_view restart_strategy,
  uint32_t intensity, std::chrono::seconds period, caf::string_view
  auto_shutdown) : event_based_actor(cfg), flags(restart_strategy, intensity,
  period, auto_shutdown) { }

  behavior make_behavior() override {
    supervising_.assign(
      [=](keep_alive) {
        delayed_send(this, std::chrono::seconds(1), keep_alive_v);
      });
    delayed_send(this, std::chrono::seconds(1), keep_alive_v);
    child = spawn<TClassActor>();
    this->monitor(child);
    this->set_down_handler([&](down_msg& msg) {
      child = spawn<TClassActor>();
      this->monitor(child);
      become(supervising_);
    });
    return supervising_;
  }

 private:
   actor child;
   sup_flags flags;
   behavior supervising_;
};