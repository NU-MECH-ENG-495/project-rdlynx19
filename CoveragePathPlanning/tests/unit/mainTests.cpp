#include <gtest/gtest.h>
#include <coveragePP.h>
#include <vector>
#include <thread>
#include <mutex>

// Test fixture for CoveragePathPlanner
class CoveragePathPlannerTest : public ::testing::Test
{
protected:
    CoveragePathPlanner cpp;
    double area_width = 5.0;
    double area_height = 5.0;
    double search_radius = 0.5;
    double z = 2.0;         // Fixed altitude for all drones
    double step_size = 0.5; // Distance between intermediate points
    bool generate_intermediate_points = false;
};

// Test case for generateBackAndForthPath
TEST_F(CoveragePathPlannerTest, GenerateBackAndForthPathTest)
{
    double x_min = 0.0;
    double x_max = 2.5;
    double y_min = 0.0;
    double y_max = 2.5;

    auto path = cpp.generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);

    // Check if the path is not empty
    ASSERT_FALSE(path.empty());

    // Check if the path contains valid points
    for (const auto &point : path)
    {
        EXPECT_GE(point.x, x_min);
        EXPECT_LE(point.x, x_max);
        EXPECT_GE(point.y, y_min);
        EXPECT_LE(point.y, y_max);
        EXPECT_EQ(point.z, z);
    }
}

// Test case for writeWaypointsToCSV
TEST_F(CoveragePathPlannerTest, WriteWaypointsToCSVTest)
{
    std::vector<std::vector<Point>> paths = {
        {{0.0, 0.0, 2.0}, {1.0, 0.0, 2.0}, {2.0, 0.0, 2.0}},
        {{0.0, 1.0, 2.0}, {1.0, 1.0, 2.0}, {2.0, 1.0, 2.0}}};

    std::string filename = "test_waypoints.csv";
    cpp.writeWaypointsToCSV(paths, filename);

    // Check if the file was created
    std::ifstream file(filename);
    ASSERT_TRUE(file.good());

    // Read the file and check its contents
    std::string line;
    std::getline(file, line);
    EXPECT_FALSE(line.empty());

    file.close();
}

// Test case for multi-threaded path generation
TEST_F(CoveragePathPlannerTest, MultiThreadedPathGenerationTest)
{
    double overlap_margin = search_radius / 2;
    std::vector<std::vector<double>> subregions = {
        {0.0, (area_width / 2.0) + overlap_margin, 0.0, (area_height / 2.0) + overlap_margin},
        {(area_width / 2.0) - overlap_margin, area_width, 0.0, (area_height / 2.0) + overlap_margin},
        {0.0, (area_width / 2.0) + overlap_margin, (area_height / 2.0) - overlap_margin, area_height},
        {(area_width / 2.0) - overlap_margin, area_width, (area_height / 2.0) - overlap_margin, area_height}};

    std::vector<std::vector<Point>> paths;
    std::mutex paths_mutex;
    std::vector<std::thread> threads;

    for (const auto &subregion : subregions)
    {
        threads.emplace_back([&, subregion]()
                             {
            double x_min = subregion[0];
            double x_max = subregion[1];
            double y_min = subregion[2];
            double y_max = subregion[3];
            auto path = cpp.generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);
            std::lock_guard<std::mutex> lock(paths_mutex);
            paths.push_back(path); });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    // Check if paths were generated for all subregions
    ASSERT_EQ(paths.size(), subregions.size());

    // Check if each path is valid
    for (const auto &path : paths)
    {
        ASSERT_FALSE(path.empty());
    }
}

// int main(int argc, char **argv)
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }