#include "particleDefinition.hpp"

std::vector<int> Particle::getRoute() const {
    return route;
}

std::vector<double> Particle::getVelocity() const {
    return velocity;
}

std::vector<int> Particle::getBestRoute() const {
    return bestRoute;
}

double Particle::getBestFitness() const {
    return bestFitness;
}

void Particle::setRoute(std::vector<int> &newRoute){
    route = newRoute;
}

void Particle::setVelocity(std::vector<double> &newVelocity){
    velocity = newVelocity;
}

void Particle::setBestRoute(std::vector<int> &newBestRoute){
    bestRoute = newBestRoute;
}

void Particle::setBestFitness(double &newBestFitness){
    bestFitness = newBestFitness;
}