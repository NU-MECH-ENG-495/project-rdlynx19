# THIS FILE GOES INTO THE FOLDER: /ros2_ws/src/crazyswarm2/crazyflie_examples/crazyflie_examples

from pathlib import Path
import csv

from crazyflie_py import Crazyswarm
import numpy as np

def main():
    swarm = Crazyswarm()
    timeHelper = swarm.timeHelper
    allcfs = swarm.allcfs

    # Path to the CSV file
    csv_file = '/home/gis/Documents/AdvancedCpp/project/project-rdlynx19/GOA/results.csv'

    allcfs.takeoff(targetHeight=1.0, duration=2.0)
    timeHelper.sleep(3.0)

    # Read the CSV file
    with open(csv_file, mode='r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            # Extract x and y coordinates for each Grasshopper
            positions = []
            for i in range(1, 5):  # Assuming 4 Grasshoppers
                x = float(row[f'Grasshopper{i}_X'])
                y = float(row[f'Grasshopper{i}_Y'])
                positions.append((x, y))
                print(f'Grasshopper{i}: ({x}, {y})')

            # Set the hover points for each Crazyflie
            for idx, cf in enumerate(allcfs.crazyflies):
                if idx < len(positions):
                    x, y = positions[idx]
                    pos = np.array(cf.initialPosition) + np.array([x, y, 1.0])
                    cf.goTo(pos, 0, 2.0)

            # Sleep for 5 seconds before moving to the next set of coordinates
            timeHelper.sleep(5.0)

    # Land all Crazyflies
    allcfs.land(targetHeight=0.06, duration=2.0)
    timeHelper.sleep(3.0)

if __name__ == '__main__':
    main()