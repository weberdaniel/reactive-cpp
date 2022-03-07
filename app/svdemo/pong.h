//
#ifndef ACTORDEMO_PONG_H
#define ACTORDEMO_PONG_H

#include <iostream>
#include "caf/all.hpp"

using namespace caf;

behavior pong(event_based_actor* self) {
  return {
    [=](const std::string& what) -> std::string {
      aout(self) << what << std::endl;
      return std::string{what.rbegin(), what.rend()};
    }
  };
}

#endif //ACTORDEMO_PONG_H
