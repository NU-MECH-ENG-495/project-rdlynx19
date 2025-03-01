#include "ObjectiveFunction.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <unordered_set>

// Constants for the search space
constexpr double SPACE_X_MIN = 0.0;
constexpr double SPACE_X_MAX = 10.0;  // Reduced search space size
constexpr double SPACE_Y_MIN = 0.0;
constexpr double SPACE_Y_MAX = 10.0;  // Reduced search space size
constexpr double SPACE_Z_MIN = 0.0;
constexpr double SPACE_Z_MAX = 10.0;  // Reduced search space size

// Search radius for collision avoidance
constexpr double SEARCH_RADIUS = 1.0;  // Drones cannot come closer than this distance

// Reward for visiting a new cell
constexpr double NEW_CELL_REWARD = 0.1;  // Reward for visiting a new cell

// Structure to represent a drone (particle)
struct Particle {
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> bestPosition;
    double bestFitness;
};

// Structure to represent the best position in a grid
struct GridBest {
    std::vector<double> position;
    double fitness;
};

// Function to clear the contents of a file
void clearFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc); // Open in truncation mode to clear the file
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    file.close();
}

// Function to transform global coordinates to local coordinates
std::vector<double> globalToLocal(const std::vector<double>& globalPos, double gridXMin, double gridYMin) {
    std::vector<double> localPos = globalPos;
    localPos[0] -= gridXMin; // Translate X-coordinate
    localPos[1] -= gridYMin; // Translate Y-coordinate
    return localPos;
}

// Function to update the best fitness of a particle and the global best
void updateBestFitness(Particle& p, std::vector<double>& globalBestPosition, double& globalBestFitness, 
                       double gridXMin, double gridYMin, std::unordered_set<std::string>& visitedCells) {
    // Transform global coordinates to local coordinates
    std::vector<double> localPos = globalToLocal(p.position, gridXMin, gridYMin);

    // Evaluate the objective function in local coordinates
    double fitness = objectiveFunction(localPos);

    // Check if the current cell is new
    int cellX = static_cast<int>(p.position[0]); // Convert X position to cell index
    int cellY = static_cast<int>(p.position[1]); // Convert Y position to cell index
    std::string cellKey = std::to_string(cellX) + "," + std::to_string(cellY); // Create a unique key for the cell

    if (visitedCells.find(cellKey) == visitedCells.end()) {
        // If the cell is new, add a reward to the fitness
        fitness -= NEW_CELL_REWARD; // Subtract because we are minimizing the fitness
        visitedCells.insert(cellKey); // Mark the cell as visited
    }

    if (fitness < p.bestFitness) {
        p.bestFitness = fitness;
        p.bestPosition = p.position;
    }

    if (fitness < globalBestFitness) {
        globalBestFitness = fitness;
        globalBestPosition = p.position;
    }
}

// Function to initialize particles (drones) within the current grid
void initializeParticles(std::vector<Particle>& particles, std::vector<double>& globalBestPosition, double& globalBestFitness, 
                         double gridXMin, double gridXMax, double gridYMin, double gridYMax, std::unordered_set<std::string>& visitedCells) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disVel(-1.0, 1.0);

    for (auto& p : particles) {
        p.position.resize(DIMENSIONS);
        p.velocity.resize(DIMENSIONS);
        p.bestPosition.resize(DIMENSIONS);

        // Initialize position within the current grid
        std::uniform_real_distribution<> disX(gridXMin, gridXMax);
        std::uniform_real_distribution<> disY(gridYMin, gridYMax);
        std::uniform_real_distribution<> disZ(SPACE_Z_MIN, SPACE_Z_MAX);

        p.position[0] = disX(gen); // X-coordinate
        p.position[1] = disY(gen); // Y-coordinate
        p.position[2] = disZ(gen); // Z-coordinate

        // Initialize velocity
        for (int j = 0; j < DIMENSIONS; ++j) {
            p.velocity[j] = disVel(gen);
        }

        p.bestPosition = p.position;
        p.bestFitness = INFINITY;

        updateBestFitness(p, globalBestPosition, globalBestFitness, gridXMin, gridYMin, visitedCells);
    }
}

// Function to handle boundary conditions (reflective boundaries)
void handleBoundaries(Particle& p, double gridXMin, double gridXMax, double gridYMin, double gridYMax) {
    // Reflective boundary handling for X and Y coordinates
    if (p.position[0] < gridXMin) {
        p.position[0] = gridXMin;
        p.velocity[0] = -p.velocity[0]; // Reverse velocity
    } else if (p.position[0] > gridXMax) {
        p.position[0] = gridXMax;
        p.velocity[0] = -p.velocity[0]; // Reverse velocity
    }

    if (p.position[1] < gridYMin) {
        p.position[1] = gridYMin;
        p.velocity[1] = -p.velocity[1]; // Reverse velocity
    } else if (p.position[1] > gridYMax) {
        p.position[1] = gridYMax;
        p.velocity[1] = -p.velocity[1]; // Reverse velocity
    }

    // Ensure Z-coordinate stays within the search space
    p.position[2] = std::max(SPACE_Z_MIN, std::min(SPACE_Z_MAX, p.position[2]));
}

// Function to calculate the Euclidean distance between two positions
double calculateDistance(const std::vector<double>& pos1, const std::vector<double>& pos2) {
    double dx = pos1[0] - pos2[0];
    double dy = pos1[1] - pos2[1];
    double dz = pos1[2] - pos2[2];
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Function to update particles (drones) positions and velocities with collision avoidance
void updateParticles(std::vector<Particle>& particles, std::vector<double>& globalBestPosition, double& globalBestFitness, 
                     double gridXMin, double gridXMax, double gridYMin, double gridYMax, GridBest& gridBest, 
                     std::unordered_set<std::string>& visitedCells) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < particles.size(); ++i) {
        auto& p = particles[i];

        // Update velocity using PSO equations
        for (int j = 0; j < DIMENSIONS; ++j) {
            double r1 = dis(gen);
            double r2 = dis(gen);

            p.velocity[j] = INERTIA_WEIGHT * p.velocity[j] +
                            COGNITIVE_WEIGHT * r1 * (p.bestPosition[j] - p.position[j]) +
                            SOCIAL_WEIGHT * r2 * (globalBestPosition[j] - p.position[j]);
        }

        // Check for collisions with other drones
        for (size_t k = 0; k < particles.size(); ++k) {
            if (i == k) continue; // Skip self

            auto& other = particles[k];
            double distance = calculateDistance(p.position, other.position);

            // If drones are too close, adjust velocities to avoid collision
            if (distance < 2 * SEARCH_RADIUS) {
                double repelStrength = 0.1; // Strength of repulsion
                for (int j = 0; j < DIMENSIONS; ++j) {
                    double delta = p.position[j] - other.position[j];
                    p.velocity[j] += repelStrength * delta / distance;
                    other.velocity[j] -= repelStrength * delta / distance;
                }
            }
        }

        // Update position
        for (int j = 0; j < DIMENSIONS; ++j) {
            p.position[j] += p.velocity[j];
        }

        // Handle boundary conditions
        handleBoundaries(p, gridXMin, gridXMax, gridYMin, gridYMax);

        updateBestFitness(p, globalBestPosition, globalBestFitness, gridXMin, gridYMin, visitedCells);

        // Update the best position in the current grid
        if (p.bestFitness < gridBest.fitness) {
            gridBest.position = p.bestPosition;
            gridBest.fitness = p.bestFitness;
        }
    }
}

// Function to save particle positions, global best, and grid best to CSV
void saveParticlePositionsToCSV(const std::vector<Particle>& particles, 
                                const std::vector<double>& globalBestPosition, 
                                double globalBestFitness, 
                                const GridBest& gridBest,
                                const std::string& filename, 
                                int iteration) {
    std::ofstream file(filename, std::ios::app); // Open in append mode
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write iteration header
    file << "Iteration " << iteration << "\n";

    // Write particle positions
    for (const auto& p : particles) {
        for (size_t i = 0; i < p.position.size(); ++i) {
            file << p.position[i];
            if (i < p.position.size() - 1) file << ",";
        }
        file << "\n";
    }

    // Write global best position and fitness
    file << "Global Best,";
    for (size_t i = 0; i < globalBestPosition.size(); ++i) {
        file << globalBestPosition[i];
        if (i < globalBestPosition.size() - 1) file << ",";
    }
    file << "," << globalBestFitness << "\n";

    // Write grid best position and fitness
    file << "Grid Best,";
    for (size_t i = 0; i < gridBest.position.size(); ++i) {
        file << gridBest.position[i];
        if (i < gridBest.position.size() - 1) file << ",";
    }
    file << "," << gridBest.fitness << "\n";

    file << "\n"; // Separate iterations with a blank line
    file.close();
}

// Function to calculate the percentage of the grid covered
double calculateCoveragePercentage(const std::unordered_set<std::string>& visitedCells, double gridXMin, double gridXMax, double gridYMin, double gridYMax) {
    int totalCells = static_cast<int>((gridXMax - gridXMin) * (gridYMax - gridYMin)); // Total number of cells in the grid
    int visitedCellCount = visitedCells.size(); // Number of unique cells visited
    return (static_cast<double>(visitedCellCount) / totalCells) * 100.0; // Coverage percentage
}

// Function to run the PSO algorithm for a single grid
void runPSOForGrid(std::vector<Particle>& particles, std::vector<double>& globalBestPosition, double& globalBestFitness, 
                   double gridXMin, double gridXMax, double gridYMin, double gridYMax, const std::string& filename) {
    std::ofstream outputFile("results.txt");

    GridBest gridBest;
    gridBest.position.resize(DIMENSIONS);
    gridBest.fitness = INFINITY;

    std::unordered_set<std::string> visitedCells; // Track visited cells

    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        updateParticles(particles, globalBestPosition, globalBestFitness, gridXMin, gridXMax, gridYMin, gridYMax, gridBest, visitedCells);

        // Write the global best fitness value of each iteration to the file
        outputFile << iter + 1 << ": " << globalBestFitness << std::endl;

        // Save particle positions, global best, and grid best to CSV
        saveParticlePositionsToCSV(particles, globalBestPosition, globalBestFitness, gridBest, filename, iter);
    }

    // Calculate and print the coverage percentage
    double coveragePercentage = calculateCoveragePercentage(visitedCells, gridXMin, gridXMax, gridYMin, gridYMax);
    std::cout << "Coverage Percentage: " << coveragePercentage << "%" << std::endl;

    outputFile.close();
}

// Function to print the results
void printResults(const std::vector<double>& globalBestPosition, double globalBestFitness, double executionTime) {
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Global Best Position: (";
    for (int i = 0; i < DIMENSIONS; i++) {
        std::cout << globalBestPosition[i];
        if (i < DIMENSIONS - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
    std::cout << "Global Best Value: " << globalBestFitness << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Execution Time: " << executionTime << " milliseconds" << std::endl;
}

int main() {
    std::vector<Particle> particles(NUM_PARTICLES);
    std::vector<double> globalBestPosition(DIMENSIONS);
    double globalBestFitness = INFINITY;

    // Clear the CSV file before starting
    clearFile("particle_positions.csv");

    auto start = std::chrono::high_resolution_clock::now();

    // Define grid boundaries
    std::vector<std::pair<double, double>> gridXBounds = {
        {SPACE_X_MIN, SPACE_X_MAX / 2}, // Grid 1: x in [0, 5)
        {SPACE_X_MAX / 2, SPACE_X_MAX}  // Grid 2: x in [5, 10)
    };
    std::vector<std::pair<double, double>> gridYBounds = {
        {SPACE_Y_MIN, SPACE_Y_MAX / 2}, // Grid 1: y in [0, 5)
        {SPACE_Y_MAX / 2, SPACE_Y_MAX}  // Grid 2: y in [5, 10)
    };

    // Explore each grid sequentially
    for (const auto& xBounds : gridXBounds) {
        for (const auto& yBounds : gridYBounds) {
            std::unordered_set<std::string> visitedCells; // Track visited cells for each grid
            initializeParticles(particles, globalBestPosition, globalBestFitness, xBounds.first, xBounds.second, yBounds.first, yBounds.second, visitedCells);
            runPSOForGrid(particles, globalBestPosition, globalBestFitness, xBounds.first, xBounds.second, yBounds.first, yBounds.second, "particle_positions.csv");
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printResults(globalBestPosition, globalBestFitness, executionTime);

    return 0;
}