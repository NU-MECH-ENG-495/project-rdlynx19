# Particle Swarm Optimization (PSO)

The [PSO algorithm](https://github.com/Aminsed/SwarmBenchmark/tree/main/PSO/None-TSP), is based on Inertia Component, Cognitive Component and Social Component for exploring the search space.

Pseudo code for the algorithm is as follows, based on this [repository](https://github.com/Aminsed/SwarmBenchmark/tree/main) by Aminsed.

```
Initialize a population of particles with random positions and velocities
while termination condition is not met:
    for each particle:
        Calculate fitness value
        if fitness value is better than the personal best (p_best):
            Update personal best (p_best)
        if fitness value is better than the global best (g_best):
            Update global best (g_best)
    for each particle:
        Update velocity using the velocity update equation
        Update position using the position update equation
```