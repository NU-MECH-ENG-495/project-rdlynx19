/**
 * @file utils.cpp
 * @brief Implementation of utility functions for the PSO algorithm.
 */

#include "utils.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

/**
 * @brief Calculates the Euclidean distance between two cities.
 * 
 * This function computes the Euclidean distance between two cities based on their
 * coordinates (x, y, z).
 * 
 * @param city1 The first city.
 * @param city2 The second city.
 * @return double The Euclidean distance between the two cities.
 */
double euclideanDistance(std::shared_ptr<City> city1, std::shared_ptr<City> city2) {
    std::tuple<double, double, double> city1Coordinates = city1->getCoordinates();
    std::tuple<double, double, double> city2Coordinates = city2->getCoordinates();

    double dx = std::get<0>(city1Coordinates) - std::get<0>(city2Coordinates);
    double dy = std::get<1>(city1Coordinates) - std::get<1>(city2Coordinates);
    double dz = std::get<2>(city1Coordinates) - std::get<2>(city2Coordinates);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

/**
 * @brief Saves the coordinates of the best route to a CSV file.
 * 
 * This function writes the coordinates of the cities in the best route to a CSV file.
 * The file includes the order of the cities, their IDs, and their coordinates (x, y, z).
 * 
 * @param bestRoute The best route (sequence of city IDs).
 * @param cityList The list of cities with their coordinates.
 */
void saveBestRouteCoordinates(const std::vector<int> &bestRoute, const std::vector<std::shared_ptr<City>> &cityList) {
    std::ofstream routeFile("../csv/best_route_coordinates.csv");

    // Write header
    routeFile << "Order,CityID,X,Y,Z\n";

    // Write coordinates in the best route order
    for (int i = 0; i < bestRoute.size(); i++) {
        int cityId = bestRoute[i];
        routeFile << i << ","
                  << cityId << ","
                  << std::get<0>(cityList[i]->getCoordinates()) << ","
                  << std::get<1>(cityList[i]->getCoordinates()) << ","
                  << std::get<2>(cityList[i]->getCoordinates()) << "\n";
    }

    // Add the first city again to complete the tour
    int firstCityId = bestRoute[0];
    routeFile << bestRoute.size() << ","
              << firstCityId << ","
              << std::get<0>(cityList[firstCityId]->getCoordinates()) << ","
              << std::get<1>(cityList[firstCityId]->getCoordinates()) << ","
              << std::get<2>(cityList[firstCityId]->getCoordinates()) << "\n";

    routeFile.close();
    std::cout << "Best route coordinates saved to 'best_route_coordinates.csv'" << std::endl;
}

/**
 * @brief Saves the XYZ coordinates of the best route to a CSV file.
 * 
 * This function writes the XYZ coordinates of the cities in the best route to a CSV file.
 * The file contains only the coordinates (x, y, z) in the order of the best route.
 * 
 * @param bestRoute The best route (sequence of city IDs).
 * @param cityList The list of cities with their coordinates.
 */
void saveRouteCoordinatesXYZ(const std::vector<int> &bestRoute,
                             const std::vector<std::shared_ptr<City>> &cityList) {
    std::ofstream xyzFile("../csv/best_route_xyz.csv");

    // No header, just X,Y,Z values

    // Write coordinates in the best route order
    for (int i = 0; i < bestRoute.size(); i++) {
        int cityId = bestRoute[i];
        xyzFile << std::get<0>(cityList[i]->getCoordinates()) << ","
                << std::get<1>(cityList[i]->getCoordinates()) << ","
                << std::get<2>(cityList[i]->getCoordinates()) << "\n";
    }

    // Add the first city again to complete the tour
    int firstCityId = bestRoute[0];
    xyzFile << std::get<0>(cityList[firstCityId]->getCoordinates()) << ","
            << std::get<1>(cityList[firstCityId]->getCoordinates()) << ","
            << std::get<2>(cityList[firstCityId]->getCoordinates()) << "\n";

    xyzFile.close();
    std::cout << "XYZ coordinates of best route saved to 'best_route_xyz.csv'" << std::endl;
}