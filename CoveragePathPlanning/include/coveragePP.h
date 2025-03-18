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

/**
 * @class CoveragePathPlanner
 * @brief A class for generating coverage paths for drones in a given area.
 *
 * This class provides methods to generate intermediate points between waypoints, create back-and-forth
 * coverage paths for subregions, and write the generated waypoints to a CSV file.
 */
class CoveragePathPlanner
{
public:
    /**
     * @brief Generates intermediate points between two waypoints.
     *
     * This function calculates intermediate points between a start and end point using linear interpolation.
     * The number of points is determined by the step size.
     *
     * @param start The starting point.
     * @param end The ending point.
     * @param step_size The distance between consecutive intermediate points.
     * @return std::vector<Point> A vector of intermediate points.
     * @throws std::invalid_argument If the step size is zero.
     */
    std::vector<Point> generateIntermediatePoints(Point start, Point end, double step_size);

    /**
     * @brief Generates a back-and-forth path for a subregion.
     *
     * This function generates a coverage path for a rectangular subregion in a back-and-forth pattern.
     * The path can optionally include intermediate points between waypoints.
     *
     * @param x_min The minimum x-coordinate of the subregion.
     * @param x_max The maximum x-coordinate of the subregion.
     * @param y_min The minimum y-coordinate of the subregion.
     * @param y_max The maximum y-coordinate of the subregion.
     * @param z The fixed z-coordinate for the path.
     * @param search_radius The radius of the search area, used to determine spacing between rows.
     * @param step_size The distance between consecutive intermediate points.
     * @param generate_intermediate_points If true, intermediate points are generated between waypoints.
     * @return std::vector<Point> A vector of points representing the path.
     * @throws std::invalid_argument If the step size is zero or if the subregion boundaries are invalid.
     */
    std::vector<Point> generateBackAndForthPath(double x_min, double x_max, double y_min, double y_max, double z, double search_radius, double step_size, bool generate_intermediate_points);

    /**
     * @brief Writes waypoints to a CSV file.
     *
     * This function writes the waypoints for multiple drones to a CSV file. Each row in the file
     * represents a waypoint and includes the drone ID and its coordinates (X, Y, Z).
     *
     * @param paths A vector of paths, where each path corresponds to a drone.
     * @param filename The name of the CSV file to write.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void writeWaypointsToCSV(const std::vector<std::vector<Point>> &paths, const std::string &filename);
};

#endif