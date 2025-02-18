#include <stdio.h>
#include <vector>
#include <random>

#define xDimLB 0.0f
#define xDimUB 1.0f
#define yDimLB 0.0f
#define yDimUB 1.0f
#define zDimLB 0.0f
#define zDimUB 1.0f
#define quatLB -1.0f
#define quatUB 1.0f

struct Grasshopper
{
    std::vector<std::pair<std::tuple<float>, std::tuple<float>>> position;
    int ID;
};

std::vector<Grasshopper> initGrasshopper(int numGrasshopper = 4)
{
    std::vector<Grasshopper> uavs;
    for (int i = 0; i < numGrasshopper; i++)
    {
        Grasshopper uav;

        std::random_device rd;                                         // Seed for the random number engine
        std::mt19937 gen(rd());                                        // Mersenne Twister engine
        std::uniform_real_distribution<float> xdis(xDimLB, xDimUB);    // Uniform distribution for x between 0.0 and 1.0
        std::uniform_real_distribution<float> ydis(yDimLB, yDimUB);    // Uniform distribution for y between 0.0 and 1.0
        std::uniform_real_distribution<float> zdis(zDimLB, zDimUB);    // Uniform distribution for z between 0.0 and 1.0
        std::uniform_real_distribution<float> quatdis(quatLB, quatUB); // Uniform distribution for quaternion between -1.0 and 1.0
        // Generate random floats
        float x = xdis(gen);
        float y = ydis(gen);
        float z = zdis(gen);
        float qx = quatdis(gen);
        float qy = quatdis(gen);
        float qz = quatdis(gen);
        float qw = quatdis(gen);
        uav.position.emplace_back(std::make_pair(std::make_tuple(x, y, z), std::make_tuple(qx, qy, qz, qw)));
        uav.ID = i;
        uavs.push_back(uav);
    }
    return uavs;
}

int main(int argc, char *argv[])
{
    std::vector<Grasshopper> uavs;
    uavs = initGrasshopper();
    return 0;
}
