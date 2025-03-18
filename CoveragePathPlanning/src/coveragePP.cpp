#include <coveragePP.h>

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
std::vector<Point> CoveragePathPlanner::generateIntermediatePoints(Point start, Point end, double step_size)
{
    if (step_size == 0.0)
    {
        throw std::invalid_argument("Step size cannot be zero");
    }
    std::vector<Point> points;
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double dz = end.z - start.z;
    double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    int num_steps = static_cast<int>(distance / step_size);

    for (int i = 0; i <= num_steps; ++i)
    {
        double t = static_cast<double>(i) / num_steps;
        Point p;
        p.x = start.x + t * dx;
        p.y = start.y + t * dy;
        p.z = start.z + t * dz;
        points.push_back(p);
    }

    return points;
}

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
std::vector<Point> CoveragePathPlanner::generateBackAndForthPath(double x_min, double x_max, double y_min, double y_max, double z, double search_radius, double step_size, bool generate_intermediate_points)
{
    if (step_size == 0.0)
    {
        throw std::invalid_argument("Step size cannot be zero");
    }
    if (x_min >= x_max || y_min >= y_max)
    {
        throw std::invalid_argument("Invalid subregion boundaries");
    }
    std::vector<Point> path;
    double spacing = 2 * search_radius;
    double y = y_min;

    while (y <= y_max)
    {
        if (fmod(y - y_min, 2 * spacing) < 1e-6)
        {
            // Move right
            Point start = {x_min, y, z};
            Point end = {x_max, y, z};
            if (generate_intermediate_points)
            {
                auto intermediate = generateIntermediatePoints(start, end, step_size);
                path.insert(path.end(), intermediate.begin(), intermediate.end());
            }
            else
            {
                path.push_back(start);
                path.push_back(end);
            }
        }
        else
        {
            // Move left
            Point start = {x_max, y, z};
            Point end = {x_min, y, z};
            if (generate_intermediate_points)
            {
                auto intermediate = generateIntermediatePoints(start, end, step_size);
                path.insert(path.end(), intermediate.begin(), intermediate.end());
            }
            else
            {
                path.push_back(start);
                path.push_back(end);
            }
        }

        // Move to the next row (smooth transition)
        if (y + spacing <= y_max)
        {
            Point start = {path.back().x, y, z};
            Point end = {path.back().x, y + spacing, z};
            if (generate_intermediate_points)
            {
                auto intermediate = generateIntermediatePoints(start, end, step_size);
                path.insert(path.end(), intermediate.begin(), intermediate.end());
            }
            else
            {
                path.push_back(start);
                path.push_back(end);
            }
        }

        y += spacing;
    }

    return path;
}

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
void CoveragePathPlanner::writeWaypointsToCSV(const std::vector<std::vector<Point>> &paths, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + filename);
    }

    // Write CSV header
    file << "DroneID,X,Y,Z\n";

    // Write waypoints for each drone
    for (size_t drone_id = 0; drone_id < paths.size(); ++drone_id)
    {
        for (const auto &point : paths[drone_id])
        {
            file << drone_id << "," << point.x << "," << point.y << "," << point.z << "\n";
        }
    }

    file.close();
    std::cout << "Waypoints written to " << filename << std::endl;
}