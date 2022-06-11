#include "supervisor.h"

void supervisor::init(const std::vector<child_specification>& specs,
supervisor_flags flags) {
  ptr_->specs_ = specs;
  ptr_->flags_ = flags;
}
child::child( child&& copy) noexcept :
  // transfer copy content into this
  child_id(std::move(copy.child_id)),
  address(std::move(copy.address)),
  process(std::move(copy.process)),
  restart_count(std::move(copy.restart_count)),
  restart_period_start(std::move(copy.restart_period_start)) {
    // leave copy in valid but undefined state
    copy.child_id = "";
    copy.address = actor_addr();
    copy.process = nullptr;
    copy.restart_count = -1;
    copy.restart_period_start =
      std::chrono::time_point<std::chrono::system_clock>::min();
};

// note move to self leaves object in valid but undefined state,
// by definition of move operation. therefore move to self should
// never occur, and if it occurs it is defined to leave an undefined
// state, so it must not be handled. See Klaus Iglberger 2019, CppCon.

child& child::operator=(child&& copy) noexcept {
  destroy(process); // clean up all visible ressources
  child_id = std::move(copy.child_id);
  address = std::move(copy.address);
  process = std::move(copy.process);
  restart_count = std::move(copy.restart_count);
  restart_period_start = std::move(copy.restart_period_start);
  copy.child_id = "";
  copy.address = actor_addr();
  copy.process = nullptr;
  copy.restart_count = -1;
  copy.restart_period_start =
    std::chrono::time_point<std::chrono::system_clock>::min();
  return *this;
}

void supervisor::operator()(event_based_actor* self)  {
  CAF_LOG_INFO("Supervisor Start");
  CAF_LOG_INFO("Supervisor Configure Down Handler");
  std::shared_ptr<supervisor_dynamic_state> ptr = ptr_;
  self->set_down_handler([self, ptr](down_msg& msg) {
    auto it = ptr->children_.begin();
    while (it != ptr->children_.end()) {
      if (msg.source == (*it).address) {
        CAF_LOG_INFO("Received Down Message " + (*it).child_id);
        std::string id = (*it).child_id;
        for( auto& e : ptr->specs_ ) {
          if( e.child_id == id ) {

            // 1. reset the restart count if period expired
            std::chrono::nanoseconds delta = std::chrono::system_clock::now() -
            (*it).restart_period_start;
              CAF_LOG_INFO("           -- delta : " + std::to_string(delta.count()));
              if (duration_cast<std::chrono::seconds>(delta) > ptr->flags_.restart_period) {
               CAF_LOG_INFO("Supervisor Reset Restart Count: ");
               CAF_LOG_INFO("           -- name : " + e.child_id);
               CAF_LOG_INFO("           -- delta : " + std::to_string(delta.count()));
               (*it).restart_count = 0;
               (*it).restart_period_start = std::chrono::system_clock::now();
               return;
            }

            // 2. if we start a new meassurement, set the timer
            if ((*it).restart_count == 0) {
              CAF_LOG_INFO("Set the Start Time");
              (*it).restart_period_start = std::chrono::system_clock::now();
            }

            // 3. if we reached the maximum attempts give up
            if ((*it).restart_count == ptr->flags_.restart_intensity) {
              CAF_LOG_INFO("Maximum Restart has been reached.");
              CAF_LOG_INFO("Shut down all children:");
              for( auto& a: ptr->specs_) {
                self->send_exit((*it).process.address(), exit_reason::unknown);
              }
              CAF_LOG_INFO("Shut down self:");
              self->quit();
              return;
            }

            // 4. if we don't give up yet, do a restart
            CAF_LOG_INFO("Respawn Child: " + e.child_id);
            actor process = self->home_system().spawn(e.start);
            self->monitor(process);
            (*it).address = process->address();
            (*it).process = std::move(process);
            (*it).restart_count++;
            CAF_LOG_INFO("Set the Start Time");
            (*it).restart_period_start = std::chrono::system_clock::now();
          }
        }
      }
    it++;
    }
  });
  CAF_LOG_INFO("Supervisor Start Children: ");
  for( auto& e : ptr_->specs_ ) {
    CAF_LOG_INFO("Supervisor Start Child: " + e.child_id);
    actor child_actor= self->home_system().spawn(e.start);
    self->monitor(child_actor);
    child child;
    child.child_id = e.child_id;
    child.address = child_actor.address();
    child.process= std::move(child_actor);
    child.restart_count = 0;
    child.restart_period_start = std::chrono::system_clock::now();
    ptr_->children_.push_back(std::move(child));
  };
  CAF_LOG_INFO("Supervisor enters Idle state");
  self->become([self,ptr](int msg){
    CAF_LOG_INFO("Supervisor receives keep_alive");
    self->delayed_send(self,std::chrono::seconds(3),3);
  });
  self->delayed_send(self,std::chrono::seconds(3),3);
}
