from pathlib import Path
import csv

from crazyflie_py import Crazyswarm
import numpy as np

def main():
    swarm = Crazyswarm()
    timeHelper = swarm.timeHelper
    allcfs = swarm.allcfs

    # Path to the CSV file
    csv_file = '/home/redhairedlynx/Documents/academics/ME495/project-rdlynx19/PSO/particle_positions.csv'

    # Takeoff all Crazyflies
    allcfs.takeoff(targetHeight=1.0, duration=2.0)
    timeHelper.sleep(3.0)

    # Read the CSV file
    with open(csv_file, mode='r') as file:
        reader = csv.reader(file)
        positions = []  # List to store positions for each iteration
        for row in reader:
            # Skip lines that contain "Iteration" or "Global Best"
            if not row or "Iteration" in row[0] or "Global Best" in row[0]:
                continue

            # Extract x, y, and z coordinates from the row
            x, y, z = map(float, row)
            positions.append((x, y, z))

            # If we have collected positions for all Crazyflies, send them as waypoints
            if len(positions) == len(allcfs.crazyflies):
                for idx, cf in enumerate(allcfs.crazyflies):
                    x, y, z = positions[idx]
                    # Add the initial position offset to the waypoint
                    pos = np.array(cf.initialPosition) + np.array([x, y, z])
                    cf.goTo(pos, 0, 2.0)  # Send the waypoint command

                # Sleep for 5 seconds before moving to the next set of coordinates
                timeHelper.sleep(5.0)

                # Clear the positions list for the next iteration
                positions.clear()

    # Land all Crazyflies
    allcfs.land(targetHeight=0.06, duration=2.0)
    timeHelper.sleep(3.0)

if __name__ == '__main__':
    main()
