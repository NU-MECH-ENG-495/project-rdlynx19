#include <gtest/gtest.h>
#include "psoDefinition.hpp"
#include <random>

class PSOTest : public::testing::Test {
    protected:
        PSO testAlgo;

        void SetUp() override {
            int numCities = 40, numParticles = 4;
            testAlgo.generateCityCoordinates(numCities);
            testAlgo.initializeDistanceMatrix();
            testAlgo.initializeParticles(numParticles, numCities);
        }

        void TearDown() override {

        }
};

TEST_F(PSOTest, CheckCityInitialization) {
    EXPECT_EQ(testAlgo.getCityList().size(), 40);
}

TEST_F(PSOTest, CheckParticleInitialization) {
    EXPECT_EQ(testAlgo.getParticleList().size(), 4);
}

TEST_F(PSOTest, CheckValidRoute) {
    EXPECT_EQ(testAlgo.getParticleList()[0]->getRoute().size(), 40);
}

