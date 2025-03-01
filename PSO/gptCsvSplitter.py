import csv

# Read input CSV file and process the data
input_filename = 'particle_positions.csv'  # The name of your input CSV file
output_filenames = ['UAV1.csv', 'UAV2.csv', 'UAV3.csv', 'UAV4.csv']

# Initialize lists to hold the positions for each UAV
uav_positions = {f'UAV{i}': [] for i in range(1, 5)}

# Read and process the input CSV file
with open(input_filename, 'r') as file:
    reader = csv.reader(file)
    iteration_data = []
    
    for row in reader:
        # Skip the "Iteration" and "Global Best" lines
        if not row or "Iteration" in row[0] or "Global Best" in row[0]:
            if iteration_data:
                # Assign each row of positions to the corresponding UAV
                for i, pos in enumerate(iteration_data):
                    uav_positions[f'UAV{i+1}'].append(pos)
            iteration_data = []  # Clear the iteration data for the next iteration
        else:
            # Parse the position data and store it in the iteration_data list
            iteration_data.append([float(coord) for coord in row[:3]])

# Write the separated data to respective CSV files
for i in range(1, 5):
    output_file = output_filenames[i - 1]
    with open(output_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["X", "Y", "Z"])  # Column headers
        for position in uav_positions[f'UAV{i}']:
            writer.writerow(position)

print("Data has been separated and written into UAV1.csv, UAV2.csv, UAV3.csv, UAV4.csv")
