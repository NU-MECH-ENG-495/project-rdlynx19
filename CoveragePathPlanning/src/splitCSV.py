import csv

# Input CSV file
input_file = "project/project-rdlynx19/CoveragePathPlanning/drone_waypoints.csv"

# Read the CSV file and group waypoints by drone ID
waypoints_by_drone = {}
with open(input_file, "r") as file:
    reader = csv.reader(file)
    next(reader)  # Skip the header
    for row in reader:
        drone_id = int(row[0])
        x = float(row[1])
        y = float(row[2])
        z = float(row[3])
        if drone_id not in waypoints_by_drone:
            waypoints_by_drone[drone_id] = []
        waypoints_by_drone[drone_id].append((x, y, z))

# Write waypoints to separate files, skipping duplicates
for drone_id, waypoints in waypoints_by_drone.items():
    output_file = f"uav{drone_id + 1}.csv"  # Files named uav1.csv, uav2.csv, etc.
    previous_waypoint = None
    with open(output_file, "w", newline="") as file:
        writer = csv.writer(file)
        for waypoint in waypoints:
            # Skip if the current waypoint is the same as the previous one
            if waypoint != previous_waypoint:
                writer.writerow(waypoint)  # Write x, y, z in order
                previous_waypoint = waypoint
    print(f"Written {len(waypoints)} waypoints to {output_file} (duplicates skipped)")

print("All files have been created.")