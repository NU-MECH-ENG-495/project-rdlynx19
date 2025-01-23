# Project Description
Title: Swarm Algorithms Implementation on CrazyFlie Drones
Author: Pushkar Dave

The aim of the project is to implement the set of popular swarm algorithms in C++, and then deploy them on a set of [CrazyFlie](https://www.bitcraze.io/products/old-products/crazyflie-2-1/) drones.

![crazy-swarm](/images/swarm.jpg)

Possible algorithms are:
- Particle Swarm Optimisation (PSO)
- Firefly Algorithm (FA)
- Moth-Flame Optimization (MFO) Algorithm
- Grey Wolf Optimizer (GWO)
- Whale Optimization Algorithm (WOA)
- Artificial Bee Colony (ABC) Algorithm
- Dragonfly Algorithm (DA)
- Salp Swarm Algorithm (SSA)
- Grasshopper Optimization Algorithm (GOA)
- Ant Colony Optimization (ACO)

The main reference comes from a Swarm Benchmark [article](https://medium.com/@amin32846/swarm-intelligence-showdown-a-benchmarking-study-a94cc2ca598c) and [repository](https://github.com/Aminsed/SwarmBenchmark?tab=readme-ov-file) which contains the implementation of these algorithms in C++, OpenMP, CUDA, and Thrust. \
However, my main idea is to implement these algorithms in a single threaded C++ approach. (possibly implement a few of them in a multi-threading).

The CrazyFlie has a swarming [package](https://imrclab.github.io/crazyswarm2/index.html) available in Python, which would allow me to communicate with the swarm, but position or trajectory of the drones will be determined by the swarming algorithm being implemented. The package also has a simulation environment, which can be used to test the algorithms before deploying them on real drones.
Additionally, it may require use of positioning systems like Motion Capture, which I am currently using for my winter project. 

My ambitious end goal is to create a clean and well-documented implementation, release it as an open-source repository, as a reference and baseline for studying swarm algorithms and deploying them on micro aerial vehicles.


