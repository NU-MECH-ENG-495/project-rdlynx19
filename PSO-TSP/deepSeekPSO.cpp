#include "ObjectiveFunction.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>

struct Particle {
    std::vector<int> position;
    std::vector<double> velocity;
    std::vector<int> bestPosition;
    double bestFitness;
};

double calculateDistance(const std::vector<int>& position) {
    double distance = 0.0;
    for (int i = 0; i < NUM_CITIES - 1; i++) {
        int city1 = position[i];
        int city2 = position[i + 1];
        distance += distances[city1][city2];
    }
    int lastCity = position[NUM_CITIES - 1];
    int firstCity = position[0];
    distance += distances[lastCity][firstCity];
    return distance;
}

// Generate random city coordinates within a 10m x 8m rectangle
std::vector<std::pair<double, double>> generateCityCoordinates(int numCities) {
    std::vector<std::pair<double, double>> coordinates(numCities);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(0.0, 10.0); // X-coordinate range
    std::uniform_real_distribution<> yDist(0.0, 8.0);  // Y-coordinate range

    for (int i = 0; i < numCities; i++) {
        coordinates[i] = {xDist(gen), yDist(gen)};
    }

    return coordinates;
}

void updateBestFitness(Particle& p, std::vector<int>& globalBestPosition, double& globalBestFitness) {
    double fitness = calculateDistance(p.position);
    if (fitness < p.bestFitness) {
        p.bestFitness = fitness;
        p.bestPosition = p.position;
    }
    if (fitness < globalBestFitness) {
        globalBestFitness = fitness;
        globalBestPosition = p.position;
    }
}

void initializeParticles(std::vector<Particle>& particles, std::vector<int>& globalBestPosition, double& globalBestFitness) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (auto& p : particles) {
        p.position.resize(NUM_CITIES);
        std::iota(p.position.begin(), p.position.end(), 0);
        std::shuffle(p.position.begin(), p.position.end(), gen);

        p.velocity.resize(NUM_CITIES);
        for (auto& v : p.velocity) {
            v = dis(gen) * 2.0 - 1.0;
        }

        p.bestPosition = p.position;
        p.bestFitness = calculateDistance(p.position);
        updateBestFitness(p, globalBestPosition, globalBestFitness);
    }
}

void updateParticles(std::vector<Particle>& particles, std::vector<int>& globalBestPosition, double& globalBestFitness, int iteration, std::ofstream& outFile) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int pIdx = 0; pIdx < particles.size(); pIdx++) {
        auto& p = particles[pIdx];
        for (int i = 0; i < NUM_CITIES; i++) {
            double r1 = dis(gen);
            double r2 = dis(gen);
            p.velocity[i] = INERTIA_WEIGHT * p.velocity[i] +
                            COGNITIVE_WEIGHT * r1 * (p.bestPosition[i] - p.position[i]) +
                            SOCIAL_WEIGHT * r2 * (globalBestPosition[i] - p.position[i]);
        }

        std::vector<int> newPosition = p.position;
        for (int i = 0; i < NUM_CITIES; i++) {
            // Ensure swapIndex is within valid range
            int swapIndex = (static_cast<int>(std::abs(p.velocity[i])) % NUM_CITIES);
            std::swap(newPosition[i], newPosition[swapIndex]);
        }

        // Validate the new position
        bool isValid = true;
        std::vector<bool> visited(NUM_CITIES, false);
        for (int city : newPosition) {
            if (city < 0 || city >= NUM_CITIES || visited[city]) {
                isValid = false;
                break;
            }
            visited[city] = true;
        }

        if (isValid) {
            p.position = newPosition;
            updateBestFitness(p, globalBestPosition, globalBestFitness);
        }

        // Log particle data to CSV
        outFile << iteration << "," << pIdx;
        for (int city : p.position) {
            outFile << "," << city;
        }
        outFile << "," << calculateDistance(p.position) << "\n";
    }
}

void runPSO(std::vector<Particle>& particles, std::vector<int>& globalBestPosition, double& globalBestFitness, std::ofstream& outFile) {
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        updateParticles(particles, globalBestPosition, globalBestFitness, iter, outFile);
    }
}

void printResults(const std::vector<int>& globalBestPosition, double globalBestFitness, double executionTime) {
    std::cout << "Best Path: ";
    for (int city : globalBestPosition) {
        std::cout << city << " ";
    }
    std::cout << std::endl;
    std::cout << "Best Distance: " << globalBestFitness << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Execution Time: " << executionTime << " milliseconds" << std::endl;
}

int main() {
    std::vector<Particle> particles(NUM_PARTICLES);
    std::vector<int> globalBestPosition(NUM_CITIES);
    double globalBestFitness = INFINITY;

    // Generate city coordinates
    std::vector<std::pair<double, double>> cityCoordinates = generateCityCoordinates(NUM_CITIES);

    // Log city coordinates to a separate CSV file
    std::ofstream coordFile("city_coordinates.csv");
    coordFile << "City,X,Y\n";
    for (int i = 0; i < NUM_CITIES; i++) {
        coordFile << i << "," << cityCoordinates[i].first << "," << cityCoordinates[i].second << "\n";
    }
    coordFile.close();

    // Open CSV file for logging particle data
    std::ofstream outFile("particle_data.csv");
    outFile << "Iteration,ParticleID";
    for (int i = 0; i < NUM_CITIES; i++) {
        outFile << ",City" << i;
    }
    outFile << ",Fitness\n";

    auto start = std::chrono::high_resolution_clock::now();
    initializeParticles(particles, globalBestPosition, globalBestFitness);
    runPSO(particles, globalBestPosition, globalBestFitness, outFile);
    auto end = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printResults(globalBestPosition, globalBestFitness, executionTime);

    outFile.close();
    return 0;
}