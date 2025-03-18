#include <coveragePP.h>

/**
 * @brief Main function to demonstrate the CoveragePathPlanner class.
 *
 * This function defines a rectangular area, divides it into subregions, and generates back-and-forth
 * coverage paths for each subregion using multiple threads. The resulting waypoints are written to a CSV file.
 *
 * @return int Returns 0 on successful execution.
 */
int main()
{
    CoveragePathPlanner cpp;

    // Define the rectangular area and subregions
    double area_width = 5.0;                   ///< Width of the rectangular area.
    double area_height = 5.0;                  ///< Height of the rectangular area.
    double search_radius = 0.5;                ///< Search radius for coverage planning.
    double z = 2.0;                            ///< Fixed altitude for all drones.
    double step_size = 0.5;                    ///< Distance between intermediate points.
    bool generate_intermediate_points = false; ///< Flag to enable/disable intermediate point generation.

    // Adjust subregion boundaries to ensure overlap
    double overlap_margin = search_radius / 2; ///< Overlap margin to ensure coverage between subregions.
    std::vector<std::vector<double>> subregions = {
        {0.0, (area_width / 2.0) + overlap_margin, 0.0, (area_height / 2.0) + overlap_margin},               // Q1
        {(area_width / 2.0) - overlap_margin, area_width, 0.0, (area_height / 2.0) + overlap_margin},        // Q2
        {0.0, (area_width / 2.0) + overlap_margin, (area_height / 2.0) - overlap_margin, area_height},       // Q3
        {(area_width / 2.0) - overlap_margin, area_width, (area_height / 2.0) - overlap_margin, area_height} // Q4
    };

    // Generate paths for each drone
    std::vector<std::vector<Point>> paths; ///< Vector to store paths for all drones.
    std::mutex paths_mutex;                ///< Mutex to synchronize access to the paths vector.
    std::vector<std::thread> threads;      ///< Vector to store threads for parallel path generation.

    // Create threads to generate paths for each subregion
    for (const auto &subregion : subregions)
    {
        threads.emplace_back([&, subregion]()
                             {
            double x_min = subregion[0]; ///< Minimum x-coordinate of the subregion.
            double x_max = subregion[1]; ///< Maximum x-coordinate of the subregion.
            double y_min = subregion[2]; ///< Minimum y-coordinate of the subregion.
            double y_max = subregion[3]; ///< Maximum y-coordinate of the subregion.

            // Generate back-and-forth path for the subregion
            auto path = cpp.generateBackAndForthPath(x_min, x_max, y_min, y_max, z, search_radius, step_size, generate_intermediate_points);

            // Safely add the path to the shared vector
            std::lock_guard<std::mutex> lock(paths_mutex);
            paths.push_back(path); });
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    // Write waypoints to CSV
    cpp.writeWaypointsToCSV(paths, "drone_waypoints.csv");

    return 0;
}