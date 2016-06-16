/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef NEAREST_NEIGHBOURS_TABLE_INTERFACE_H
#define NEAREST_NEIGHBOURS_TABLE_INTERFACE_H

#include <src/apache/type/apache_session_entry.h>

#include "neighbour.h"

namespace apache
{

namespace analyzer
{

namespace detail
{

namespace prepare_statistics
{

class NearestNeighboursTableInterface {
 public:
  virtual ~NearestNeighboursTableInterface() = default;

  virtual void SetSession(const ::apache::type::ApacheSessionEntry &session) = 0;

  virtual void Add(const ::apache::type::ApacheSessionEntry &session) = 0;
  virtual const Neighbours& Get() = 0;

  virtual void Clear() = 0;
};

}

}

}

}

#endif /* NEAREST_NEIGHBOURS_TABLE_INTERFACE_H */
