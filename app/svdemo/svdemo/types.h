// Copyright 2022 Daniel Weber
#ifndef ACTORDEMO_TYPES_H
#define ACTORDEMO_TYPES_H

#include "caf/all.hpp"
#include "svdemo/messages.h"
#pragma once

CAF_BEGIN_TYPE_ID_BLOCK(supervisor, caf::first_custom_type_id)
CAF_ADD_TYPE_ID(supervisor, (mail));
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
// CAF_ADD_ATOM(supervisor, simple_one_for_one);
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
#endif  // ACTORDEMO_TYPES_H
