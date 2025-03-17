#include <gtest/gtest.h>
#include "coveragePP.h"

TEST(GenerateIntermediatePointsTest, BasicTest)
{
    Point start = {0.0, 0.0, 0.0};
    Point end = {1.0, 1.0, 1.0};
    double step_size = 0.5;
    auto points = generateIntermediatePoints(start, end, step_size);

    ASSERT_EQ(points.size(), 3); // Expected number of points
    EXPECT_DOUBLE_EQ(points[0].x, 0.0);
    EXPECT_DOUBLE_EQ(points[0].y, 0.0);
    EXPECT_DOUBLE_EQ(points[0].z, 0.0);
    EXPECT_DOUBLE_EQ(points[1].x, 0.5);
    EXPECT_DOUBLE_EQ(points[1].y, 0.5);
    EXPECT_DOUBLE_EQ(points[1].z, 0.5);
    EXPECT_DOUBLE_EQ(points[2].x, 1.0);
    EXPECT_DOUBLE_EQ(points[2].y, 1.0);
    EXPECT_DOUBLE_EQ(points[2].z, 1.0);
}

TEST(GenerateIntermediatePointsTest, ZeroDistanceTest)
{
    Point start = {1.0, 1.0, 1.0};
    Point end = {1.0, 1.0, 1.0};
    double step_size = 0.5;
    auto points = generateIntermediatePoints(start, end, step_size);

    ASSERT_EQ(points.size(), 1); // Only the start/end point should be generated
    EXPECT_DOUBLE_EQ(points[0].x, 1.0);
    EXPECT_DOUBLE_EQ(points[0].y, 1.0);
    EXPECT_DOUBLE_EQ(points[0].z, 1.0);
}

TEST(GenerateIntermediatePointsTest, LargeStepSizeTest)
{
    Point start = {0.0, 0.0, 0.0};
    Point end = {1.0, 1.0, 1.0};
    double step_size = 2.0; // Step size larger than the distance
    auto points = generateIntermediatePoints(start, end, step_size);

    ASSERT_EQ(points.size(), 2); // Only start and end points should be generated
    EXPECT_DOUBLE_EQ(points[0].x, 0.0);
    EXPECT_DOUBLE_EQ(points[1].x, 1.0);
}
