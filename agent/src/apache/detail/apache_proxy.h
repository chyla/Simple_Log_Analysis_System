#pragma once

#include <slas/type/apache_log_entry.h>
#include <slas/dbus/proxy_object.h>
#include <slas/dbus/detail/bus_interface.h>
#include <memory>

namespace apache
{

namespace detail
{

class ApacheProxy : public dbus::ProxyObject {
 public:
  ApacheProxy(std::shared_ptr<::dbus::detail::BusInterface> bus);

  bool AddLogEntry(const type::ApacheLogEntry &log_entry);

 private:
  std::shared_ptr<::dbus::detail::BusInterface> bus_;
};

typedef std::shared_ptr<ApacheProxy> ApacheProxyPtr;

}

}
