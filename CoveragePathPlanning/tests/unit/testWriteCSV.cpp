#include <gtest/gtest.h>
#include "coveragePP.h"
#include <fstream>
#include <sstream>

TEST(WriteWaypointsToCSVTest, BasicTest)
{
    std::vector<std::vector<Point>> paths = {
        {{0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}},
        {{2.0, 2.0, 1.0}, {3.0, 3.0, 1.0}}};
    std::string filename = "test_output.csv";

    writeWaypointsToCSV(paths, filename);

    // Read the file and verify its contents
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line); // Skip header
    EXPECT_EQ(line, "DroneID,X,Y,Z");

    std::getline(file, line);
    EXPECT_EQ(line, "0,0,0,1");

    std::getline(file, line);
    EXPECT_EQ(line, "0,1,1,1");

    std::getline(file, line);
    EXPECT_EQ(line, "1,2,2,1");

    std::getline(file, line);
    EXPECT_EQ(line, "1,3,3,1");

    file.close();
}

TEST(WriteWaypointsToCSVTest, EmptyPathsTest)
{
    std::vector<std::vector<Point>> paths;
    std::string filename = "test_empty_output.csv";

    writeWaypointsToCSV(paths, filename);

    // Read the file and verify its contents
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line); // Skip header
    EXPECT_EQ(line, "DroneID,X,Y,Z");

    // No data lines should be present
    EXPECT_FALSE(std::getline(file, line));

    file.close();
}