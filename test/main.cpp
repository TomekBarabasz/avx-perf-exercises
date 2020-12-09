//
// Created by barabasz on 09.12.2020.
//

#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
