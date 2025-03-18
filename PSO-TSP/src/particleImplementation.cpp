/**
 * @file particleImplementation.cpp
 * @brief Implementation of the Particle class methods.
 */

#include "particleDefinition.hpp"

/**
 * @brief Get the current route of the particle.
 * 
 * This function returns the current route (sequence of cities) that the particle is following.
 * 
 * @return std::vector<int> A vector representing the particle's current route.
 */
std::vector<int> Particle::getRoute() const {
    return route;
}

/**
 * @brief Get the velocity of the particle.
 * 
 * This function returns the velocity vector of the particle, which determines its movement in the solution space.
 * 
 * @return std::vector<double> A vector representing the particle's velocity.
 */
std::vector<double> Particle::getVelocity() const {
    return velocity;
}

/**
 * @brief Get the best route found by the particle so far.
 * 
 * This function returns the best route (sequence of cities) that the particle has discovered during its search.
 * 
 * @return std::vector<int> A vector representing the particle's best route.
 */
std::vector<int> Particle::getBestRoute() const {
    return bestRoute;
}

/**
 * @brief Get the fitness value of the best route found by the particle.
 * 
 * This function returns the fitness value associated with the best route discovered by the particle.
 * 
 * @return double The fitness value of the particle's best route.
 */
double Particle::getBestFitness() const {
    return bestFitness;
}

/**
 * @brief Set the current route of the particle.
 * 
 * This function updates the particle's current route with a new route.
 * 
 * @param newRoute A vector representing the new route to be assigned to the particle.
 */
void Particle::setRoute(std::vector<int> &newRoute) {
    route = newRoute;
}

/**
 * @brief Set the velocity of the particle.
 * 
 * This function updates the particle's velocity with a new velocity vector.
 * 
 * @param newVelocity A vector representing the new velocity to be assigned to the particle.
 */
void Particle::setVelocity(std::vector<double> &newVelocity) {
    velocity = newVelocity;
}

/**
 * @brief Set the best route found by the particle.
 * 
 * This function updates the particle's best route with a new route.
 * 
 * @param newBestRoute A vector representing the new best route to be assigned to the particle.
 */
void Particle::setBestRoute(std::vector<int> &newBestRoute) {
    bestRoute = newBestRoute;
}

/**
 * @brief Set the fitness value of the best route found by the particle.
 * 
 * This function updates the fitness value associated with the particle's best route.
 * 
 * @param newBestFitness The new fitness value to be assigned to the particle's best route.
 */
void Particle::setBestFitness(double &newBestFitness) {
    bestFitness = newBestFitness;
}