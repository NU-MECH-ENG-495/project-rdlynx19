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

void PSO::generateCityCoordinates(int numCities)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(-2.0, 8.0);
    std::uniform_real_distribution<> yDist(-5.0, 5.0);
    std::uniform_real_distribution<> zDist(0.0, 2.5);

    this->cityList.resize(numCities);

    for (int i = 0; i < numCities; i++)
    {
        this->cityList[i] = std::make_shared<City>(i);
        this->cityList[i]->setCoordinates(xDist(gen), yDist(gen), zDist(gen));
    }
}

void PSO::initializeDistanceMatrix()
{
    distanceMatrix.resize(cityList.size(), std::vector<double>(cityList.size(), 0.0));
    int numCities = cityList.size();
    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; j < numCities; j++)
        {
            if (i != j)
            {
                distanceMatrix[i][j] = euclideanDistance(this->cityList[i], this->cityList[j]);
            }
        }
    }
}

double PSO::calculateDistance(const std::vector<int> &route, int numCities)
{
    double distance = 0.0;
    for (int i = 0; i < numCities - 1; i++)
    {
        int city1 = route[i];
        int city2 = route[i + 1];
        distance += distanceMatrix[city1][city2];
    }
    int lastCity = route[numCities - 1];
    int firstCity = route[0];
    distance += distanceMatrix[lastCity][firstCity];
    return distance;
}

void PSO::updateBestFitness(std::shared_ptr<Particle> p, int numCities)
{
    double fitness = calculateDistance(p->getRoute(), numCities);
    if (fitness < p->getBestFitness())
    {
        p->setBestFitness(fitness);
        std::vector<int> newBestRoute = p->getRoute();
        p->setBestRoute(newBestRoute);
    }
    if (fitness < globalBestFitness)
    {
        globalBestFitness = fitness;
        globalBestRoute = p->getRoute();
    }
}

void PSO::initializeParticles(int numParticles, int numCities)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    this->particleList.resize(numParticles);

    for (int i = 0; i < numParticles; i++)
    {
        this->particleList[i] = std::make_shared<Particle>(i);

        std::vector<int> initializeRoute(numCities);
        std::iota(initializeRoute.begin(), initializeRoute.end(), 0);
        std::shuffle(initializeRoute.begin(), initializeRoute.end(), gen);
        this->particleList[i]->setRoute(initializeRoute);

        std::vector<double> initializeVelocity(numCities);
        for (auto &v : initializeVelocity)
        {
            v = dis(gen) * 2.0 - 1.0;
        }
        this->particleList[i]->setVelocity(initializeVelocity);

        this->particleList[i]->setBestRoute(initializeRoute);
        double fitness = calculateDistance(initializeRoute, numCities);
        this->particleList[i]->setBestFitness(fitness);

        updateBestFitness(this->particleList[i], numCities);
    }
}

void PSO::updateParticles(int iteration, std::ofstream &outFile, int numCities)
{
    std::vector<std::thread> threads;

    for (int pIdx = 0; pIdx < this->particleList.size(); pIdx++)
    {
        // Lambda Function
        threads.emplace_back([this, pIdx, iteration, numCities, &outFile]() {
            auto &p = this->particleList[pIdx];
            
            
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.0);

            std::vector<double> updatedVelocity = p->getVelocity();
            std::vector<int> currentBestRoute = p->getBestRoute();
            std::vector<int> currentRoute = p->getRoute();
            for (int i = 0; i < numCities; i++){
                double r1 = dis(gen);
                double r2 = dis(gen);

                updatedVelocity[i] = INERTIA_WEIGHT * updatedVelocity[i] +
                                    COGNITIVE_WEIGHT * r1 * (currentBestRoute[i] - currentRoute[i]) +
                                    SOCIAL_WEIGHT * r2 * (globalBestRoute[i] - currentRoute[i]);
            }
            p->setVelocity(updatedVelocity);
            for(int i = 0; i < NUM_CITIES; i++){
                int swapIndex = (static_cast<int>(std::abs(updatedVelocity[i])) % NUM_CITIES);
                std::swap(currentRoute[i], currentRoute[swapIndex]);
            }

            bool isValid = true;
            std::vector<bool> visited(NUM_CITIES, false);
            for(int city: currentRoute)
            {
                if(city < 0 or city >= NUM_CITIES or visited[city]){
                    isValid = false;
                    break;
                }
                visited[city] = true;
            }

            if (isValid){
                p->setRoute(currentRoute);
                double currentFitness = calculateDistance(currentRoute, numCities);

                std::lock_guard<std::mutex> lock(globalMutex);
                if(currentFitness < p->getBestFitness()){
                    p->setBestFitness(currentFitness);
                    p->setBestRoute(currentRoute);
                }
                if(currentFitness < globalBestFitness){
                    globalBestFitness = currentFitness;
                    globalBestRoute = currentRoute;
                }

                // updateBestFitness(p, numCities);
            }
            std::lock_guard<std::mutex> lock(globalMutex);
            outFile << iteration << "," << pIdx;
            for (int city : p->getRoute())
            {
                outFile << "," << city;
            }
            outFile << "," << calculateDistance(p->getRoute(), NUM_CITIES) << "\n"; 
        });
    }
    
    for (auto &t : threads)
    {
        t.join();
    }
}

void PSO::runPSO(std::ofstream &outFile, int numCities)
{
    for (int iter = 0; iter < MAX_ITERATIONS; iter++)
    {
        updateParticles(iter, outFile, numCities);
    }
}

void PSO::printResults(double executionTime)
{
    std::cout << "Best Path: ";
    for (int city : globalBestRoute)
    {
        std::cout << city << " ";
    }
    std::cout << std::endl;
    std::cout << "Best Distance: " << globalBestFitness << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Execution Time: " << executionTime << " milliseconds" << std::endl;
}