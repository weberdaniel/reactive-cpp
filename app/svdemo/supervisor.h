#include <iostream>
#include "caf/all.hpp"

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(supervisor, caf::first_custom_type_id)
// public interface messages
CAF_ADD_ATOM(supervisor, start_child);
CAF_ADD_ATOM(supervisor, restart_child);
CAF_ADD_ATOM(supervisor, count_children);
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
// this functionality will be excluded for now
//CAF_ADD_ATOM(supervisor, simple_one_for_one);
// auto-shutdown
CAF_ADD_ATOM(supervisor, never);
CAF_ADD_ATOM(supervisor, any_significant);
CAF_ADD_ATOM(supervisor, all_significant)
// shutdown
CAF_ADD_ATOM(supervisor, brutal_kill);
CAF_ADD_ATOM(supervisor, infinity);
CAF_ADD_ATOM(supervisor, wait);
// other
CAF_ADD_ATOM(supervisor, pinging_atom);
CAF_ADD_ATOM(supervisor, get_child);
CAF_ADD_ATOM(supervisor, keep_alive);
CAF_END_TYPE_ID_BLOCK(supervisor)

// TODO: add default CTOR
struct childspec {
  std::string child_id;
  // instantiated functor with internal variables replaces erlang mfa args
  std::function<void(event_based_actor*)> start;
  caf::string_view restart;
  bool significant;
  caf::string_view shutdown;
  std::chrono::milliseconds wait_time;
  caf::string_view type;
};

// TODO: add default CTOR
struct child {
  std::string child_id;
  actor child;
  int restart_count;
  std::chrono::time_point<std::chrono::system_clock> restart_period_start;
};

struct sup_flags {
  sup_flags(caf::string_view restart_strategy,
            uint32_t restart_intensity,
            std::chrono::seconds restart_period,
            caf::string_view auto_shutdown) :
    restart_strategy(restart_strategy),
    restart_intensity(restart_intensity),
    restart_period(restart_period),
    auto_shutdown(auto_shutdown) {};

  sup_flags() : restart_strategy(type_name<one_for_one>::value),
                restart_intensity(1),
                restart_period(std::chrono::seconds(5)),
                auto_shutdown(type_name<never>::value) {};

  caf::string_view restart_strategy;
  uint32_t restart_intensity;
  std::chrono::seconds restart_period;
  caf::string_view auto_shutdown;
};

class supervisor {
 public:
  supervisor() : specs_(), children_(), flags_() {}

  // override or call by additional layer of indirection
  virtual void init(const std::vector<childspec>& specs,
                    sup_flags flags = sup_flags()) {
    specs_ = specs;
    flags_ = flags;
  }

  void operator()(event_based_actor* self)  {
    for( auto& e : specs_ ) {
      actor process = self->home_system().spawn(e.start);
      self->monitor(process);
      child just_started;
      just_started.child_id = e.child_id;
      just_started.child = std::move(process);
      just_started.restart_count = 0;
      just_started.restart_period_start = std::chrono::system_clock::now();
      children_.push_back(just_started);
    }
    self->set_down_handler([](down_msg& msg) {
        msg.source

    })
  }

 private:
  std::vector<childspec> specs_;
  std::vector<child> children_;
  sup_flags flags_;
  //behavior supervising_;
};

/* template< class ... Xs >
class supervisor: public event_based_actor {
 public:
  supervisor(actor_config& cfg,
             sup_flags flags) :
             event_based_actor(cfg),
             flags_(flags)
  { }

  supervisor(actor_config& cfg,
             caf::string_view restart_strategy,
             uint32_t intensity,
             std::chrono::seconds period,
             caf::string_view auto_shutdown) :
             event_based_actor(cfg),
             flags_(restart_strategy,
                   intensity,
                   period,
                   auto_shutdown)
  { }

  void init() {

  }

 protected:
  behavior make_behavior() override {
    supervising_.assign(
      [=](keep_alive) {
        this->delayed_send(this, std::chrono::seconds(1), keep_alive_v);
      }
    );

    delayed_send(this, std::chrono::seconds(1), keep_alive_v);
    //child = spawn();

    //this->monitor(child);
    this->set_down_handler([&](down_msg& msg) {
      //child = spawn<>();
      //this->monitor(child);
      become(supervising_);
    } );

    return supervising_;
  }

 private:
  std::vector<childspec> specs_;
  std::vector<child> children_;
  sup_flags flags_;
  behavior supervising_;
}; */
