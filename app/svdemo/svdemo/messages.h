// Copyright 2022 Daniel Weber
#ifndef ACTORDEMO_MESSAGES_H
#define ACTORDEMO_MESSAGES_H
#pragma once

#include <string>
#include "caf/all.hpp"

// this is a message from source to destination.

struct mail {
  uint32_t source {0};  // 0 == invalid source
  uint32_t destination {0};  // 0 == invalid destination
  std::string content;
};

// this function will be serializing the data inside
// the mail structure, it will be automatically be
// called by the c++ actor framework

template <class Inspector>
bool inspect(Inspector& f, mail x) {  // NOLINT
    return f.object(x).fields(
            f.field("source", x.source),
            f.field("destination", x.destination),
            f.field("content", x.content));
}


#endif  // ACTORDEMO_MESSAGES_H
