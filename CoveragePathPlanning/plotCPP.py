import csv
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
import numpy as np

# Parameters
search_radius = 5.0  # Search radius of the drones
drone_point_size = 10  # Size of the point representing the drone
frame_delay = 0.01  # Delay between frames (for animation)

# Read the CSV file
waypoints = []
with open("project/project-rdlynx19/CoveragePathPlanning/drone_waypoints.csv", "r") as file:
    reader = csv.reader(file)
    next(reader)  # Skip the header
    for row in reader:
        drone_id = int(row[0])
        x = float(row[1])
        y = float(row[2])
        z = float(row[3])  # Z is not used in 2D plotting
        waypoints.append((drone_id, x, y))

# Separate waypoints by drone ID
drone_paths = {}
for drone_id, x, y in waypoints:
    if drone_id not in drone_paths:
        drone_paths[drone_id] = []
    drone_paths[drone_id].append((x, y))

# Create a plot
fig, ax = plt.subplots()
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_title("Drone Search Paths with Coverage")
ax.set_aspect('equal')
ax.grid(True)

# Define colors for each drone
colors = ['r', 'g', 'b', 'm']  # Red, Green, Blue, Magenta

# Initialize drone positions and coverage
drone_positions = {}
coverage_patches = {}
for drone_id, path in drone_paths.items():
    drone_positions[drone_id] = path[0]  # Start at the first waypoint
    coverage_patches[drone_id] = Circle(path[0], search_radius, color=colors[drone_id], alpha=0.1)
    ax.add_patch(coverage_patches[drone_id])

# Function to update the plot
def update_plot(frame):
    for drone_id, path in drone_paths.items():
        if frame < len(path):
            x, y = path[frame]
            drone_positions[drone_id] = (x, y)
            coverage_patches[drone_id].center = (x, y)
            ax.scatter(x, y, color=colors[drone_id], s=drone_point_size)
    plt.pause(frame_delay)

# Animate the plot
max_frames = max(len(path) for path in drone_paths.values())
for frame in range(max_frames):
    update_plot(frame)

# Add legend
ax.legend([f"Drone {i}" for i in range(len(drone_paths))])

# Show the plot
plt.show()