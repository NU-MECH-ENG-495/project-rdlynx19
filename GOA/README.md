# Grasshopper Optimization Algorithm
The algorithm, outline [here](https://github.com/Aminsed/SwarmBenchmark/tree/main/GOA), is based on Social Interaction, Gravity Force, and Wind Advection to explore the search space.

The pseudo code that the algorithm will be built on, described by [Aminsed](https://github.com/Aminsed/SwarmBenchmark/commits?author=Aminsed), is as follows:
```
Initialize a population of grasshoppers with random positions
while termination condition is not met:
    for each grasshopper:
        Calculate fitness value
        Update best solution found so far
    for each grasshopper:
        Update position using the position update equation
    Update decreasing coefficient c
```
