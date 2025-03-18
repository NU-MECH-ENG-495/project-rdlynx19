#include "psoDefinition.hpp"
#include "utils.hpp"
#include <chrono>
#include <fstream>

/**
 * @brief Main function to execute the Particle Swarm Optimization (PSO) algorithm.
 * 
 * This function initializes the PSO algorithm, generates city coordinates, computes the distance matrix,
 * and runs the PSO algorithm to find the optimal route. It also logs the results and execution time.
 * 
 * @return int Returns 0 on successful execution.
 */
int main() {
    // Initialize the PSO algorithm
    PSO algoSim;

    // Generate random coordinates for cities and initialize the distance matrix
    algoSim.generateCityCoordinates(NUM_CITIES);
    algoSim.initializeDistanceMatrix();

    // Retrieve the list of cities and save their coordinates to a CSV file
    std::vector<std::shared_ptr<City>> cityList = algoSim.getCityList();
    std::ofstream coordFile("../csv/city_coordinates.csv");
    coordFile << "City,X,Y,Z\n";
    for (int i = 0; i < NUM_CITIES; i++) {
        coordFile << i << "," << std::get<0>(cityList[i]->getCoordinates()) << ","
                  << std::get<1>(cityList[i]->getCoordinates()) << ","
                  << std::get<2>(cityList[i]->getCoordinates()) << "\n";
    }
    coordFile.close();

    // Open a file to log particle data during the PSO execution
    std::ofstream outFile("../csv/particle_data.csv");
    outFile << "Iteration,ParticleID";
    for (int i = 0; i < NUM_CITIES; i++) {
        outFile << ",City" << i;
    }
    outFile << ",Fitness\n";

    // Start the timer for execution time measurement
    auto start = std::chrono::high_resolution_clock::now();

    // Initialize particles and run the PSO algorithm
    algoSim.initializeParticles(NUM_PARTICLES, NUM_CITIES);
    algoSim.runPSO(outFile, NUM_CITIES);

    // Stop the timer and calculate the execution time
    auto end = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Print the results of the PSO algorithm
    algoSim.printResults(executionTime);

    // Save the best route coordinates to files
    saveBestRouteCoordinates(algoSim.getGlobalBestRoute(), cityList);
    saveRouteCoordinatesXYZ(algoSim.getGlobalBestRoute(), cityList);

    // Close the output file and return
    outFile.close();
    return 0;
}