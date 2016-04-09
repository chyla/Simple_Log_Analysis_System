/*
 * Copyright 2014-2015 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "../config.h"

#if defined(HAVE_GTEST) && defined(HAVE_GMOCK)

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

#else

#error Cannot make tests. Google Test or Google Mock library not found.

#endif // HAVE_BOOST_UNIT_TEST_FRAMEWORK

