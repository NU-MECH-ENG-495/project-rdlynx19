#include <coveragePP.h>

int main()
{
    CoveragePathPlanner cpp;
    // Define the rectangular area and subregions
    double area_width = 5.0;
    double area_height = 5.0;
    double search_radius = 0.5;
    double z = 2.0;         // Fixed altitude for all drones
    double step_size = 0.5; // Distance between intermediate points
    bool generate_intermediate_points = false;

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

    // Write waypoints to CSV
    cpp.writeWaypointsToCSV(paths, "drone_waypoints.csv");

    return 0;
}