/**
 * @file cityImplementation.cpp
 * @brief Implementation of the City class methods.
 */

#include "cityDefinition.hpp"

/**
 * @brief Get the coordinates of the city.
 * 
 * This function returns the coordinates of the city as a tuple containing the x, y, and z values.
 * 
 * @return std::tuple<double, double, double> A tuple representing the city's coordinates (x, y, z).
 */
std::tuple<double, double, double> City::getCoordinates() const {
    return coordinates;
}

/**
 * @brief Set the coordinates of the city.
 * 
 * This function sets the coordinates of the city using the provided x, y, and z values.
 * 
 * @param x The x-coordinate of the city.
 * @param y The y-coordinate of the city.
 * @param z The z-coordinate of the city.
 */
void City::setCoordinates(double x, double y, double z) {
    coordinates = std::make_tuple(x, y, z);
}