import numpy as np
# from matplotlib import pyplot as plt
from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Conv2D, MaxPooling2D, Dropout, GlobalAveragePooling2D
from keras.preprocessing.image import ImageDataGenerator
from keras import applications
import math

batch_size = 64
epochs = 100

# data loading and augmentation
train_datagen = ImageDataGenerator(
    rescale = 1./255,
    horizontal_flip = True,
    fill_mode = "nearest",
    zoom_range = 0.2,
    width_shift_range = 0.2,
    height_shift_range=0.2,
    rotation_range=20
    )

test_datagen = ImageDataGenerator(
    rescale = 1./255,
    )

generator_train = train_datagen.flow_from_directory('/home/workspace/CarND-Capstone/img/train/',
                                              target_size = (600, 800),
                                              batch_size = batch_size, 
                                              class_mode = "binary")

generator_test = train_datagen.flow_from_directory('/home/workspace/CarND-Capstone/img/test/',
                                              target_size = (600, 800),
                                              batch_size = batch_size, 
                                              class_mode = "binary")

# visualization
# for sample in generator_train:
#     samples = (8,4)
#     figs, axs = plt.subplots(samples[0], samples[1], figsize=(12, 20))
#     axs = axs.ravel()

#     for i, ind in enumerate(range(32)):
#         image = sample[0][ind].astype(int)
#         axs[i].imshow(image)
#         title = '{}'.format(sample[1][ind])
#         axs[i].set_title(title)
#         axs[i].axis('off')

#     plt.tight_layout()
#     break

# model development
model = Sequential()
model.add(applications.MobileNetV2(include_top = False, weights="imagenet", input_shape=(600, 800, 3)))
model.add(GlobalAveragePooling2D())
model.add(Dense(1, activation = 'sigmoid'))
# model.layers[0].trainable = False

model.compile(loss = "binary_crossentropy", optimizer = optimizers.Adam(), metrics=["accuracy"])

print(model.summary())

history = model.fit_generator(generator_train, steps_per_epoch=math.ceil(generator_train.n / batch_size), validation_data=generator_test, validation_steps=math.ceil(generator_test.n/batch_size), epochs=epochs, verbose=1)

# plt.figure(figsize=(10, 5))
# plt.plot(history.history['accuracy'])
# plt.plot(history.history['val_accuracy'])
# plt.title('model accuracy')
# plt.ylabel('accuracy')
# plt.xlabel('epoch')
# plt.legend(['training set', 'validation set'], loc='upper right')
# plt.savefig('results.png')
# plt.show()

model.save('model.h5')
