/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_PROGRAM_OPTIONS_TYPE_SECURITY_OPTION_H
#define SRC_PROGRAM_OPTIONS_TYPE_SECURITY_OPTION_H

namespace program_options
{

namespace type
{

enum class SecurityOption {
  STARTTLS,
  SSL,
  NONE
};

}

}

#endif /* SECURITY_OPTION_H */
