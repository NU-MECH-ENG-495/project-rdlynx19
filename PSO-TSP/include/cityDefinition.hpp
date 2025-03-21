#ifndef CITY_DEFINITION_HPP
#define CITY_DEFINITION_HPP

#include <iostream>
#include <tuple>

class City {
    private:
        int id;
        std::tuple<double, double, double> coordinates;
    public:
        City(int id): id(id){}
        ~City(){};
        std::tuple<double, double, double> getCoordinates() const;
        void setCoordinates(double x, double y, double z);
};

#endif