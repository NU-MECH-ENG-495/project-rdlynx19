#include <coveragePP.h>

// Function to generate intermediate points between two waypoints
std::vector<Point> CoveragePathPlanner::generateIntermediatePoints(Point start, Point end, double step_size)
{
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

// Function to generate a back-and-forth path for a subregion
std::vector<Point> CoveragePathPlanner::generateBackAndForthPath(double x_min, double x_max, double y_min, double y_max, double z, double search_radius, double step_size, bool generate_intermediate_points)
{
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

// Function to write waypoints to a CSV file
void CoveragePathPlanner::writeWaypointsToCSV(const std::vector<std::vector<Point>> &paths, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
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
