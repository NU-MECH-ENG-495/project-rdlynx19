#include <gtest/gtest.h>
#include "coveragePP.h"

TEST(GenerateBackAndForthPathTest, BasicTest)
{
    double x_min = 0.0, x_max = 2.0, y_min = 0.0, y_max = 2.0;
    double z = 1.0, search_radius = 0.5, step_size = 0.5;
    bool generate_intermediate_points = false;

    auto path = generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);

    // Check the number of waypoints
    ASSERT_GT(path.size(), 0);

    // Check the first and last points
    EXPECT_DOUBLE_EQ(path[0].x, x_min);
    EXPECT_DOUBLE_EQ(path[0].y, y_min);
    EXPECT_DOUBLE_EQ(path[0].z, z);
    EXPECT_DOUBLE_EQ(path.back().x, x_max);
    EXPECT_DOUBLE_EQ(path.back().y, y_max);
    EXPECT_DOUBLE_EQ(path.back().z, z);
}

TEST(GenerateBackAndForthPathTest, SmallSubregionTest)
{
    double x_min = 0.0, x_max = 0.5, y_min = 0.0, y_max = 0.5;
    double z = 1.0, search_radius = 0.5, step_size = 0.5;
    bool generate_intermediate_points = false;

    auto path = generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);

    // Check the number of waypoints
    ASSERT_GT(path.size(), 0);

    // Check the first and last points
    EXPECT_DOUBLE_EQ(path[0].x, x_min);
    EXPECT_DOUBLE_EQ(path[0].y, y_min);
    EXPECT_DOUBLE_EQ(path[0].z, z);
    EXPECT_DOUBLE_EQ(path.back().x, x_max);
    EXPECT_DOUBLE_EQ(path.back().y, y_max);
    EXPECT_DOUBLE_EQ(path.back().z, z);
}

TEST(GenerateBackAndForthPathTest, IntermediatePointsTest)
{
    double x_min = 0.0, x_max = 2.0, y_min = 0.0, y_max = 2.0;
    double z = 1.0, search_radius = 0.5, step_size = 0.5;
    bool generate_intermediate_points = true;

    auto path = generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);

    // Check that intermediate points are generated
    ASSERT_GT(path.size(), 4); // More points than just the start and end
}