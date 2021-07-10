# Traffic Sign Recognition Classifier


This repository contains the codes for traffic sign classifier method using convolutional neural networks. The goals / steps of this project includes:

* Load, explore and visualization of the data set
* Preprocessing data
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images

In what follows, I will briefly describe the code and its main components

## 1. Data exploration

The dataset contains over 50,000 german traffic signs devided into three parts of training, validation and test sets. The database includes 43 different classes of signs including speed limits, yeild, stop, caution, etc. Samples of data and the distribution of data over different signs are plotted in the jupyter notebook.


## 2. Preprocessing

Three preprocessing steps are considered here: 

1. The RGB image is converted into a gray scale image. Although color does play a role in the meaning of signs, our results show that even in a gray scale image the classifier is capable of achieving high accuracy.

2. To combat the wide range of contrast and brightness in the database, we utilized `cv2.equalizeHist` to normalize the images.

3. We used `ImageDataGenerator` class from `keras.preprocessing.image` to augment images with rotational, zoom, and shift variations. In our experiment, this allows to train a more robust classifier and increase the accuracy score.


## 3. Model development and training

The model comprises of five layers including two CNNs and three FCNNs. A summary of the network is presented hereunder

| Layer name    | Layer characteristics        | 
|:-------------:|:----------------------------:| 
| CNN_1         | kernel: 5x5, channels: 16    | 
| CNN_2         | kernel: 5x5, channels: 32    |
| Flatten       |                              |
| Dropout       |                              |
| FCNN_1        | output_size = 256            |
| Dropout       |                              |
| FCNN_2        | output_size = 128            |
| FCNN_3        | output_size = 43             |

Adam optimizer is used to train a cross entropy loss function between model predictions (logits) and one-hot encoded labels in the database. The dropout rate is set as 0.7 for training while it is kept at 1.0 for evaluations. Other hyperparameters can be found in the provided jupyter notebook.

## 4. Results and discussions

After training for 20 epochs, we achieve an accuracy of 99.3% of training data, 97.9% of validation data, and 95.8% of test data.

Five new images are downloaded online, the trained model is capable of predicting the output of these five images with 100% accuracy (admittedly, the new five images are not challenging cases). The exploration of top 5 probabilities predicted by the model, shows high confidence in the model prediction, where in all 5 cases the model produces a probability of close to 1 for the correct label.


## Usage
Run the cells in the jupyter notebook, Traffic_sign_Classifier.ipynb, in the provided order.



