# Trajectory Generation for Crazyflie Drones
Authors: [Pushkar Dave](https://www.pushkardave.com) and [Grayson Snyder](https://www.snydergi.github.io)

This repository contains the implementation of two distinct algorithms for the [CrazyFlie](https://www.bitcraze.io/products/old-products/crazyflie-2-1/) drones, as a part of the final project for ME495: Advanced Programming Concepts in C++

The project is divided into two main components:
1. Coverage Path Planning: Implemented by Grayson Snyder, 
2. Particle Swarm Optimization(PSO) for Traveling Salesman Problem (TSP): Implemented by Pushkar Dave, this algorithm solves TSP using PSO to generate an optimal route for a Crazyflie to visit a set of waypoints. The [uav-trajectories](https://www.github.com/whoenig/uav_trajectories) package is used to generate a trajectory from the optimal route of waypoints.

## Overview
### Coverage Path Planning

### Particle Swarm Optimization(PSO) for TSP




## License
The project is licensed under the MIT License. See the LICENSE file for more details.


The main reference comes from a Swarm Benchmark [article](https://medium.com/@amin32846/swarm-intelligence-showdown-a-benchmarking-study-a94cc2ca598c) and [repository](https://github.com/Aminsed/SwarmBenchmark?tab=readme-ov-file) which contains the implementation of these algorithms in C++, OpenMP, CUDA, and Thrust. \
However, my main idea is to implement these algorithms in a single threaded C++ approach. (possibly implement a few of them in a multi-threading).

The CrazyFlie has a swarming [package](https://imrclab.github.io/crazyswarm2/index.html) available in Python, which would allow me to communicate with the swarm, but position or trajectory of the drones will be determined by the swarming algorithm being implemented. The package also has a simulation environment, which can be used to test the algorithms before deploying them on real drones.
Additionally, it may require use of positioning systems like Motion Capture, which I am currently using for my winter project. 

My ambitious end goal is to create a clean and well-documented implementation, release it as an open-source repository, as a reference and baseline for studying swarm algorithms and deploying them on micro aerial vehicles.


