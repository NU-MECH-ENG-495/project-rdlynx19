#ifndef PSO_DEFINITION_HPP
#define PSO_DEFINITION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <mutex>
#include <thread>
#include "cityDefinition.hpp"
#include "particleDefinition.hpp"
#include "ObjectiveFunction.hpp"

class PSO {
    private:
        double globalBestFitness = std::numeric_limits<double>::max();
        std::vector<int> globalBestRoute;
        std::vector<std::vector<double>> distanceMatrix;
        std::vector<std::shared_ptr<Particle>> particleList;
        std::vector<std::shared_ptr<City>> cityList;
        std::mutex globalMutex;
        
    public:
        PSO(){};
        ~PSO(){};
        void generateCityCoordinates(int numCities);
        void initializeDistanceMatrix();
        double calculateDistance(const std::vector<int> &route, int numCities);
        void updateBestFitness(std::shared_ptr<Particle> p, int numCities);
        void initializeParticles(int numParticles, int numCities);
        void updateParticles(int iteration, std::ofstream &outFile, int numCities);
        void runPSO(std::ofstream &outFile, int numCities);
        void printResults(double executionTime);

        std::vector<std::shared_ptr<City>> getCityList() const {return cityList;}
        std::vector<int> getGlobalBestRoute () const {return globalBestRoute; }
        std::vector<std::shared_ptr<Particle>> getParticleList() const {return particleList;}
};

#endif