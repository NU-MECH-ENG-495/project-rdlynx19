# Trajectory Generation for Crazyflie Drones
Authors: [Pushkar Dave](https://www.pushkardave.com) and [Grayson Snyder](https://www.snydergi.github.io)

This repository contains the implementation of two distinct algorithms for the [CrazyFlie](https://www.bitcraze.io/products/old-products/crazyflie-2-1/) drones, as a part of the final project for ME495: Advanced Programming Concepts in C++

The project is divided into two main components:
1. Coverage Path Planning: Implemented by Grayson Snyder, 
2. Particle Swarm Optimization(PSO) for Traveling Salesman Problem (TSP): Implemented by Pushkar Dave, this algorithm solves TSP using PSO to generate an optimal route for a Crazyflie to visit a set of waypoints. The [uav-trajectories](https://www.github.com/whoenig/uav_trajectories) package is used to generate a trajectory from the optimal route of waypoints.

## Overview
### Coverage Path Planning

### Particle Swarm Optimization(PSO) for TSP
- The algorithm randomly initializes a set of waypoints (cities), with the goal of finding the shortest possible route that visits all cities exactly once and returns to the starting point
- The velocity update follows the standard PSO equation, while the position update uses the velocity vector to determine the swap index for modifying the route
- Using 4 particles and 100 iterations, the algorithm converges to a feasible solution, which is then converted into a trajectory and deployed on the Crazyflie

## References
- [SwarmBenchmark (Amin Sedaghat)](https://github.com/Aminsed/SwarmBenchmark?tab=readme-ov-file) 
- [CrazySwarm2 (IMRC Lab)](https://imrclab.github.io/crazyswarm2/)

## License
The project is licensed under the MIT License.



