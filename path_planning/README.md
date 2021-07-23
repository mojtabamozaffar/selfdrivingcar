# Path Planning for Self Driving Cars


## Goals
In this project your goal is to safely navigate around a virtual highway with other traffic that is driving +-10 MPH of the 50 MPH speed limit. You will be provided the car's localization and sensor fusion data, there is also a sparse map list of waypoints around the highway. The car should try to go as close as possible to the 50 MPH speed limit, which means passing slower traffic when possible, note that other cars will try to change lanes too. The car should avoid hitting other cars at all cost as well as driving inside of the marked road lanes at all times, unless going from one lane to another. The car should be able to make one complete loop around the 6946m highway. Since the car is trying to go 50 MPH, it should take a little over 5 minutes to complete 1 loop. Also the car should not experience total acceleration over 10 m/s^2 and jerk that is greater than 10 m/s^3.

Four steps are used to develop this project:
* Processing sensor fusion data to detect the state of the environment
* Selecting an action using a state machine
* Generating anchor points and fit a spline to it
* Producing target points using previous path and the new spline

In what follows, I will briefly describe the code and its main components.

## 1. Process sensor fusion data

We obtain the lane, velocity and s position in frenet coordinate system for each car detected by the sensor fusion module. Using the velocity we calculate the s position of the detected cars in the next time step and use these information to update the status of the environment including if we have a car close to ego vehicle in the same lane, if a car is too close (for emergency stop), and if there is a car on the left/right side of the ego vehicle.


## 2. State machine

A simple state machine is adopted for this project, which includes five state in the order of priority:
1. Decrease speed rapidly (emergency stop) if case of a very close vehicle. This state has the highest priority.
2. If there is a car in front of ego vehicle and left lane is open, change lane to left.
3. If there is a car in front of ego vehicle and right lane is open, change lane to right.
4. If there is a car in front of ego vehicle and both sides are occupied, decrease the speed comfortably.
5. If no car in front, incrase/match the speed close to the speed limit.


## 3. Generate anchor points

Following the instructions in the Q&A video, 5 anchor points are selected. Two points from previous path (if not available approximated using current position of the car), and three evenly spaced out 30, 60, and 90 meters away from the car in s dimension of frenet coodinate with appropriate lane from the state machine decision. The anchor points are converted into the local coordinate system of the car. An spline is fitted into this five points using the helper library `spline.h`.

## 4. Produce target points

The remaining of target points from previous path is added to the current path to smoothen the behavior. New target points are generated from the fitted spline, converted back into the global coordinte system and added to the final list of points.


## Future imporvements

* Defining a cost function for different options, including how to merge to different lanes.
* Better prediction of other cars' behavior.






