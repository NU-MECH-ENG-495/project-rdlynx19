#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <memory>
#include "cityDefinition.hpp"

// Declare functions
double euclideanDistance(std::shared_ptr<City> city1, std::shared_ptr<City> city2);
void saveBestRouteCoordinates(const std::vector<int>& route, const std::vector<std::shared_ptr<City>>& cityList);
void saveRouteCoordinatesXYZ(const std::vector<int>& route, const std::vector<std::shared_ptr<City>>& cityList);

#endif // UTILS_H