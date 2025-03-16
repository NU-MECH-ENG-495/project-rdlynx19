import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Constants
NUM_PARTICLES = 4  # Explicitly specify the number of particles

# Read the particle data and city coordinates
particle_data = pd.read_csv("particle_data.csv")
city_coords = pd.read_csv("city_coordinates.csv")

# Extract city coordinates
cities = city_coords[['X', 'Y']].values

# Determine the number of cities dynamically
NUM_CITIES = len(city_coords)

# Create a figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# Subplot 1: TSP Routes
ax1.set_xlim(-1, 11)  # Slightly larger than 10m to include margins
ax1.set_ylim(-1, 9)   # Slightly larger than 8m to include margins
ax1.set_xlabel("X (m)")
ax1.set_ylabel("Y (m)")
ax1.set_title("Particle Swarm Optimization for TSP")

# Plot the cities
ax1.scatter(cities[:, 0], cities[:, 1], c='red', label="Cities")
for i, (x, y) in enumerate(cities):
    ax1.text(x, y, f"{i}", fontsize=12, ha='right')

# Initialize lines for current routes and best route
current_lines = [ax1.plot([], [], lw=1, alpha=0.3, label=f"Particle {i}")[0] for i in range(NUM_PARTICLES)]
best_line = ax1.plot([], [], lw=2, c='green', label="Best Route")[0]

# Subplot 2: Fitness Over Time
ax2.set_xlim(0, len(particle_data['Iteration'].unique()))
ax2.set_ylim(0, particle_data['Fitness'].max() * 1.1)  # Add 10% margin
ax2.set_xlabel("Iteration")
ax2.set_ylabel("Fitness (Distance)")
ax2.set_title("Fitness Over Time")
fitness_line, = ax2.plot([], [], lw=2, c='blue', label="Best Fitness")

# Initialize fitness data
fitness_history = []

# Store the best route and fitness across all iterations
global_best_route = []
global_best_fitness = float('inf')

# Function to validate a route
def validate_route(route):
    return all(0 <= city < NUM_CITIES for city in route)

# Function to update the animation
def update(iteration):
    global global_best_route, global_best_fitness

    # Get the data for the current iteration
    iteration_data = particle_data[particle_data['Iteration'] == iteration]
    
    # Plot each particle's current route
    for i in range(NUM_PARTICLES):
        particle_route = iteration_data[iteration_data['ParticleID'] == i].iloc[:, 2:2+NUM_CITIES].values.flatten()
        
        # Check if the particle's route is valid
        if len(particle_route) > 0 and validate_route(particle_route):
            x = [cities[city][0] for city in particle_route]
            y = [cities[city][1] for city in particle_route]
            current_lines[i].set_data(x, y)
        else:
            # If the particle's route is invalid, hide it
            current_lines[i].set_data([], [])
    
    # Update the global best route and fitness
    if not iteration_data.empty:
        best_fitness = iteration_data['Fitness'].min()
        if best_fitness < global_best_fitness:
            global_best_fitness = best_fitness
            global_best_route = iteration_data.loc[iteration_data['Fitness'].idxmin()].iloc[:, 2:2+NUM_CITIES].values.flatten()
        
        # Update fitness history
        fitness_history.append(global_best_fitness)
        fitness_line.set_data(range(len(fitness_history)), fitness_history)
        
        # Print the best route and fitness
        print(f"Iteration {iteration}: Best Route = {global_best_route}, Fitness = {global_best_fitness}")
    
    # Plot the global best route
    if len(global_best_route) > 0 and validate_route(global_best_route):
        x = [cities[city][0] for city in global_best_route]
        y = [cities[city][1] for city in global_best_route]
        best_line.set_data(x, y)
    
    # Adjust fitness plot limits
    ax2.relim()
    ax2.autoscale_view()
    
    return current_lines + [best_line, fitness_line]

# Create the animation
ani = animation.FuncAnimation(fig, update, frames=particle_data['Iteration'].unique(), interval=200, blit=True)

# Save the animation as a GIF
ani.save("pso_tsp_animation.gif", writer="pillow")

# Show the animation
ax1.legend()
ax2.legend()
plt.tight_layout()
plt.show()