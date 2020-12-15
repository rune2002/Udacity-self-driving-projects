
## Self-Driving Car Engineer Nanodegree
## Deep Learning
## Project: Behavorial Cloning

# 1. Import all necessary libraries
import os
import csv
import cv2
import numpy as np
import sklearn
from sklearn.model_selection import train_test_split
from sklearn.utils import shuffle

from keras.models import Sequential, Model
from keras.layers import Flatten, Dense, Lambda, Cropping2D, Dropout
from keras.layers import Convolution2D
from keras.layers.pooling import MaxPooling2D

import matplotlib.pyplot as plt
import matplotlib.image as mpimg

# 2. Read data
def generator(samples, batch_size=32):
    correction = [0.0, 0.2, -0.2]
    num_samples = len(samples)
    while 1: # Loop forever so the generator never terminates
        shuffle(samples)
        for offset in range(0, num_samples, batch_size):
            batch_samples = samples[offset:offset+batch_size]

            images = []
            measurements = []
            for batch_sample in batch_samples:
                for i in range(3):
                    name = './data/IMG/'+batch_sample[i].split('/')[-1]
#                    image = cv2.imread(name)
                    image = mpimg.imread(name)
                    measurement = float(batch_sample[3]) + correction[i]
                    images.append(image)
                    measurements.append(measurement)
                    
            augmented_images, augmented_measurements = [], []
            for image, measurement in zip(images, measurements):
                augmented_images.append(image)
                augmented_measurements.append(measurement)
                augmented_images.append(cv2.flip(image,1))
                augmented_measurements.append(measurement*(-1.0))

            X_train = np.array(augmented_images)
            y_train = np.array(augmented_measurements)
            yield sklearn.utils.shuffle(X_train, y_train)

lines = []
with open('./data/driving_log.csv') as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        lines.append(line)
        
train_samples, validation_samples = train_test_split(lines, test_size=0.2)

batch_size = 512
train_generator = generator(lines, batch_size=batch_size)

# 3. Define & train the model
model = Sequential()
model.add(Lambda(lambda x: x / 255.0 - 0.5, input_shape=(160,320,3)))
model.add(Cropping2D(cropping=((70,25),(0,0))))
model.add(Convolution2D(24,5,5,subsample=(2,2),activation="relu"))
model.add(Convolution2D(36,5,5,subsample=(2,2),activation="relu"))
model.add(Convolution2D(48,5,5,subsample=(2,2),activation="relu"))
model.add(Convolution2D(64,3,3,activation="relu"))
model.add(Convolution2D(64,3,3,activation="relu"))
model.add(Flatten())
model.add(Dense(100))
model.add(Dense(50))
model.add(Dense(10))
model.add(Dropout(0.2))
model.add(Dense(1))
model.compile(loss='mse', optimizer='adam')

for i in range(int(len(lines) / batch_size)):
    X_train, y_train = next(train_generator)
    model.fit(X_train, y_train, validation_split=0.2, shuffle=True, nb_epoch=4)

model.save('model.h5')
