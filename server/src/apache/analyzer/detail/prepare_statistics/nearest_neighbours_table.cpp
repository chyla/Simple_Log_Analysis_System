/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "nearest_neighbours_table.h"

#include <algorithm>
#include <boost/log/trivial.hpp>

using namespace std;

namespace apache
{

namespace analyzer
{

namespace detail
{

namespace prepare_statistics
{

NearestNeighboursTable::NearestNeighboursTable() :
number_of_neighbours_(3) {
  nearest_neighbours_.reserve(number_of_neighbours_ + 1);
}

void NearestNeighboursTable::SetSession(const ::apache::type::ApacheSessionEntry &session) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::SetSession: Function call";
  original_session_ = session;
  Clear();
}

void NearestNeighboursTable::Add(const ::apache::type::ApacheSessionEntry &session) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Add: Function call";

  auto it = find_if(nearest_neighbours_.begin(), nearest_neighbours_.end(), [&session](const Neighbour & n) {
    return n.session_id == session.id;
  });
  if (it != nearest_neighbours_.end())
    return;

  Neighbour n;
  n.distance = Distance(session, original_session_);
  n.classification = session.classification;
  n.session_id = session.id;

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Add: Adding neighbour where distance=" << n.distance << "; classification=" << static_cast<int> (n.classification) << "; session_id=" << n.session_id;

  nearest_neighbours_.push_back(n);

  sort(nearest_neighbours_.begin(), nearest_neighbours_.end(),
       [](const Neighbour &a, const Neighbour & b) {
         return a.distance < b.distance;
       });

  if (nearest_neighbours_.size() > static_cast<unsigned> (number_of_neighbours_)) {
    auto position_to_erase = nearest_neighbours_.end() - 1;
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Add: Removing item with id=" << position_to_erase->session_id;
    nearest_neighbours_.erase(position_to_erase);
  }

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Add: New table size " << nearest_neighbours_.size();
}

const Neighbours& NearestNeighboursTable::Get() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Get: Function call";

  return nearest_neighbours_;
}

void NearestNeighboursTable::Clear() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Clear: Function call";

  nearest_neighbours_.clear();
}

double NearestNeighboursTable::Distance(const ::apache::type::ApacheSessionEntry &a,
                                        const ::apache::type::ApacheSessionEntry &b) const {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Distance: Function call";

  double d = sqrt(pow(a.session_length - b.session_length, 2) +
                  pow(a.bandwidth_usage - b.bandwidth_usage, 2) +
                  pow(a.requests_count - b.requests_count, 2) +
                  pow(a.error_percentage - b.error_percentage, 2)
                  );

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::prepare_statistics::NearestNeighboursTable::Distance: Distance between session id=" << a.id << "; id2=" << b.id << " is " << d;

  return d;
}

}

}

}

}
