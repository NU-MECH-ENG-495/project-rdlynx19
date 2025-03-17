#include <gtest/gtest.h>
#include "coveragePP.h"

TEST(MainFunctionalityTest, SubregionPathGenerationTest)
{
    double area_width = 5.0, area_height = 5.0;
    double search_radius = 0.5, z = 2.0, step_size = 0.5;
    bool generate_intermediate_points = false;

    std::vector<std::vector<double>> subregions = {
        {0.0, 2.5, 0.0, 2.5},
        {2.5, 5.0, 0.0, 2.5},
        {0.0, 2.5, 2.5, 5.0},
        {2.5, 5.0, 2.5, 5.0}};

    std::vector<std::vector<Point>> paths;
    for (const auto &subregion : subregions)
    {
        double x_min = subregion[0], x_max = subregion[1];
        double y_min = subregion[2], y_max = subregion[3];
        auto path = generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);
        paths.push_back(path);
    }

    // Verify that paths are generated for all subregions
    ASSERT_EQ(paths.size(), subregions.size());

    // Verify that each path is non-empty
    for (const auto &path : paths)
    {
        ASSERT_GT(path.size(), 0);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}