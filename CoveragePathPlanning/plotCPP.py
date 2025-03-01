import csv
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
import numpy as np

# Parameters
search_radius = 5.0  # Search radius of the drones
drone_point_size = 10  # Size of the point representing the drone

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

# Define colors for each drone
colors = ['r', 'g', 'b', 'm']  # Red, Green, Blue, Magenta

# Plot paths and coverage for each drone
for drone_id, path in drone_paths.items():
    color = colors[drone_id % len(colors)]  # Assign a color to the drone
    x_coords, y_coords = zip(*path)  # Unzip the path into x and y coordinates

    # Plot the path
    ax.plot(x_coords, y_coords, color=color, linestyle='-', linewidth=1, label=f"Drone {drone_id}")

    # Plot the coverage (circles) and drone positions (points)
    for i, (x, y) in enumerate(path):
        # Draw the search radius circle
        circle = Circle((x, y), search_radius, color=color, alpha=0.1)
        ax.add_patch(circle)

        # Draw the drone position (smaller point)
        ax.scatter(x, y, color=color, s=drone_point_size)

        # Update the plot dynamically (optional, for animation effect)
        plt.pause(1)  # Pause to visualize the movement

# Add legend and grid
ax.legend()
ax.grid(True)
ax.set_aspect('equal', adjustable='box')

# Show the plot
plt.show()