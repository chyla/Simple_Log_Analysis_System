/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "nearest_neighbours_table_interface.h"

#include <vector>

namespace apache
{

namespace analyzer
{

namespace detail
{

namespace prepare_statistics
{

class NearestNeighboursTable : public NearestNeighboursTableInterface {
 public:
  NearestNeighboursTable();
  virtual ~NearestNeighboursTable() = default;

  void SetSession(const ::apache::type::ApacheSessionEntry &session) override;

  void Add(const ::apache::type::ApacheSessionEntry &session) override;
  const Neighbours& Get() override;

  void Clear() override;

 private:
  int number_of_neighbours_;

  ::apache::type::ApacheSessionEntry original_session_;
  Neighbours nearest_neighbours_;

  double Distance(const ::apache::type::ApacheSessionEntry &a,
                  const ::apache::type::ApacheSessionEntry &b) const;
};

}

}

}

}
