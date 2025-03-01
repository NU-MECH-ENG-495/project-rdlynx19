import csv

# Input CSV file
input_file = "project/project-rdlynx19/CoveragePathPlanning/drone_waypoints.csv"

# Read the CSV file and group waypoints by drone ID
waypoints_by_drone = {}
with open(input_file, "r") as file:
    reader = csv.reader(file)
    next(reader)  # Skip the header
    ct = 1
    for row in reader:
        if ct % 5 == 0:
            drone_id = int(row[0])
            x = float(row[1])
            y = float(row[2])
            z = float(row[3])
            if drone_id not in waypoints_by_drone:
                waypoints_by_drone[drone_id] = []
            waypoints_by_drone[drone_id].append((x, y, z))
        ct += 1

# Write waypoints to separate files
for drone_id, waypoints in waypoints_by_drone.items():
    output_file = f"uav{drone_id + 1}.csv"  # Files named uav1.csv, uav2.csv, etc.
    with open(output_file, "w", newline="") as file:
        writer = csv.writer(file)
        for x, y, z in waypoints:
            writer.writerow([x, y, z])  # Write x, y, z in order
    print(f"Written {len(waypoints)} waypoints to {output_file}")

print("All files have been created.")