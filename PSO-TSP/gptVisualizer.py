import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Load city data
cities = pd.read_csv("city_coordinates.csv")
city_coords = cities[['X', 'Y']].values
num_cities = len(city_coords)

# Load PSO iteration data
pso_data = pd.read_csv("particle_data.csv")

# Extract unique iterations
iterations = pso_data["Iteration"].unique()

# Set up figure and subplots
fig, axes = plt.subplots(1, 2, figsize=(12, 6))

# Left subplot: Route animation
ax1 = axes[0]
ax1.set_title("TSP Route Optimization")
ax1.set_xlabel("X")
ax1.set_ylabel("Y")

# Right subplot: Fitness evolution
ax2 = axes[1]
ax2.set_title("Fitness Evolution")
ax2.set_xlabel("Iteration")
ax2.set_ylabel("Best Fitness")

# Initialize plots
city_scatter, = ax1.plot(city_coords[:, 0], city_coords[:, 1], 'ko', label="Cities")  # Cities as black dots
routes = [ax1.plot([], [], 'b-', alpha=0.3)[0] for _ in range(10)]  # Particle routes in blue
best_route_plot, = ax1.plot([], [], 'r-', linewidth=2, label="Best Route")  # Best route in red
ax1.legend()

fitness_line, = ax2.plot([], [], 'g-', linewidth=2, label="Best Fitness")  # Fitness plot
ax2.legend()
best_fitness_values = []

# Function to validate a route
def validate_route(route):
    return all(0 <= city < num_cities for city in route)

# Animation update function
def update(frame):
    iteration = iterations[frame]
    iter_data = pso_data[pso_data["Iteration"] == iteration]
    print(f"Iteration {iteration}, iter_data shape: {iter_data.shape}, columns: {iter_data.columns}")

    # Get the best particle (min fitness)
    best_index = iter_data["Fitness"].idxmin()  # Get index of the row with the minimum fitness value
    if 0 <= best_index < iter_data.shape[0]:
        best_route = iter_data.iloc[best_index, 2:2+num_cities].values.astype(int)  # Dynamically slice columns
    else:
        print(f"Invalid best_index: {best_index}")
        best_route = []

    # Validate the best route
    if validate_route(best_route):
        best_route_coords = city_coords[best_route]
        best_route_plot.set_data(best_route_coords[:, 0], best_route_coords[:, 1])
    else:
        print(f"Invalid best route: {best_route}")

    # Update particle routes (for each particle in the iteration)
    for i, route_line in enumerate(routes):
        if i < len(iter_data):
            route = iter_data.iloc[i, 2:2+num_cities].values.astype(int)  # Dynamically slice columns
            if validate_route(route):
                route_coords = city_coords[route]
                route_line.set_data(route_coords[:, 0], route_coords[:, 1])
            else:
                print(f"Invalid route for particle {i}: {route}")
                route_line.set_data([], [])
        else:
            route_line.set_data([], [])

    # Update fitness plot
    best_fitness = iter_data["Fitness"].min()  # Get the best fitness for the current iteration
    best_fitness_values.append(best_fitness)
    
    # Ensure x and y data have the same length
    x_data = range(len(best_fitness_values))
    y_data = best_fitness_values
    fitness_line.set_data(x_data, y_data)

    ax2.relim()
    ax2.autoscale_view()

    return routes + [best_route_plot, fitness_line]

# Create animation
ani = animation.FuncAnimation(fig, update, frames=len(iterations), interval=200, blit=True)

plt.show()