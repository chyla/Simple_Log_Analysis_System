/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <string>
#include <vector>

namespace database
{

namespace type
{

enum class Classification
{
  UNKNOWN,
  NORMAL,
  ANOMALY
};

}

}

#endif /* CLASSIFICATION_H */
