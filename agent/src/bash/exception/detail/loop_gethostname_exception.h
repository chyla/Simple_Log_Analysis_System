#ifndef LOOP_GETHOSTNAME_EXCEPTION_H
#define LOOP_GETHOSTNAME_EXCEPTION_H

namespace bash
{

namespace exception
{

namespace detail
{

class LoopGethostnameException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* LoopGethostnameException::what() const throw () {
  return "Gethostname failed.";
}

}

}

}

#endif /* LOOP_GETHOSTNAME_EXCEPTION_H */
