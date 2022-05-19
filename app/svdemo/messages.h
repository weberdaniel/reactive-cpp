//
#ifndef ACTORDEMO_MESSAGES_H
#define ACTORDEMO_MESSAGES_H
#pragma once

#include <string>
#include "caf/all.hpp"

using namespace caf;

struct mail {
  uint32_t source {0}; // 0 == invalid source
  uint32_t destination {0}; // 0 == invalid destination
  std::string content;
};

template <class Inspector>
bool inspect(Inspector& f, mail x) {
    return f.object(x).fields(
            f.field("source", x.source),
            f.field("destination", x.destination),
            f.field("content", x.content));
}


#endif //ACTORDEMO_MESSAGES_H
