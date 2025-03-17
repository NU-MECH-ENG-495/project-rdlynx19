#include "cityDefinition.hpp"

std::tuple<double, double, double> City::getCoordinates() const {
    return coordinates;
}

void City::setCoordinates (double x, double y, double z){
    coordinates = std::make_tuple(x, y, z);
}