#ifndef COVERAGEPP_H
#define COVERAGEPP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <vector>
#include <point.h>

class CoveragePathPlanner
{
public:
    std::vector<Point> generateIntermediatePoints(Point start, Point end, double step_size);
    std::vector<Point> generateBackAndForthPath(double x_min, double x_max, double y_min, double y_max, double z, double search_radius, double step_size, bool generate_intermediate_points);
    void writeWaypointsToCSV(const std::vector<std::vector<Point>> &paths, const std::string &filename);
};

#endif