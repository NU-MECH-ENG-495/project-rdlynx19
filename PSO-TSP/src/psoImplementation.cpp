/**
 * @file psoImplementation.cpp
 * @brief Implementation of the PSO class methods.
 */

#include "psoDefinition.hpp"
#include "utils.cpp"
#include <random>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cassert>

/**
 * @brief Generates random coordinates for a given number of cities.
 * 
 * This function initializes the `cityList` with random coordinates for each city.
 * The coordinates are generated within predefined ranges for x, y, and z.
 * 
 * @param numCities The number of cities to generate coordinates for.
 */
void PSO::generateCityCoordinates(int numCities) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(-1.0, 1.0);
    std::uniform_real_distribution<> yDist(-1.5, 2.0);
    std::uniform_real_distribution<> zDist(0.0, 1.8);

    this->cityList.resize(numCities);

    for (int i = 0; i < numCities; i++) {
        this->cityList[i] = std::make_shared<City>(i);
        this->cityList[i]->setCoordinates(xDist(gen), yDist(gen), zDist(gen));
    }
}

/**
 * @brief Initializes the distance matrix for all cities.
 * 
 * This function calculates and stores the Euclidean distance between every pair of cities
 * in the `distanceMatrix`. The diagonal elements (distance from a city to itself) are set to 0.
 */
void PSO::initializeDistanceMatrix() {
    distanceMatrix.resize(cityList.size(), std::vector<double>(cityList.size(), 0.0));
    int numCities = cityList.size();
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (i != j) {
                distanceMatrix[i][j] = euclideanDistance(this->cityList[i], this->cityList[j]);
            }
        }
    }
}

/**
 * @brief Calculates the total distance of a given route.
 * 
 * This function computes the total distance traveled for a given route, including the return
 * to the starting city.
 * 
 * @param route The route (sequence of cities) to calculate the distance for.
 * @param numCities The number of cities in the route.
 * @return double The total distance of the route.
 */
double PSO::calculateDistance(const std::vector<int> &route, int numCities) {
    double distance = 0.0;
    for (int i = 0; i < numCities - 1; i++) {
        int city1 = route[i];
        int city2 = route[i + 1];
        distance += distanceMatrix[city1][city2];
    }
    int lastCity = route[numCities - 1];
    int firstCity = route[0];
    distance += distanceMatrix[lastCity][firstCity];
    return distance;
}

/**
 * @brief Updates the best fitness and route for a particle.
 * 
 * This function updates the particle's best fitness and route if the current route
 * has a better fitness value. It also updates the global best fitness and route if applicable.
 * 
 * @param p The particle to update.
 * @param numCities The number of cities in the route.
 */
void PSO::updateBestFitness(std::shared_ptr<Particle> p, int numCities) {
    double fitness = calculateDistance(p->getRoute(), numCities);
    if (fitness < p->getBestFitness()) {
        p->setBestFitness(fitness);
        std::vector<int> newBestRoute = p->getRoute();
        p->setBestRoute(newBestRoute);
    }
    if (fitness < globalBestFitness) {
        globalBestFitness = fitness;
        globalBestRoute = p->getRoute();
    }
}

/**
 * @brief Initializes the particles for the PSO algorithm.
 * 
 * This function initializes the particles with random routes and velocities. It also sets
 * their initial best routes and fitness values.
 * 
 * @param numParticles The number of particles to initialize.
 * @param numCities The number of cities in the problem.
 */
void PSO::initializeParticles(int numParticles, int numCities) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    this->particleList.resize(numParticles);

    for (int i = 0; i < numParticles; i++) {
        this->particleList[i] = std::make_shared<Particle>(i);

        std::vector<int> initializeRoute(numCities);
        std::iota(initializeRoute.begin(), initializeRoute.end(), 0);
        std::shuffle(initializeRoute.begin(), initializeRoute.end(), gen);
        this->particleList[i]->setRoute(initializeRoute);

        std::vector<double> initializeVelocity(numCities);
        for (auto &v : initializeVelocity) {
            v = dis(gen) * 2.0 - 1.0;
        }
        this->particleList[i]->setVelocity(initializeVelocity);

        this->particleList[i]->setBestRoute(initializeRoute);
        double fitness = calculateDistance(initializeRoute, numCities);
        this->particleList[i]->setBestFitness(fitness);

        updateBestFitness(this->particleList[i], numCities);
    }
}

/**
 * @brief Updates the particles' positions and velocities for a given iteration.
 * 
 * This function updates each particle's velocity and route based on its current state,
 * personal best, and the global best. It also logs the particle's state to the output file.
 * 
 * @param iteration The current iteration number.
 * @param outFile The output file stream to log particle data.
 * @param numCities The number of cities in the problem.
 */
void PSO::updateParticles(int iteration, std::ofstream &outFile, int numCities) {
    std::vector<std::thread> threads;

    for (int pIdx = 0; pIdx < this->particleList.size(); pIdx++) {
        // Lambda Function
        threads.emplace_back([this, pIdx, iteration, numCities, &outFile]() {
            auto &p = this->particleList[pIdx];

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.0);

            std::vector<double> updatedVelocity = p->getVelocity();
            std::vector<int> currentBestRoute = p->getBestRoute();
            std::vector<int> currentRoute = p->getRoute();
            for (int i = 0; i < numCities; i++) {
                double r1 = dis(gen);
                double r2 = dis(gen);

                updatedVelocity[i] = INERTIA_WEIGHT * updatedVelocity[i] +
                                     COGNITIVE_WEIGHT * r1 * (currentBestRoute[i] - currentRoute[i]) +
                                     SOCIAL_WEIGHT * r2 * (globalBestRoute[i] - currentRoute[i]);
            }
            p->setVelocity(updatedVelocity);
            for (int i = 0; i < NUM_CITIES; i++) {
                int swapIndex = (static_cast<int>(std::abs(updatedVelocity[i])) % NUM_CITIES);
                std::swap(currentRoute[i], currentRoute[swapIndex]);
            }

            bool isValid = true;
            std::vector<bool> visited(NUM_CITIES, false);
            for (int city : currentRoute) {
                if (city < 0 || city >= NUM_CITIES || visited[city]) {
                    isValid = false;
                    break;
                }
                visited[city] = true;
            }

            if (isValid) {
                p->setRoute(currentRoute);
                double currentFitness = calculateDistance(currentRoute, numCities);

                std::lock_guard<std::mutex> lock(globalMutex);
                if (currentFitness < p->getBestFitness()) {
                    p->setBestFitness(currentFitness);
                    p->setBestRoute(currentRoute);
                }
                if (currentFitness < globalBestFitness) {
                    globalBestFitness = currentFitness;
                    globalBestRoute = currentRoute;
                }
            }
            std::lock_guard<std::mutex> lock(globalMutex);
            outFile << iteration << "," << pIdx;
            for (int city : p->getRoute()) {
                outFile << "," << city;
            }
            outFile << "," << calculateDistance(p->getRoute(), NUM_CITIES) << "\n";
        });
    }

    for (auto &t : threads) {
        t.join();
    }
}

/**
 * @brief Runs the PSO algorithm for a fixed number of iterations.
 * 
 * This function executes the PSO algorithm, updating particles and logging their states
 * for each iteration.
 * 
 * @param outFile The output file stream to log particle data.
 * @param numCities The number of cities in the problem.
 */
void PSO::runPSO(std::ofstream &outFile, int numCities) {
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        updateParticles(iter, outFile, numCities);
    }
}

/**
 * @brief Prints the results of the PSO algorithm.
 * 
 * This function prints the best route found, its distance, and the execution time.
 * 
 * @param executionTime The total execution time of the PSO algorithm in milliseconds.
 */
void PSO::printResults(double executionTime) {
    std::cout << "Best Path: ";
    for (int city : globalBestRoute) {
        std::cout << city << " ";
    }
    std::cout << std::endl;
    std::cout << "Best Distance: " << globalBestFitness << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Execution Time: " << executionTime << " milliseconds" << std::endl;
}
