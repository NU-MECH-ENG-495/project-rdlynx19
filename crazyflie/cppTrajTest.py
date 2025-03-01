'''
Steps to run this file:
Place it in the crazyflie_examples package 
Add this script to the `setup.cfg` file 
Add your generated trajectories to the data folder of the package 
Each trajectory should be a csv file
Edit the path in this script to load the csv files
'''
#!/usr/bin/env python
from pathlib import Path
import csv

from crazyflie_py import Crazyswarm
from crazyflie_py.uav_trajectory import Trajectory

import numpy as np

def main():
    swarm = Crazyswarm()
    timeHelper = swarm.timeHelper
    allcfs = swarm.allcfs
    trajs = []
    n = 4

    allcfs.setParam('usd.logging', 1)
    # Path to the CSV file
    # csv_file = '/home/redhairedlynx/Documents/academics/ME495/project-rdlynx19/PSO/particle_positions.csv'

    for i in range(1,n+1):
        traj = Trajectory()
        traj.loadcsv(Path(__file__).parent / f'data/test_multi_trajectory/uav{i}.csv')
        trajs.append(traj)

    # Takeoff all Crazyflies
    allcfs.takeoff(targetHeight=1.0, duration=2.0)
    timeHelper.sleep(3.0)

    TRIALS = 1
    TIMESCALE = 1.0
    for i in range(TRIALS):
        for idx, cf in enumerate(allcfs.crazyflies):
            cf.uploadTrajectory(0, 0, trajs[idx % len(trajs)])

        allcfs.takeoff(targetHeight=1.0, duration = 2.0)
        timeHelper.sleep(3.0)
        for cf in allcfs.crazyflies:
            pos = np.array(cf.initialPosition) + np.array([0.0, 0.0, 1.0])
            cf.goTo(pos, 0, 2.0)
        timeHelper.sleep(2.5)

        allcfs.startTrajectory(0, timescale=TIMESCALE)
        timeHelper.sleep(max([t.duration for t in trajs]) * TIMESCALE + 2.0)

        allcfs.land(targetHeight=0.06, duration=2.0)
        timeHelper.sleep(3.0)

    allcfs.setParam('usd.logging', 0)

if __name__ == '__main__':
    main()
