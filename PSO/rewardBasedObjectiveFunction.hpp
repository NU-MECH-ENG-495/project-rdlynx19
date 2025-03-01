#ifndef REWARD_BASED_OBJECTIVE_FUNCTION_H
#define REWARD_BASED_OBJECTIVE_FUNCTION_H

#include <cmath>
#include <vector>
#include <unordered_set>
#include <string>

constexpr int NUM_PARTICLES = 4;
constexpr int MAX_ITERATIONS = 20;
constexpr double COGNITIVE_WEIGHT = 1.49; // Cognitive weight (c1) in the velocity update equation
constexpr double SOCIAL_WEIGHT = 1.49;    // Social weight (c2) in the velocity update equation
constexpr double INERTIA_WEIGHT = 0.729;  // Inertia weight (w) in the velocity update equation

// Reward for visiting a new cell
constexpr double NEW_CELL_REWARD = 0.1;  // Reward for visiting a new cell

// Penalty for revisiting the same cell
constexpr double REPEAT_CELL_PENALTY = 0.05;  // Penalty for revisiting the same cell

// Global set to track visited cells
std::unordered_set<std::string> visitedCells;

// Function to generate a unique key for a cell
std::string getCellKey(const std::vector<double>& position) {
    int cellX = static_cast<int>(position[0]); // Convert X position to cell index
    int cellY = static_cast<int>(position[1]); // Convert Y position to cell index
    return std::to_string(cellX) + "," + std::to_string(cellY); // Create a unique key for the cell
}

// Griewank function with exploration reward
constexpr int DIMENSIONS = 3; // Number of dimensions in the optimization problem

double objectiveFunction(const std::vector<double>& position) {
    double x = position[0];
    double y = position[1];
    double z = position[2];

    // Calculate the Griewank function value
    double griewankValue = 1.0 + (x * x + y * y + z * z) / 4000.0 - cos(x) * cos(y / sqrt(2.0)) * cos(z / sqrt(3.0));

    // Check if the current cell is new
    std::string cellKey = getCellKey(position);
    if (visitedCells.find(cellKey) == visitedCells.end()) {
        // If the cell is new, add a reward to the fitness
        griewankValue -= NEW_CELL_REWARD; // Subtract because we are minimizing the fitness
        visitedCells.insert(cellKey); // Mark the cell as visited
    } else {
        // If the cell is revisited, add a penalty to the fitness
        griewankValue += REPEAT_CELL_PENALTY; // Add because we are minimizing the fitness
    }

    return griewankValue;
}

#endif