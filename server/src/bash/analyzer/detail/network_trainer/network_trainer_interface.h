/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <memory>

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace network_trainer
{

class NetworkTrainerInterface {
 public:
  virtual ~NetworkTrainerInterface() = default;

  virtual void Train() = 0;
};

typedef std::shared_ptr<NetworkTrainerInterface> NetworkTrainerInterfacePtr;

}

}

}

}
