#include <iostream>
#include "types.h"
#pragma once

using namespace caf;

struct childspec {
  childspec() : start(), restart(type_name<transient>::value),
    significant(false), shutdown(type_name<wait>::value),
    wait_time(0), type(type_name<work>::value) { }

  std::string child_id;
  // instantiated functor with internal variables replaces erlang mfa args
  std::function<void(event_based_actor*)> start;
  caf::string_view restart;
  bool significant;
  caf::string_view shutdown;
  std::chrono::milliseconds wait_time;
  caf::string_view type;
};

struct child {
  child() : child_id(), address(), restart_count(0),
            restart_period_start(std::chrono::system_clock::time_point::min()) {}

  std::string child_id;
  actor_addr address;
  actor process;
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

struct supervisor_state {
  std::vector<childspec> specs_;
  std::vector<child> children_;
  sup_flags flags_;
};

class supervisor {
 public:
  supervisor()   {
      ptr_ = std::make_shared<supervisor_state>();
  }

  // override or call by additional layer of indirection
  virtual void init(const std::vector<childspec>& specs,
  sup_flags flags = sup_flags()) {
    ptr_->specs_ = specs;
    ptr_->flags_ = flags;
  }

  void operator()(event_based_actor* self)  {
    for( auto& e : ptr_->specs_ ) {
      actor process = self->home_system().spawn(e.start);
      self->monitor(process);
      child just_started;
      just_started.child_id = e.child_id;
      just_started.address = process.address();
      just_started.process= std::move(process);
      just_started.restart_count = 0;
      just_started.restart_period_start = std::chrono::system_clock::now();
      ptr_->children_.push_back(just_started);
    };
    std::shared_ptr<supervisor_state> ptr = ptr_;
    self->set_down_handler([self, ptr](down_msg& msg) {
        aout(self) << "received down \n";
        auto it = ptr->children_.begin();
        while (it != ptr->children_.end()) {
            if (msg.source == (*it).address) {
              std::string id = (*it).child_id;
              for( auto& e : ptr->specs_ ) {
                if( e.child_id == id ) {
                    // 1. if meassurement interval experied, reset everything
                    auto delta = std::chrono::system_clock::now() -
                                            (*it).restart_period_start;
                    if (delta > ptr->flags_.restart_period) {
                      (*it).restart_count = 0;
                    }

                    // 2. if we start a new meassurement, set the timer
                    if ((*it).restart_count == 0) {
                      (*it).restart_period_start = std::chrono::system_clock::now();
                    }

                    // 3. if we reached the maximum attempts give up
                    if ((*it).restart_count == ptr->flags_.restart_intensity) {
                    }

                    // 4. if we don't give up yet, do a restart
                    aout(self) << "respawning \n";
                    actor process = self->home_system().spawn(e.start);
                    self->monitor(process);
                    (*it).address = process->address();
                    (*it).process = std::move(process);
                    (*it).restart_count++;
                }
              }
            }
            it++;
        }
    });
    self->become([self,ptr](int msg){
        aout(self) << "supervisor receives keep alive\n";
        self->delayed_send(self,std::chrono::seconds(3),3);
    });
    self->delayed_send(self,std::chrono::seconds(3),3);
  }

  std::shared_ptr<supervisor_state> ptr_;
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
