#include <gtest/gtest.h>
#include "cityDefinition.hpp"

class CityTest : public::testing::Test {
    protected:
        City testSkeleton;

        CityTest(): testSkeleton(2) {}

        void SetUp() override {
            testSkeleton.setCoordinates(4.0, 3.0, 0.2);            
        }

        void TearDown() override {

        }
};

TEST_F(CityTest, GetCoordinatesReturnsCorrectCoordinaes) {
    EXPECT_EQ(typeid(testSkeleton.getCoordinates()), typeid(std::tuple<double, double, double>) );
}

