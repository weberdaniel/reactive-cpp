// Copyright 2022 Daniel Weber
#include "svdemo/supervisor.h"

using caf::down_msg;
using caf::exit_reason;

void one_for_one_strategy(
const std::shared_ptr<supervisor_dynamic_state>& ptr,
const down_msg& msg,
event_based_actor* self) {
  auto it = ptr->children_.begin();
  while (it != ptr->children_.end()) {
    if (msg.source == (*it).address) {
      CAF_LOG_INFO("Received Down Message from " + (*it).child_id);
      CAF_LOG_INFO("Apply one_for_one strategy");
      std::string id = (*it).child_id;
      for (auto &e : ptr->specs_) {
        if (e.child_id == id) {
          auto duration =
            std::chrono::system_clock::now().time_since_epoch();
          auto millis =
            std::chrono::duration_cast<std::chrono::milliseconds>(duration)
            .count();
          auto delta = millis -
            std::chrono::duration_cast<std::chrono::milliseconds>(
            (*it).restart_period_start.time_since_epoch()).count();

          CAF_LOG_INFO("Iterating Child " + e.child_id);
          CAF_LOG_INFO(" -- time passed (ms): " + std::to_string(delta));
          CAF_LOG_INFO(" -- restarts during time passed (#): "
            + std::to_string((*it).restart_count)
            + " (max. " + std::to_string(ptr->flags_.restart_intensity) + ")");

          // 1. reset the restart count if period expired
          if (delta > ptr->flags_.restart_period.count()) {
            CAF_LOG_INFO(" -- reset restarts during time passed " + e.child_id);
            (*it).restart_count = 0;
            (*it).restart_period_start = std::chrono::system_clock::now();
          }

          // 2. if we start a new meassurement/ the old meassurement expired,
          //    set the timer
          if ((*it).restart_count == 0) {
            CAF_LOG_INFO(" -- reset time passed " + e.child_id);
            (*it).restart_period_start = std::chrono::system_clock::now();
          }

          // 3. if we reached the maximum attempts give up
          if ((*it).restart_count == ptr->flags_.restart_intensity) {
            CAF_LOG_INFO(" -- maximum restarts reached for " + e.child_id);
            CAF_LOG_INFO(" -- shut down all children ");
            for (auto &a : ptr->children_) {
              self->send_exit(a.address, exit_reason::unknown);
              a.restart_count = 0;
              a.process = nullptr;
              a.child_id = "";
              a.address = nullptr;
            }
            ptr->children_.clear();
            CAF_LOG_INFO(" -- shut down self:");
            self->quit();
            return;
          }

          // 4. if we don't give up, do a restart
          CAF_LOG_INFO(" -- respawn child " + e.child_id);
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
}
void rest_for_one_strategy(
const std::shared_ptr<supervisor_dynamic_state>& ptr,
const down_msg& msg,
event_based_actor* self) {
  auto it = ptr->children_.begin();
  while (it != ptr->children_.end()) {
    if (msg.source == (*it).address) {
      CAF_LOG_INFO("Received Down Message from " + (*it).child_id);
      CAF_LOG_INFO("Apply rest_for_one strategy");
      std::string id = (*it).child_id;
      for (auto &e : ptr->specs_) {
        if (e.child_id == id) {
          auto duration =
          std::chrono::system_clock::now().time_since_epoch();
          auto millis =
          std::chrono::duration_cast<std::chrono::milliseconds>(duration)
          .count();
          auto delta = millis -
          std::chrono::duration_cast<std::chrono::milliseconds>(
          ptr->restart_period_start_.time_since_epoch()).count();

          CAF_LOG_INFO("Iterating Child " + e.child_id);
          CAF_LOG_INFO(" -- time passed (ms): " + std::to_string(delta));
          CAF_LOG_INFO(" -- restarts during time passed (#): "
          + std::to_string(ptr->restart_count_)
          + " (max. " + std::to_string(ptr->flags_.restart_intensity) + ")");

          // 1. reset the restart count if period expired
          if (delta > ptr->flags_.restart_period.count()) {
            CAF_LOG_INFO(" -- reset restarts during time passed " + e.child_id);
            ptr->restart_period_start_ = std::chrono::system_clock::now();
            ptr->restart_count_ = 0;
          }

          // 2. if we start a new meassurement/ the old meassurement expired,
          //    set the timer
          if (ptr->restart_count_ == 0) {
            CAF_LOG_INFO(" -- reset time passed for " + e.child_id);
            ptr->restart_period_start_ = std::chrono::system_clock::now();
          }

          // 3. if we reached the maximum attempts give up entirely
          if (ptr->restart_count_ == ptr->flags_.restart_intensity) {
            CAF_LOG_INFO(" -- maximum restarts reached for " + e.child_id);
            CAF_LOG_INFO(" -- shut down all children ");
            for (auto &a : ptr->children_) {
              self->demonitor(a.address);
              self->send_exit(a.address, exit_reason::unknown);
              a.restart_count = 0;
              a.process = nullptr;
              a.child_id = "";
              a.address = nullptr;
            }
            ptr->children_.clear();
            CAF_LOG_INFO(" -- shut down self:");
            self->quit();
            return;
          }

          // 4. if we don't give up yet, do a restart, but since this is
          // the rest_for_one, the last child will be stopped first until
          // we reach the crashed supervisor. Then we restart the crashed
          // supervisor and restart everything from left to right

          ptr->restart_count_++;

          // 4.1. find the index of the crashed child
          int index_of_crashed_child = 0;
          for (; index_of_crashed_child < ptr->children_.size();
          index_of_crashed_child++ ) {
            CAF_LOG_INFO("search");
            if (ptr->children_.at(index_of_crashed_child).child_id == id)
              break;
          }

          // 4.2. start at the end and iterate backwards to stop all children
          for (int i = ptr->children_.size()-1;
               i >= index_of_crashed_child; i--) {
            CAF_LOG_INFO(" -- demonitor " + ptr->children_.at(i).child_id);
            self->demonitor(ptr->children_.at(i).address);
            CAF_LOG_INFO(" -- send_exit " + ptr->children_.at(i).child_id);
            self->send_exit(ptr->children_.at(i).address, exit_reason::unknown);
            ptr->children_.at(i).process = nullptr;
            ptr->children_.at(i).address = nullptr;
          }

          // 4.3. start at the child and restart everything
          for (int i = index_of_crashed_child; i < ptr->children_.size(); i++) {
            for (auto& k : ptr->specs_) {
              if (k.child_id == ptr->children_.at(i).child_id) {
                CAF_LOG_INFO(" -- respawn child " +
                             ptr->children_.at(i).child_id);
                actor process = self->home_system().spawn(k.start);
                self->monitor(process);
                ptr->children_.at(i).address = process->address();
                ptr->children_.at(i).process = std::move(process);
              }
            }
          }
        }
      }
    }
    it++;
  }
}

void one_for_all_strategy(
const std::shared_ptr<supervisor_dynamic_state>& ptr,
const down_msg& msg,
event_based_actor* self) {
  auto it = ptr->children_.begin();
  while (it != ptr->children_.end()) {
    if (msg.source == (*it).address) {
      CAF_LOG_INFO("Received Down Message from " + (*it).child_id);
      CAF_LOG_INFO("Apply one_for_all strategy");
      std::string id = (*it).child_id;
      for (auto &e : ptr->specs_) {
        if (e.child_id == id) {
          auto duration =
          std::chrono::system_clock::now().time_since_epoch();
          auto millis =
          std::chrono::duration_cast<std::chrono::milliseconds>(duration)
          .count();
          auto delta = millis -
          std::chrono::duration_cast<std::chrono::milliseconds>(
          (*it).restart_period_start.time_since_epoch()).count();

          CAF_LOG_INFO("Iterating Child " + e.child_id);
          CAF_LOG_INFO(" -- time passed (ms): " + std::to_string(delta));
          CAF_LOG_INFO(" -- restarts during time passed (#): "
                       + std::to_string((*it).restart_count)
           + " (max. " + std::to_string(ptr->flags_.restart_intensity) + ")");

          // 1. reset the restart count if period expired
          if (delta > ptr->flags_.restart_period.count()) {
            CAF_LOG_INFO(" -- reset restarts during time passed " + e.child_id);
            (*it).restart_period_start = std::chrono::system_clock::now();
          }

          // 2. if we start a new meassurement/ the old meassurement expired,
          //    set the timer
          if ((*it).restart_count == 0) {
            CAF_LOG_INFO(" -- reset time passed " + e.child_id);
            (*it).restart_period_start = std::chrono::system_clock::now();
          }

          // 3. if we reached the maximum attempts give up entirely
          if ((*it).restart_count == ptr->flags_.restart_intensity) {
            CAF_LOG_INFO(" -- maximum restarts reached for " + e.child_id);
            CAF_LOG_INFO(" -- shut down all children ");
            for (auto &a : ptr->children_) {
              self->demonitor(a.address);
              self->send_exit(a.address, exit_reason::unknown);
              a.restart_count = 0;
              a.process = nullptr;
              a.child_id = "";
              a.address = nullptr;
            }
            ptr->children_.clear();
            CAF_LOG_INFO(" -- shut down self:");
            self->quit();
            return;
          }

          // 4. if we don't give up, do a restart, but since this is
          // the one_for_all, terminate all children, then restart
          // all of them

          // 4.1. find the child that crashed
          int index = 0;
          for (; index < ptr->children_.size(); index++) {
            if (ptr->children_.at(index).child_id == id) break;
          }

          // 4.2 terminate children starting from where the crash
          // happened to the sides
          int index_right = index+1;
          int index_left = index-1;
          while (index_left >= 0 || index_right != ptr->children_.size()) {
            if (index_left >= 0) {
              CAF_LOG_INFO(" -- demonitor " +
                ptr->children_.at(index_left).child_id);
              self->demonitor(ptr->children_.at(index_left).address);
              CAF_LOG_INFO(" -- send_exit " +
                ptr->children_.at(index_left).child_id);
              self->send_exit(ptr->children_.at(index_left).address,
                exit_reason::unknown);
              ptr->children_.at(index_left).process = nullptr;
              ptr->children_.at(index_left).address = nullptr;
              index_left--;
            }
            if (index_right != ptr->children_.size()) {
              CAF_LOG_INFO(" -- demonitor " +
                ptr->children_.at(index_right).child_id);
              self->demonitor(ptr->children_.at(index_right).address);
              CAF_LOG_INFO(" -- send_exit " +
                ptr->children_.at(index_right).child_id);
              self->send_exit(ptr->children_.at(index_right).address,
                exit_reason::unknown);
              ptr->children_.at(index_right).process = nullptr;
              ptr->children_.at(index_right).address = nullptr;
              index_right++;
            }
          }

          // 4.3 restart all children from left to right (0 to max)
          for (int i = 0; i < ptr->children_.size(); i++) {
            for (auto& k : ptr->specs_) {
              if (k.child_id == ptr->children_.at(i).child_id) {
                CAF_LOG_INFO(" -- respawn child " +
                ptr->children_.at(i).child_id);
                actor process = self->home_system().spawn(k.start);
                self->monitor(process);
                ptr->children_.at(i).address = process->address();
                ptr->children_.at(i).process = std::move(process);
                ptr->children_.at(i).restart_count++;
              }
            }
          }
        }
      }
    }
  it++;
  }
}

void supervisor::init(const std::vector<child_specification>& specs,
supervisor_flags flags) {
  ptr_->specs_ = specs;
  ptr_->flags_ = flags;
}

child::child(child&& copy) noexcept :
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
}

// note move to self leaves object in valid but undefined state,
// by definition of move operation. therefore move to self should
// never occur, and if it occurs it is defined to leave an undefined
// state, so it must not be handled. See Klaus Iglberger 2019, CppCon.

child& child::operator=(child&& copy) noexcept {
  destroy(process);  // clean up all visible ressources
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
    if (ptr->flags_.restart_strategy == type_name<one_for_one>::value) {
      CAF_LOG_INFO("Apply One for One");
      one_for_one_strategy(ptr, msg, self);
    } else if (ptr->flags_.restart_strategy == type_name<one_for_all>::value) {
      CAF_LOG_INFO("Apply One for All");
      one_for_all_strategy(ptr, msg, self);
    } else if (ptr->flags_.restart_strategy == type_name<rest_for_one>::value) {
      CAF_LOG_INFO("Apply Rest for One");
      rest_for_one_strategy(ptr, msg, self);
    }
  });

  // add one to the child counter for each child
  auto fptr = self->home_system().metrics().gauge_family("supervisor",
  "children",{{"wtf","bro"}},"helptext", "ms", false);
  auto count = fptr->get_or_add({{"method", "put"}});

  CAF_LOG_INFO("Supervisor Start Children: ");
  for ( auto& e : ptr_->specs_ ) {
    count->inc();
    CAF_LOG_INFO("Supervisor Start Child: " + e.child_id);
    actor child_actor = self->home_system().spawn(e.start);
    self->monitor(child_actor);
    child child;
    child.child_id = e.child_id;
    child.address = child_actor.address();
    child.process = std::move(child_actor);
    child.restart_count = 0;
    child.restart_period_start = std::chrono::system_clock::now();
    ptr_->children_.push_back(std::move(child));
  }
  CAF_LOG_INFO("Supervisor enters Idle state");
  self->become([self, ptr](int msg){
    CAF_LOG_INFO("Supervisor receives keep_alive");
    self->delayed_send(self, std::chrono::seconds(3), 3);
  });
  self->delayed_send(self, std::chrono::seconds(3), 3);
}
