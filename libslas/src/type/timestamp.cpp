#include <slas/type/timestamp.h>

namespace type
{

std::ostream& operator<<(std::ostream& os, const type::Timestamp &timestamp) {
  os << timestamp.GetDate() << " " << timestamp.GetTime();
  return os;
}

}
