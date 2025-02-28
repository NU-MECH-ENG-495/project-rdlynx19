import csv
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# File path to the CSV file
csv_file = "/home/gis/Documents/AdvancedCpp/project/project-rdlynx19/GOA/results.csv"

# Lists to store data
iterations = []
global_best_fitness = []
grasshopper_positions = []  # List of lists to store (x, y) positions for each grasshopper

# Read the CSV file
with open(csv_file, mode="r") as file:
    reader = csv.reader(file)
    headers = next(reader)  # Read the header row

    # Determine the number of grasshoppers from the headers
    num_grasshoppers = (len(headers) - 2) // 2  # Subtract 2 for "Iteration" and "GlobalBestFitness"

    # Initialize lists for each grasshopper's positions
    for _ in range(num_grasshoppers):
        grasshopper_positions.append({"x": [], "y": []})

    # Read the data rows
    for row in reader:
        iterations.append(int(row[0]))  # Iteration
        global_best_fitness.append(float(row[1]))  # Global Best Fitness

        # Read positions for each grasshopper
        for i in range(num_grasshoppers):
            x_index = 2 + 2 * i  # X position column index
            y_index = x_index + 1  # Y position column index
            grasshopper_positions[i]["x"].append(float(row[x_index]))
            grasshopper_positions[i]["y"].append(float(row[y_index]))

# Set up the figure and axis
fig, ax = plt.subplots(figsize=(10, 6))
ax.set_xlim(min(min(pos["x"]) for pos in grasshopper_positions) - 1, max(max(pos["x"]) for pos in grasshopper_positions) + 1)
ax.set_ylim(min(min(pos["y"]) for pos in grasshopper_positions) - 1, max(max(pos["y"]) for pos in grasshopper_positions) + 1)
ax.set_title("Grasshopper Positions Over Iterations")
ax.set_xlabel("X Position")
ax.set_ylabel("Y Position")
ax.grid(True)

# Create a list of scatter plots for each grasshopper
scatters = [ax.scatter([], [], label=f"Grasshopper {i + 1}") for i in range(num_grasshoppers)]
ax.legend()

# Animation function
def animate(frame):
    ax.clear()  # Clear the previous frame
    ax.set_xlim(min(min(pos["x"]) for pos in grasshopper_positions) - 0.5, max(max(pos["x"]) for pos in grasshopper_positions) + 0.5)
    ax.set_ylim(min(min(pos["y"]) for pos in grasshopper_positions) - 0.5, max(max(pos["y"]) for pos in grasshopper_positions) + 0.5)
    ax.set_title(f"Iteration {frame + 1}")
    ax.set_xlabel("X Position")
    ax.set_ylabel("Y Position")
    ax.grid(True)

    # Plot the current positions of all grasshoppers
    for i in range(num_grasshoppers):
        ax.scatter(
            grasshopper_positions[i]["x"][frame],
            grasshopper_positions[i]["y"][frame],
            label=f"Grasshopper {i + 1}",  # Always include the label
        )

    if frame == 0:
        ax.legend()  # Add legend only once (for the first frame)

# Create the animation
ani = animation.FuncAnimation(
    fig,
    animate,
    frames=len(iterations),
    interval=0.5,  # Delay between frames in milliseconds
    repeat=False,
)

# Show the animation
plt.show()