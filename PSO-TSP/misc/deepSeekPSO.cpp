#include "ObjectiveFunction.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include <tuple> // Include tuple for 3D coordinates

struct Particle
{
    std::vector<int> position;
    std::vector<double> velocity;
    std::vector<int> bestPosition;
    double bestFitness;
};

// Function to calculate Euclidean distance between two 3D points
double euclideanDistance(const std::tuple<double, double, double> &p1, const std::tuple<double, double, double> &p2)
{
    double dx = std::get<0>(p1) - std::get<0>(p2);
    double dy = std::get<1>(p1) - std::get<1>(p2);
    double dz = std::get<2>(p1) - std::get<2>(p2);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Function to calculate the total distance of a route
double calculateDistance(const std::vector<int> &position, const std::vector<std::vector<double>> &distances)
{
    double distance = 0.0;
    for (int i = 0; i < NUM_CITIES - 1; i++)
    {
        int city1 = position[i];
        int city2 = position[i + 1];
        distance += distances[city1][city2];
    }
    int lastCity = position[NUM_CITIES - 1];
    int firstCity = position[0];
    distance += distances[lastCity][firstCity];
    return distance;
}

// Generate random city coordinates in 3D space within a 10m x 8m x 6m box
std::vector<std::tuple<double, double, double>> generateCityCoordinates(int numCities)
{
    std::vector<std::tuple<double, double, double>> coordinates(numCities);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(0.0, 10.0); // X-coordinate range
    std::uniform_real_distribution<> yDist(0.0, 8.0);  // Y-coordinate range
    std::uniform_real_distribution<> zDist(0.0, 6.0);  // Z-coordinate range

    for (int i = 0; i < numCities; i++)
    {
        coordinates[i] = std::make_tuple(xDist(gen), yDist(gen), zDist(gen));
    }

    return coordinates;
}

// Function to initialize the distance matrix based on 3D city coordinates
std::vector<std::vector<double>> initializeDistanceMatrix(const std::vector<std::tuple<double, double, double>> &cityCoordinates)
{
    int numCities = cityCoordinates.size();
    std::vector<std::vector<double>> distances(numCities, std::vector<double>(numCities, 0.0));

    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; j < numCities; j++)
        {
            if (i != j)
            {
                distances[i][j] = euclideanDistance(cityCoordinates[i], cityCoordinates[j]);
            }
        }
    }

    return distances;
}

void updateBestFitness(Particle &p, std::vector<int> &globalBestPosition, double &globalBestFitness, const std::vector<std::vector<double>> &distances)
{
    double fitness = calculateDistance(p.position, distances);
    if (fitness < p.bestFitness)
    {
        p.bestFitness = fitness;
        p.bestPosition = p.position;
    }
    if (fitness < globalBestFitness)
    {
        globalBestFitness = fitness;
        globalBestPosition = p.position;
    }
}

void initializeParticles(std::vector<Particle> &particles, std::vector<int> &globalBestPosition, double &globalBestFitness, const std::vector<std::vector<double>> &distances)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (auto &p : particles)
    {
        p.position.resize(NUM_CITIES);
        std::iota(p.position.begin(), p.position.end(), 0);
        std::shuffle(p.position.begin(), p.position.end(), gen);

        p.velocity.resize(NUM_CITIES);
        for (auto &v : p.velocity)
        {
            v = dis(gen) * 2.0 - 1.0;
        }

        p.bestPosition = p.position;
        p.bestFitness = calculateDistance(p.position, distances);
        updateBestFitness(p, globalBestPosition, globalBestFitness, distances);
    }
}

void updateParticles(std::vector<Particle> &particles, std::vector<int> &globalBestPosition, double &globalBestFitness, int iteration, std::ofstream &outFile, const std::vector<std::vector<double>> &distances)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int pIdx = 0; pIdx < particles.size(); pIdx++)
    {
        auto &p = particles[pIdx];
        for (int i = 0; i < NUM_CITIES; i++)
        {
            double r1 = dis(gen);
            double r2 = dis(gen);
            p.velocity[i] = INERTIA_WEIGHT * p.velocity[i] +
                            COGNITIVE_WEIGHT * r1 * (p.bestPosition[i] - p.position[i]) +
                            SOCIAL_WEIGHT * r2 * (globalBestPosition[i] - p.position[i]);
        }

        std::vector<int> newPosition = p.position;
        for (int i = 0; i < NUM_CITIES; i++)
        {
            // Ensure swapIndex is within valid range
            int swapIndex = (static_cast<int>(std::abs(p.velocity[i])) % NUM_CITIES);
            std::swap(newPosition[i], newPosition[swapIndex]);
        }

        // Validate the new position
        bool isValid = true;
        std::vector<bool> visited(NUM_CITIES, false);
        for (int city : newPosition)
        {
            if (city < 0 || city >= NUM_CITIES || visited[city])
            {
                isValid = false;
                break;
            }
            visited[city] = true;
        }

        if (isValid)
        {
            p.position = newPosition;
            updateBestFitness(p, globalBestPosition, globalBestFitness, distances);
        }

        // Log particle data to CSV
        outFile << iteration << "," << pIdx;
        for (int city : p.position)
        {
            outFile << "," << city;
        }
        outFile << "," << calculateDistance(p.position, distances) << "\n";
    }
}

void runPSO(std::vector<Particle> &particles, std::vector<int> &globalBestPosition, double &globalBestFitness, std::ofstream &outFile, const std::vector<std::vector<double>> &distances)
{
    for (int iter = 0; iter < MAX_ITERATIONS; iter++)
    {
        updateParticles(particles, globalBestPosition, globalBestFitness, iter, outFile, distances);
    }
}

void printResults(const std::vector<int> &globalBestPosition, double globalBestFitness, double executionTime)
{
    std::cout << "Best Path: ";
    for (int city : globalBestPosition)
    {
        std::cout << city << " ";
    }
    std::cout << std::endl;
    std::cout << "Best Distance: " << globalBestFitness << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Execution Time: " << executionTime << " milliseconds" << std::endl;
}

// Function to save the best route coordinates to a CSV file
void saveBestRouteCoordinates(const std::vector<int> &bestRoute,
                              const std::vector<std::tuple<double, double, double>> &cityCoordinates)
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
                  << std::get<0>(cityCoordinates[cityId]) << ","
                  << std::get<1>(cityCoordinates[cityId]) << ","
                  << std::get<2>(cityCoordinates[cityId]) << "\n";
    }

    // Add the first city again to complete the tour
    int firstCityId = bestRoute[0];
    routeFile << bestRoute.size() << ","
              << firstCityId << ","
              << std::get<0>(cityCoordinates[firstCityId]) << ","
              << std::get<1>(cityCoordinates[firstCityId]) << ","
              << std::get<2>(cityCoordinates[firstCityId]) << "\n";

    routeFile.close();
    std::cout << "Best route coordinates saved to 'best_route_coordinates.csv'" << std::endl;
}

// Function to save only the X, Y, Z coordinates of the best route to a CSV file
void saveRouteCoordinatesXYZ(const std::vector<int> &bestRoute,
                             const std::vector<std::tuple<double, double, double>> &cityCoordinates)
{
    std::ofstream xyzFile("../csv/best_route_xyz.csv");

    // No header, just X,Y,Z values

    // Write coordinates in the best route order
    for (int i = 0; i < bestRoute.size(); i++)
    {
        int cityId = bestRoute[i];
        xyzFile << std::get<0>(cityCoordinates[cityId]) << ","
                << std::get<1>(cityCoordinates[cityId]) << ","
                << std::get<2>(cityCoordinates[cityId]) << "\n";
    }

    // Add the first city again to complete the tour
    int firstCityId = bestRoute[0];
    xyzFile << std::get<0>(cityCoordinates[firstCityId]) << ","
            << std::get<1>(cityCoordinates[firstCityId]) << ","
            << std::get<2>(cityCoordinates[firstCityId]) << "\n";

    xyzFile.close();
    std::cout << "XYZ coordinates of best route saved to 'best_route_xyz.csv'" << std::endl;
}

int main()
{
    std::vector<Particle> particles(NUM_PARTICLES);
    std::vector<int> globalBestPosition(NUM_CITIES);
    double globalBestFitness = INFINITY;

    // Generate city coordinates in 3D
    std::vector<std::tuple<double, double, double>> cityCoordinates = generateCityCoordinates(NUM_CITIES);

    // Initialize the distance matrix based on 3D city coordinates
    std::vector<std::vector<double>> distances = initializeDistanceMatrix(cityCoordinates);

    // Log city coordinates to a separate CSV file
    std::ofstream coordFile("../csv/city_coordinates.csv");
    coordFile << "City,X,Y,Z\n";
    for (int i = 0; i < NUM_CITIES; i++)
    {
        coordFile << i << "," << std::get<0>(cityCoordinates[i]) << "," << std::get<1>(cityCoordinates[i]) << "," << std::get<2>(cityCoordinates[i]) << "\n";
    }
    coordFile.close();

    // Open CSV file for logging particle data
    std::ofstream outFile("../csv/particle_data.csv");
    outFile << "Iteration,ParticleID";
    for (int i = 0; i < NUM_CITIES; i++)
    {
        outFile << ",City" << i;
    }
    outFile << ",Fitness\n";

    auto start = std::chrono::high_resolution_clock::now();
    initializeParticles(particles, globalBestPosition, globalBestFitness, distances);
    runPSO(particles, globalBestPosition, globalBestFitness, outFile, distances);
    auto end = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printResults(globalBestPosition, globalBestFitness, executionTime);

    saveBestRouteCoordinates(globalBestPosition, cityCoordinates);

    saveRouteCoordinatesXYZ(globalBestPosition, cityCoordinates);

    outFile.close();
    return 0;
}