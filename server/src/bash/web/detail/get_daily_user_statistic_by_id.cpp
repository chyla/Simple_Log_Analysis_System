/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_daily_user_statistic_by_id.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>

#include <slas/type/date.h>

using namespace std;
using namespace nlohmann;

namespace bash
{

namespace web
{

namespace detail
{

GetDailyUserStatisticByIdPtr GetDailyUserStatisticById::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetDailyUserStatisticByIdPtr(new GetDailyUserStatisticById(scripts));
}

::web::type::Command GetDailyUserStatisticById::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserStatisticById::GetCommandName: Function call";

  return "bash.get_daily_user_statistic_by_id";
}

::web::type::JsonMessage GetDailyUserStatisticById::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserStatisticById::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto s = scripts_->GetDailyUserStatisticById(args.at(0));

  json j, t;
  t["id"] = s.id;
  t["agent_name"] = s.agent_name;
  t["uid"] = s.uid;
  t["date"] = s.date.ToString();
  t["classification"] = static_cast<int> (s.classification);

  j["status"] = "ok";
  j["result"] = t;

  return j.dump();
}

GetDailyUserStatisticById::GetDailyUserStatisticById(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
