#ifndef PARTICLE_DEFINITION_HPP
#define PARTICLE_DEFINITION_HPP

#include <vector>
#include <iostream>

class Particle {
    private:
        int id;
        std::vector<int> route;
        std::vector<double> velocity;
        std::vector<int> bestRoute;
        double bestFitness;

    public:
        Particle(int id){
            id = id;
        }
        ~Particle(){};

        std::vector<int> getRoute() const ;
        std::vector<double> getVelocity() const;
        std::vector<int> getBestRoute() const;
        double getBestFitness() const;

        void setRoute(std::vector<int>& newRoute);
        void setVelocity(std::vector<double> &newVelocity);
        void setBestRoute(std::vector<int> &newBestRoute);
        void setBestFitness(double &newBestFitness);

    };

#endif