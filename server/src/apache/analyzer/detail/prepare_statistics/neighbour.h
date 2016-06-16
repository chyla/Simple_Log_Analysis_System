/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef NEIGHBOUR_H
#define NEIGHBOUR_H

#include <vector>
#include <cmath>
#include <limits>

#include "src/database/type/row_id.h"

namespace apache
{

namespace analyzer
{

namespace detail
{

namespace prepare_statistics
{

struct Neighbour {
  double distance;
  bool is_session_anomaly;
  ::database::type::RowId session_id;

  inline bool operator==(const Neighbour& oth) const;
};

typedef std::vector<Neighbour> Neighbours;

bool Neighbour::operator==(const Neighbour& oth) const {
  return (abs(distance - oth.distance) < std::numeric_limits<double>::epsilon()) &&
    is_session_anomaly == oth.is_session_anomaly &&
    session_id == session_id;
}

}

}

}

}

#endif /* NEIGHBOUR_H */
