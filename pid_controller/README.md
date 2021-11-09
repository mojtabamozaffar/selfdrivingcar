# PID Controller for Self Driving Cars


## Goals
In this project we design a PID controller to accurately and smoothly pass through desired driving path.

Two steps are used to develop this project:
* Develop a PID class
* Adjust the parameters of the PID controller


## 1. PID class

A simple PID controller is developed in `PID.cpp` where proportional error (`p_error`) is proportional to the crosstrack error (`cte`), the derivitie error (`e_error`) depends on the change of the `cte` error since last iteration, and the integral error (`i_error`) accumulates the `cte` error over the execution period. The final error is the addition of these three components. To correct the direction we apply the streeing value in the opposite direction of the calculated summed up error.


## 2. Parameter optimization

The parametes are initially set to the ones found in the course materials, i.e. Kp = 3, Ke = 10, Ki = 0.5. However, this resulted in an undesirable behavior. Through manual adjustment following parameters are obtained which shows decent driving behavior: Kp = 0.1, Ke = 5, Ki = 0.001

A few rules of thumb are deployed to adjust the PID parameters including:
* Increase Kp to make the response faster and reduce rise time.
* Increase Kd to decrease the oscillations.
* Add small values of Ki to compensate for the steady state error.
* Decrease Kp to avoid excessive overshooting.


## Future imporvements

* Automated optimization of PID parameters using Twiddle
* Using advanced controllers such as model predictive control





