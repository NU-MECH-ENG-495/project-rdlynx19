#include "utils.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

double euclideanDistance(std::shared_ptr<City> city1, std::shared_ptr<City> city2)
{
    std::tuple<double, double, double> city1Coordinates = city1->getCoordinates();
    std::tuple<double, double, double> city2Coordinates = city2->getCoordinates();

    double dx = std::get<0>(city1Coordinates) - std::get<0>(city2Coordinates);
    double dy = std::get<1>(city1Coordinates) - std::get<1>(city2Coordinates);
    double dz = std::get<2>(city1Coordinates) - std::get<2>(city2Coordinates);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Function to save the best route coordinates to a CSV file
void saveBestRouteCoordinates(const std::vector<int> &bestRoute, const std::vector<std::shared_ptr<City>> &cityList)
{
    std::ofstream routeFile("../csv/best_route_coordinates.csv");

    // Write header
    routeFile << "Order,CityID,X,Y,Z\n";

    // Write coordinates in the best route order
    for (int i = 0; i < bestRoute.size(); i++)
    {
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

void saveRouteCoordinatesXYZ(const std::vector<int> &bestRoute,
                             const std::vector<std::shared_ptr<City>> &cityList)
{
    std::ofstream xyzFile("../csv/best_route_xyz.csv");

    // No header, just X,Y,Z values

    // Write coordinates in the best route order
    for (int i = 0; i < bestRoute.size(); i++)
    {
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