import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation

# Load city data
cities = pd.read_csv("city_coordinates.csv")
city_coords = cities[['X', 'Y', 'Z']].values
num_cities = len(city_coords)

# Load PSO iteration data
pso_data = pd.read_csv("particle_data.csv")

# Extract unique iterations
iterations = pso_data["Iteration"].unique()
print(f"Total iterations detected: {len(iterations)}")  # Debugging

# Set up figure and 3D subplot
fig = plt.figure(figsize=(12, 6))

# Left subplot: 3D route animation
ax1 = fig.add_subplot(121, projection='3d')
ax1.set_title("TSP Route Optimization (3D)")
ax1.set_xlabel("X")
ax1.set_ylabel("Y")
ax1.set_zlabel("Z")

# Right subplot: Fitness evolution
ax2 = fig.add_subplot(122)
ax2.set_title("Fitness Evolution")
ax2.set_xlabel("Iteration")
ax2.set_ylabel("Best Fitness")

# Plot cities as 3D scatter points with labels
city_scatter = ax1.scatter(city_coords[:, 0], city_coords[:, 1], city_coords[:, 2], c='red', label="Cities")
for i, (x, y, z) in enumerate(city_coords):
    ax1.text(x, y, z, f"{i}", fontsize=8, ha='right')  # Label each city with its index

# Initialize lines for particle routes
particle_routes = [ax1.plot([], [], [], 'b-', alpha=0.3)[0] for _ in range(4)]  # One line per particle

# Initialize line for the best route
best_route_line, = ax1.plot([], [], [], 'g-', linewidth=2, label="Best Route")
ax1.legend()

# Initialize fitness plot
iter_fitness_line, = ax2.plot([], [], 'b-', linewidth=2, label="Iteration Best Fitness")
fitness_line, = ax2.plot([], [], 'g-', linewidth=2, label="Global Best Fitness")
ax2.legend()
best_fitness_values = []
global_best_fitness_values = []

# Track the global best route and fitness
global_best_route = None
global_best_fitness = float('inf')

# Function to validate a route
def validate_route(route):
    return all(0 <= city < num_cities for city in route) and len(set(route)) == num_cities

# Function to reset plots after all iterations
def reset_plots():
    global best_fitness_values, global_best_route, global_best_fitness, global_best_fitness_values

    print("Resetting plots...")  # Debugging

    # Reset best route
    global_best_route = None
    global_best_fitness = float('inf')

    # Clear fitness values
    best_fitness_values = []
    global_best_fitness_values = []

    # Clear fitness plot
    fitness_line.set_data([], [])
    iter_fitness_line.set_data([], [])
    
    # Clear particle routes
    for route_line in particle_routes:
        route_line.set_data([], [])
        route_line.set_3d_properties([])

    # Clear best route line
    best_route_line.set_data([], [])
    best_route_line.set_3d_properties([])

    # Rescale axes
    ax2.relim()
    ax2.autoscale_view()

# Animation update function
def update(frame):
    global global_best_route, global_best_fitness

    print(f"Updating frame {frame}/{len(iterations)}")  # Debugging

    if frame >= len(iterations):
        reset_plots()
        return particle_routes + [best_route_line, fitness_line, iter_fitness_line]

    iteration = iterations[frame]
    iter_data = pso_data[pso_data["Iteration"] == iteration]

    # Get the best particle (min fitness) for the current iteration
    if not iter_data.empty:
        best_fitness = iter_data["Fitness"].min()
        best_particle = iter_data.loc[iter_data["Fitness"].idxmin()]

        best_route = [best_particle[f"City{i}"] for i in range(num_cities)]

        best_route = [int(city) for city in best_route]

        # best_route = iter_data.iloc[best_index, 2:2+num_cities].values.astype(int)
    # else:
    #     best_route = []

    # Validate the best route
        if validate_route(best_route):
            if best_fitness < global_best_fitness:
                global_best_fitness = best_fitness
                global_best_route = best_route
                print(f"New global best: {global_best_fitness}")

    # Update particle routes
    for i, route_line in enumerate(particle_routes):
        if i < len(iter_data):
            route = iter_data.iloc[i, 2:2+num_cities].values.astype(int)
            if validate_route(route):
                route_coords = city_coords[route]
                route_line.set_data(route_coords[:, 0], route_coords[:, 1])
                route_line.set_3d_properties(route_coords[:, 2])
            else:
                route_line.set_data([], [])
                route_line.set_3d_properties([])
        else:
            route_line.set_data([], [])
            route_line.set_3d_properties([])

    # Update the best route line
    if global_best_route is not None:
        best_route_coords = city_coords[global_best_route]
        best_route_line.set_data(best_route_coords[:, 0], best_route_coords[:, 1])
        best_route_line.set_3d_properties(best_route_coords[:, 2])
    else:
        best_route_line.set_data([], [])
        best_route_line.set_3d_properties([])

    # Update fitness plot
    if frame < len(iterations):
        global_best_fitness_values.append(global_best_fitness)
        best_fitness_values.append(best_fitness)

        print(f"Frame {frame}: Best Fitness = {global_best_fitness}")  # Debugging

        fitness_line.set_data(range(len(global_best_fitness_values)), global_best_fitness_values)
        iter_fitness_line.set_data(range(len(best_fitness_values)), best_fitness_values)
    ax2.relim()
    ax2.autoscale_view()

    return particle_routes + [best_route_line, fitness_line, iter_fitness_line]

# Create animation (disable blitting)
ani = animation.FuncAnimation(fig, update, frames=len(iterations) + 1, interval=200, blit=False)

plt.tight_layout()
plt.show()
