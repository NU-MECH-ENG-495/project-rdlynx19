#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Define a structure for a 3D point
struct Point
{
    double x, y, z;
};

// Function to generate intermediate points between two waypoints
std::vector<Point> generateIntermediatePoints(Point start, Point end, double step_size)
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
std::vector<Point> generateBackAndForthPath(double x_min, double x_max, double y_min, double y_max, double z, double search_radius, double step_size)
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
            auto intermediate = generateIntermediatePoints(start, end, step_size);
            path.insert(path.end(), intermediate.begin(), intermediate.end());
        }
        else
        {
            // Move left
            Point start = {x_max, y, z};
            Point end = {x_min, y, z};
            auto intermediate = generateIntermediatePoints(start, end, step_size);
            path.insert(path.end(), intermediate.begin(), intermediate.end());
        }

        // Move to the next row (smooth transition)
        if (y + spacing <= y_max)
        {
            Point start = {path.back().x, y, z};
            Point end = {path.back().x, y + spacing, z};
            auto intermediate = generateIntermediatePoints(start, end, step_size);
            path.insert(path.end(), intermediate.begin(), intermediate.end());
        }

        y += spacing;
    }

    return path;
}

// Function to write waypoints to a CSV file
void writeWaypointsToCSV(const std::vector<std::vector<Point>> &paths, const std::string &filename)
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

int main()
{
    // Define the rectangular area and subregions
    double area_width = 10.0;
    double area_height = 10.0;
    double search_radius = 1.0;
    double z = 2.0;         // Fixed altitude for all drones
    double step_size = 0.5; // Distance between intermediate points

    // Adjust subregion boundaries to ensure overlap
    double overlap_margin = search_radius / 2; // Overlap by half the search radius
    std::vector<std::vector<double>> subregions = {
        {0.0, (area_width / 2.0) + overlap_margin, 0.0, (area_height / 2.0) + overlap_margin},               // Q1
        {(area_width / 2.0) - overlap_margin, area_width, 0.0, (area_height / 2.0) + overlap_margin},        // Q2
        {0.0, (area_width / 2.0) + overlap_margin, (area_height / 2.0) - overlap_margin, area_height},       // Q3
        {(area_width / 2.0) - overlap_margin, area_width, (area_height / 2.0) - overlap_margin, area_height} // Q4
    };

    // Generate paths for each drone
    std::vector<std::vector<Point>> paths;
    for (const auto &subregion : subregions)
    {
        double x_min = subregion[0];
        double x_max = subregion[1];
        double y_min = subregion[2];
        double y_max = subregion[3];
        paths.push_back(generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size));
    }

    // Write waypoints to CSV
    writeWaypointsToCSV(paths, "drone_waypoints.csv");

    return 0;
}