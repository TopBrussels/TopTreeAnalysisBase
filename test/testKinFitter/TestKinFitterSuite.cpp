/* 
 * File:   TestKinFitterSuite
 * Author: iihe
 *
 * Created on Aug 11, 2016, 1:03:00 AM
 */

#include <gtest/gtest.h>

class KinFitterTestSuite : public testing::Test {
protected:
};

TEST_F(KinFitterTestSuite, testExample) {
    FAIL();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}