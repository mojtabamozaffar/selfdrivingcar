import csv
import numpy as np
import matplotlib.image as mpimg
from matplotlib import pyplot as plt
from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Conv2D, MaxPooling2D, Cropping2D, Dropout
import sklearn
from sklearn.model_selection import train_test_split
from sklearn.utils import shuffle
import math

def generator(samples, batch_size=32):
    num_samples = len(samples)
    correction = 0.3
    while 1:
        shuffle(samples)
        for offset in range(0, num_samples, batch_size):
            batch_samples = samples[offset:offset+batch_size]

            images = []
            angles = []
            for batch_sample in batch_samples:
                name = 'data/IMG/'+batch_sample[0].split('/')[-1]
                center_image = mpimg.imread(name)
                center = mpimg.imread('data/IMG/'+batch_sample[0].split('/')[-1])
                left = mpimg.imread('data/IMG/'+batch_sample[1].split('/')[-1])
                right = mpimg.imread('data/IMG/'+batch_sample[2].split('/')[-1])
                steering = float(batch_sample[3])
                
                # center data
                images.append(center)
                angles.append(steering)
                images.append(np.fliplr(center))
                angles.append(-1 * steering)
                
                # left data
                images.append(left)
                angles.append(steering + correction)
                images.append(np.fliplr(left))
                angles.append(-1 * (steering + correction))
                
                # right data
                images.append(right)
                angles.append(steering - correction)
                images.append(np.fliplr(right))
                angles.append(-1 * (steering - correction))

            # trim image to only see section with road
            X_train = np.array(images)
            y_train = np.array(angles)
            yield sklearn.utils.shuffle(X_train, y_train)


if __name__ == '__main__':
    
    samples = []
    with open('data/driving_log.csv') as csvfile:
        reader = csv.reader(csvfile)
        next(reader, None)
        for line in reader:
            samples.append(line)

    train_samples, validation_samples = train_test_split(samples, test_size=0.2)

    batch_size=512

    train_generator = generator(train_samples, batch_size=batch_size)
    validation_generator = generator(validation_samples, batch_size=batch_size)



    model = Sequential()
    model.add(Lambda(lambda x: (x / 255.0) - 0.5, input_shape=(160, 320, 3)))
    model.add(Cropping2D(cropping=((60, 25), (0, 0))))
    model.add(Conv2D(24, (5, 5), strides=(2, 2), activation='relu'))
    model.add(Conv2D(36, (5, 5), strides=(2, 2), activation='relu'))
    model.add(Conv2D(48, (5, 5), strides=(2, 2), activation='relu'))
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(Flatten())
    model.add(Dense(256, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(64, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(8))
    model.add(Dense(1))

    model.compile(optimizer='adam', loss='mse')

    print(model.summary())

#     history = model.fit(x=X_data, y=y_data, batch_size=64, shuffle=True, validation_split=0.2, epochs=10)
    history = model.fit_generator(train_generator, steps_per_epoch=math.ceil(len(train_samples)/batch_size), validation_data=validation_generator, validation_steps=math.ceil(len(validation_samples)/batch_size), epochs=15, verbose=1)

    plt.figure(figsize=(10, 10))
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('model mean squared error loss')
    plt.ylabel('mean squared error loss')
    plt.xlabel('epoch')
    plt.legend(['training set', 'validation set'], loc='upper right')
    plt.savefig('training.png')
    plt.show()

    model.save('model.h5')


