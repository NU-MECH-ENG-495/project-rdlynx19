import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv

# Read particle positions from CSV
def read_particle_positions(filename):
    positions = []
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        iteration_positions = []
        for row in reader:
            if row and row[0].startswith("Iteration"):
                if iteration_positions:
                    positions.append(iteration_positions)
                    iteration_positions = []
            elif row:
                iteration_positions.append([float(x) for x in row])
        if iteration_positions:
            positions.append(iteration_positions)
    return positions

# Animation function
def update(frame, positions, scat, texts):
    # Update scatter plot with new positions
    scat.set_offsets(positions[frame][:, :2])  # Use only the first 2 dimensions for 2D plot

    # Update text annotations (optional: display particle IDs or other info)
    for i, text in enumerate(texts):
        text.set_position((positions[frame][i, 0], positions[frame][i, 1]))
    return scat, *texts

# Main function
def visualize_particle_positions(filename):
    positions = read_particle_positions(filename)
    positions = np.array(positions)  # Convert to numpy array for easier handling

    # Extract only the first 2 dimensions for 2D visualization
    positions = positions[:, :, :2]

    # Set up the plot
    fig, ax = plt.subplots()
    ax.set_xlim(np.min(positions[:, :, 0]), np.max(positions[:, :, 0]))
    ax.set_ylim(np.min(positions[:, :, 1]), np.max(positions[:, :, 1]))

    # Initialize scatter plot with color-coded particles
    num_particles = positions.shape[1]
    colors = plt.cm.viridis(np.linspace(0, 1, num_particles))  # Assign unique colors to particles

    # Initialize scatter plot with the first frame's positions
    scat = ax.scatter(positions[0][:, 0], positions[0][:, 1], c=colors, s=50, label='Particles')

    # Add text annotations for particle IDs (optional)
    texts = [ax.text(positions[0][i, 0], positions[0][i, 1], f'P{i}', fontsize=8, color='black') for i in range(num_particles)]

    # Add legend
    ax.legend(loc='upper right')

    # Create animation
    ani = animation.FuncAnimation(
        fig, update, frames=len(positions),
        fargs=(positions, scat, texts),
        interval=100, blit=True
    )

    # Set axis labels
    ax.set_xlabel('X Axis')
    ax.set_ylabel('Y Axis')
    ax.set_title('Particle Swarm Optimization (2D Animation)')

    # Show the plot
    plt.show()

# Run the visualization
visualize_particle_positions("particle_positions.csv")