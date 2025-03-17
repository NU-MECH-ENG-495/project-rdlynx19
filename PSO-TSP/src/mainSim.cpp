#include "psoDefinition.hpp"
#include "utils.hpp"
#include <chrono>
#include <fstream>


int main(){
    PSO algoSim;
    algoSim.generateCityCoordinates(NUM_CITIES);
    algoSim.initializeDistanceMatrix();
    
    std::vector<std::shared_ptr<City>> cityList = algoSim.getCityList();
    std::ofstream coordFile("../csv/city_coordinates.csv");
    coordFile << "City,X,Y,Z\n";
    for(int i = 0; i < NUM_CITIES; i++){
        coordFile << i << "," << std::get<0>(cityList[i]->getCoordinates()) << "," << std::get<1>(cityList[i]->getCoordinates()) << "," << std::get<2>(cityList[i]->getCoordinates()) << "\n";
    }
    coordFile.close();


    std::ofstream outFile("../csv/particle_data.csv");
    outFile << "Iteration,ParticleID";
    for (int i = 0; i < NUM_CITIES; i++)
    {
        outFile << ",City" << i;
    }
    outFile << ",Fitness\n";

    auto start = std::chrono::high_resolution_clock::now();
    algoSim.initializeParticles(NUM_PARTICLES, NUM_CITIES);
    algoSim.runPSO(outFile, NUM_CITIES);
    auto end = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    algoSim.printResults(executionTime);

    saveBestRouteCoordinates(algoSim.getGlobalBestRoute(), cityList);
    saveRouteCoordinatesXYZ(algoSim.getGlobalBestRoute(), cityList);
    outFile.close();
    return 0;
}

