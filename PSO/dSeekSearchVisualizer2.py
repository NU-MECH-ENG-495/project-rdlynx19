import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv

# Read particle positions, global best, and grid best data from CSV
def read_particle_positions(filename):
    positions = []
    global_best_positions = []
    global_best_fitness = []
    grid_best_positions = []
    grid_best_fitness = []

    with open(filename, 'r') as file:
        reader = csv.reader(file)
        iteration_positions = []
        for row in reader:
            if row and row[0].startswith("Iteration"):
                if iteration_positions:
                    positions.append(iteration_positions)
                    iteration_positions = []
            elif row and row[0].startswith("Global Best"):
                global_best_positions.append([float(x) for x in row[1:-1]])  # Extract position (excluding label and fitness)
                global_best_fitness.append(float(row[-1]))  # Last value is the fitness
            elif row and row[0].startswith("Grid Best"):
                grid_best_positions.append([float(x) for x in row[1:-1]])  # Extract position (excluding label and fitness)
                grid_best_fitness.append(float(row[-1]))  # Last value is the fitness
            elif row:
                iteration_positions.append([float(x) for x in row])

        if iteration_positions:
            positions.append(iteration_positions)

    return np.array(positions), np.array(global_best_positions), np.array(global_best_fitness), np.array(grid_best_positions), np.array(grid_best_fitness)

# Function to create a heatmap of visited areas
def create_heatmap(positions, grid_size=10, cell_size=1):
    # Initialize the heatmap grid
    heatmap = np.zeros((grid_size, grid_size))

    # Iterate through all positions and update the heatmap
    for iteration in positions:
        for pos in iteration:
            x, y = int(pos[0] // cell_size), int(pos[1] // cell_size)
            if 0 <= x < grid_size and 0 <= y < grid_size:
                heatmap[y, x] += 1  # Increment the visit count for the cell

    return heatmap

# Animation function
def update(frame, positions, global_best_positions, grid_best_positions, scat, best_scat, grid_scat, fitness_line, global_best_fitness, ax1, heatmap_img, search_radius):
    # Update scatter plot with new positions
    scat.set_offsets(positions[frame][:, :2])  # Use only first 2 dimensions for 2D plot

    # Update global best position marker
    best_scat.set_offsets([global_best_positions[frame][0], global_best_positions[frame][1]])

    # Update grid best position marker
    grid_scat.set_offsets([grid_best_positions[frame][0], grid_best_positions[frame][1]])

    # Update fitness line graph
    fitness_line.set_data(np.arange(frame + 1), global_best_fitness[:frame + 1])

    # Update heatmap
    heatmap = create_heatmap(positions[:frame + 1])
    heatmap_img.set_data(heatmap)

    # Update search radius circles
    for i, circle in enumerate(search_radius):
        circle.center = (positions[frame][i, 0], positions[frame][i, 1])

    # Update title to show current iteration
    ax1.set_title(f'Particle Swarm Optimization (Iteration {frame + 1})')

    return scat, best_scat, grid_scat, fitness_line, heatmap_img, *search_radius

# Main function
def visualize_particle_positions(filename):
    # Read data
    positions, global_best_positions, global_best_fitness, grid_best_positions, grid_best_fitness = read_particle_positions(filename)

    # Extract only the first 2 dimensions for 2D visualization
    positions = positions[:, :, :2]

    # Set up the figure with two subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

    # Set up scatter plot
    ax1.set_xlim(0, 10)  # X-axis limits for the search space
    ax1.set_ylim(0, 10)  # Y-axis limits for the search space
    ax1.set_xlabel('X Axis')
    ax1.set_ylabel('Y Axis')
    ax1.set_title('Particle Swarm Optimization (2D Animation)')

    # Draw grid boundaries
    ax1.axvline(x=5, color='gray', linestyle='--', linewidth=1)  # Vertical line at x=5
    ax1.axhline(y=5, color='gray', linestyle='--', linewidth=1)  # Horizontal line at y=5

    # Label grids
    ax1.text(2.5, 2.5, 'Grid 1', fontsize=12, ha='center', va='center', color='blue')
    ax1.text(7.5, 2.5, 'Grid 2', fontsize=12, ha='center', va='center', color='blue')
    ax1.text(2.5, 7.5, 'Grid 3', fontsize=12, ha='center', va='center', color='blue')
    ax1.text(7.5, 7.5, 'Grid 4', fontsize=12, ha='center', va='center', color='blue')

    # Initialize scatter plot
    num_particles = positions.shape[1]
    colors = plt.cm.viridis(np.linspace(0, 1, num_particles))  # Assign unique colors to particles
    scat = ax1.scatter(positions[0][:, 0], positions[0][:, 1], c=colors, s=50, label='Particles')

    # Add global best position marker
    best_scat = ax1.scatter(global_best_positions[0][0], global_best_positions[0][1], c='red', s=100, marker='X', label='Global Best')

    # Add grid best position marker
    grid_scat = ax1.scatter(grid_best_positions[0][0], grid_best_positions[0][1], c='green', s=100, marker='*', label='Grid Best')

    # Add search radius circles
    search_radius = [plt.Circle((0, 0), 1, color='r', fill=False, alpha=0.5) for _ in range(num_particles)]
    for circle in search_radius:
        ax1.add_patch(circle)

    # Add legend
    ax1.legend(loc='upper right')

    # Initialize heatmap
    heatmap = create_heatmap(positions)
    heatmap_img = ax1.imshow(heatmap, extent=[0, 10, 0, 10], origin='lower', cmap='hot', alpha=0.5)

    # Add colorbar for the heatmap
    cbar = plt.colorbar(heatmap_img, ax=ax1, orientation='vertical', label='Visit Frequency')
    cbar.set_label('Visit Frequency', rotation=270, labelpad=15)

    # Set up fitness plot
    ax2.set_xlim(0, len(global_best_fitness))
    ax2.set_ylim(np.min(global_best_fitness) * 0.9, np.max(global_best_fitness) * 1.1)
    ax2.set_xlabel('Iteration')
    ax2.set_ylabel('Global Best Fitness')
    ax2.set_title('Global Best Fitness Over Time')

    # Initialize fitness plot line
    fitness_line, = ax2.plot([], [], 'r-', label="Global Best Fitness")  # Line2D object
    ax2.legend()

    # Create animation
    ani = animation.FuncAnimation(
        fig, update, frames=len(positions),
        fargs=(positions, global_best_positions, grid_best_positions, scat, best_scat, grid_scat, fitness_line, global_best_fitness, ax1, heatmap_img, search_radius),
        interval=200, blit=True
    )

    # Show the plot
    plt.show()

# Run the visualization
visualize_particle_positions("particle_positions.csv")