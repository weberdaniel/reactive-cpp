#include "supervisor.h"

void supervisor::init(const std::vector<child_specification>& specs,
supervisor_flags flags) {
    ptr_->specs_ = specs;
    ptr_->flags_ = flags;
}

void supervisor::operator()(event_based_actor* self)  {
    for( auto& e : ptr_->specs_ ) {
        actor child_actor= self->home_system().spawn(e.start);
        self->monitor(child_actor);
        child child;
        child.child_id = e.child_id;
        child.address = child_actor.address();
        child.process= std::move(child_actor);
        child.restart_count = 0;
        child.restart_period_start = std::chrono::system_clock::now();
        ptr_->children_.push_back(child);
    };
    std::shared_ptr<supervisor_dynamic_state> ptr = ptr_;
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
