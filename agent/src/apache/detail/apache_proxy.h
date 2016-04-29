#ifndef APACHE_PROXY_H
#define APACHE_PROXY_H

#include <patlms/type/apache_log_entry.h>
#include <patlms/dbus/proxy_object.h>
#include <patlms/dbus/detail/bus_interface.h>
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


}

}

#endif /* APACHE_PROXY_H */
