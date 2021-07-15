# Driving Behavioral Cloning

[//]: # (Image References)

[image1]: img/data_augmentation.png "data"
[image2]: img/network.png "network"
[image3]: img/training.png "train"


This repository contains the codes for behavioral cloning based on data collected from [Udacity simulator](https://github.com/udacity/self-driving-car-sim). The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior 
* Design, train and validate a model that predicts a steering angle from image data
* Use the model to drive the vehicle autonomously around the first track in the simulator. The vehicle should remain on the road for an entire loop around the track.
* Summarize the results with a written report

In what follows, I will briefly describe the code and its main components

## 1. Data creation, visualization, augmentation

The data generated from simulator includes a `driving_log.csv` file which reports the control inputs along with corresponding images in the `IMG` directory. The task of the network is to predict a similar steering angle given an input image. 

As the CSV file has a reasonable size, we read this file line by line and extract the information and image addresses for all database instances. This data is devided into two sets of training and validation sets with a 80/20 ratio.

A generator is developed to read images on the fly and reduce the memory requirement of the code. In the generator, the samples of suffles and batched into assigned batch sizes. For each instance within the batch, the corresponding images for center, left, and right cameras are loaded. Knowing the database steering angle for center image, we estimate the correct steering angle for left and right images by adding and substracting the center steering angle with the constant correction factor respectively. Another form of augmentation is applied to the database, where all images (including the center, left, and right) are flipped horizentally and a negative of the original steering angle is considered. As example of the data instance and its augmented forms are presented in the following figure.

![alt text][image1]

Through experimentation, we observed that the original data leads to insufficient steering is sharp turns, therefore, more data is collected by driving in the opposite direction of the road and sharp turns.



## 2. Model development

The model consist of a manual normalization layer (maps (0,255) range to (-0.5,0.5)), a cropping layer, 5 conv nets, follwing dense and dropout layers. Therefore, the network converts the input image to a continuous varaible, steering angle. The architecture of the network can be seen in the follwing figure.

![alt text][image2]

## 3. Training and results

The network is trained using `mse` loss function and `adam` optimization method for 15 epochs. The training results as shown in the figure below does not show overfitting as the validation loss continue to decrease.

![alt text][image3]

The result of the network is deployed in the autonomous mode of the simulator and results in completing the round on the drivable surface as shown in the `run.mp4` video.

## Usage
```
python driving_behavioral_cloning.py
python drive.py model.h5 run1
python video.py run1

```


## Future imporvements

* The oscilation of the car can be improved by adjusting hyperparameters such as correction factor

* Exploration and documentation of network architecture of the performance







