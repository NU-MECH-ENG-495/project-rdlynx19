# Trajectory Generation for Crazyflie Drones
Authors: [Pushkar Dave](https://www.pushkardave.com) and [Grayson Snyder](https://www.snydergi.github.io)

This repository contains the implementation of two distinct algorithms for the [CrazyFlie](https://www.bitcraze.io/products/old-products/crazyflie-2-1/) drones, as a part of the final project for ME495: Advanced Programming Concepts in C++

The project is divided into two main components:
1. Coverage Path Planning: Implemented by Grayson Snyder, this algorithm uses four Crazyflies to thoroughly explore a specified search area divided into four equivalent quadrants.
2. Particle Swarm Optimization(PSO) for Traveling Salesman Problem (TSP): Implemented by Pushkar Dave, this algorithm solves TSP using PSO to generate an optimal route for a Crazyflie to visit a set of waypoints. 

For both methods of waypoint generation, the [uav-trajectories](https://www.github.com/whoenig/uav_trajectories) package is used to generate the trajectory for the four Crazyflies in the case of CPP, and from the optimal route of waypoints for the PSO-TSP problem for a single Crazyflie.

## Overview
### Coverage Path Planning
- The goal of Coverage Path Planning is to ensure a complete search of the task space.
- The task space is divided into four equal quadrants, with one Crazyflie assigned to each. The search is performed in a roughly S shaped pattern, with the waypoints informing the trajectory being placed in a grid of rows and columns.
- This implementation was designed to include slight overlap of search areas by each Crazyflie to ensure total coverage.

### Particle Swarm Optimization(PSO) for TSP
- The algorithm randomly initializes a set of waypoints (cities), with the goal of finding the shortest possible route that visits all cities exactly once and returns to the starting point
- The velocity update follows the standard PSO equation, while the position update uses the velocity vector to determine the swap index for modifying the route
- Using 4 particles and 100 iterations, the algorithm converges to a feasible solution, which is then converted into a trajectory and deployed on the Crazyflie

### Visualizer
- A small C++ ROS2 Jazzy node was developed to aid in the visualization of the created trajectories.
- When used with the crazyswarm sim and RViz2, the messages for each Crazyflie's `tf` are used to create a marker array, leaving a trail behind the frame as it moves through the task space.

## References
- [SwarmBenchmark (Amin Sedaghat)](https://github.com/Aminsed/SwarmBenchmark?tab=readme-ov-file) 
- [CrazySwarm2 (IMRC Lab)](https://imrclab.github.io/crazyswarm2/)
- [CMU Coverage Path Planning](https://www.ri.cmu.edu/pub_files/pub4/choset_howie_1997_3/choset_howie_1997_3.pdf)

## License
The project is licensed under the MIT License.



