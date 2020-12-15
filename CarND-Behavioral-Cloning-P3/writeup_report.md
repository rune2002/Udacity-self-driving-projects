# **Behavioral Cloning** 

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./examples/center.png "Center Image"
[image2]: ./examples/out.png "Recovery Image"
[image3]: ./examples/recover.png "Recovery Image"
[image4]: ./examples/in.png "Recovery Image"
[image5]: ./examples/normal.png "Normal Image"
[image6]: ./examples/flip.png "Flipped Image"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
### Files Submitted & Code Quality

#### 1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* writeup_report.md or writeup_report.pdf summarizing the results
* video.mp4 recording the vehicle driving autonomously at least one lap around the track

#### 2. Submission includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

#### 3. Submission code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

### Model Architecture and Training Strategy

#### 1. An appropriate model architecture has been employed

My model consists of a convolution neural network with 3x3 and 5x5 filter sizes and depths between 24 and 64 (model.py lines 64-77) 

The model includes RELU layers to introduce nonlinearity, and the input data is normalized in the model using a Keras lambda layer (code line 65). 

#### 2. Attempts to reduce overfitting in the model

The model contains dropout layers in order to reduce overfitting (model.py lines 76). 

The model was trained and validated on different data sets to ensure that the model was not overfitting (code line 82). The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

#### 3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually (model.py line 78).

#### 4. Appropriate training data

Training data was chosen to keep the vehicle driving on the road. I used a combination of center lane driving, recovering from the left and right sides of the road, and focusing on driving smoothly around curves.

For details about how I created the training data, see the next section. 

### Model Architecture and Training Strategy

#### 1. Solution Design Approach

The overall strategy for deriving a model architecture was to make use of existing model architecture from Nvidia behavorial cloning paper. In order to gauge how well the model was working, I split my image and steering angle data into a training and validation set. I found that my first model had low mean squared error on the training set and mean squared error on the validation set, however it did not work well on the curve and bridge. This implied that I need more data, so I gathered more data rather than change the model architecture. After training on new data set, the vehicle was able to drive autonomously around the track without leaving the road.

#### 2. Final Model Architecture

My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 160x320x3 RGB image							| 
| Lambda         		| 160x320x3 normalized 3 channel image			| 
| Cropping2D         	| cropping = ((70,25),(0,0))					| 
| Convolution 5x5     	| 2x2 stride, outputs 24 channels				|
| RELU					|												|
| Convolution 5x5	    | 2x2 stride, outputs 36 channels				|
| RELU					|												|
| Convolution 5x5	    | 2x2 stride, outputs 48 channels				|
| RELU					|												|
| Convolution 5x5	    | 1x1 stride, outputs 64 channels				|
| RELU					|												|
| Convolution 5x5	    | 1x1 stride, outputs 64 channels				|
| RELU					|												|
| Flatten   		    | 			        							|
| Fully connected		| outputs 100       							|
| Fully connected		| outputs 50      							    |
| Fully connected		| outputs 10       							    |
| Fully connected		| outputs 1       							    |

#### 3. Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one using center lane driving. Here is an example image of center lane driving:

![alt text][image1]

I then recorded the vehicle recovering from the left side and right sides of the road back to center so that the vehicle would learn to recover to center when the vehicle is off-centered. Here is example images:

![alt text][image2]
![alt text][image3]
![alt text][image4]

Then I repeated this process on track two in order to get more data points.

To augment the data set, I also flipped images and angles thinking that this would compensate left-biased steering angle due to driving the track counterclockwise. For example, here is an image that has then been flipped:

![alt text][image5]
![alt text][image6]


After the collection process, I had 4488 number of data points. I then preprocessed this data by Lambda layer to normalize the data, and Cropping2D layer to remove unnecessary sky and vehicle image. I finally randomly shuffled the data set and put 20% of the data into a validation set. I used this training data for training the model. The validation set helped determine if the model was over or under fitting. The ideal number of epochs was 4 as evidenced by validation loss which increases after 4th epoch. I used an adam optimizer so that manually training the learning rate wasn't necessary.
