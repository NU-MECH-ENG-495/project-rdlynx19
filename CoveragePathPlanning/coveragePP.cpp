#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Define a structure for a 3D point
struct Point
{
    double x, y, z;
};

// Function to generate a back-and-forth path for a subregion
std::vector<Point> generateBackAndForthPath(double x_min, double x_max, double y_min, double y_max, double z, double search_radius)
{
    std::vector<Point> path;
    double spacing = 2 * search_radius;
    double y = y_min;

    while (y <= y_max)
    {
        if (fmod(y - y_min, 2 * spacing) < 1e-6)
        {
            // Move right
            path.push_back({x_min, y, z});
            path.push_back({x_max, y, z});
        }
        else
        {
            // Move left
            path.push_back({x_max, y, z});
            path.push_back({x_min, y, z});
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
    double area_width = 100.0;
    double area_height = 100.0;
    double search_radius = 5.0;
    double z = 10.0; // Fixed altitude for all drones

    // Divide the area into 4 quadrants
    std::vector<std::vector<double>> subregions = {
        {0.0, (area_width / 2.0), 0.0, area_height / 2.0},             // Q1
        {area_width / 2.0, area_width, 0.0, area_height / 2.0},        // Q2
        {0.0, area_width / 2.0, area_height / 2.0, area_height},       // Q3
        {area_width / 2.0, area_width, area_height / 2.0, area_height} // Q4
    };

    // Generate paths for each drone
    std::vector<std::vector<Point>> paths;
    for (const auto &subregion : subregions)
    {
        double x_min = subregion[0];
        double x_max = subregion[1];
        double y_min = subregion[2];
        double y_max = subregion[3];
        paths.push_back(generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius));
    }

    // Write waypoints to CSV
    writeWaypointsToCSV(paths, "drone_waypoints.csv");

    return 0;
}