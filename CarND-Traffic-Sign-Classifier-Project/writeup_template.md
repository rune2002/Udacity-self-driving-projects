# **Traffic Sign Recognition** 
---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./data/train_distribution.png "Train"
[image2]: ./data/valid_distribution.jpg "Valid"
[image3]: ./data/test_distribution.png "Test"
[image4]: ./data/original.png "Original"
[image5]: ./data/gray.png "Gray"
[image6]: ./data/new_images/100_1607_small.jpg "Traffic Sign 1"
[image7]: ./data/new_images/Arterial_small1.jpg "Traffic Sign 2"
[image8]: ./data/new_images/Do-Not-Enter_small.jpg "Traffic Sign 3"
[image9]: ./data/new_images/Stop_sign_small.jpg "Traffic Sign 4"
[image10]: ./data/new_images/bwylezich160200124.jpg "Traffic Sign 5"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

Writeup(./writeup_template.md) and jupyter notebook(./Traffic_Sign_Classifier.ipynb) are submitted.

### Data Set Summary & Exploration

#### 1. Provide a basic summary of the data set. In the code, the analysis should be done using python, numpy and/or pandas methods rather than hardcoding results manually.

I used the numpy library to calculate summary statistics of the traffic
signs data set:

* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
* The shape of a traffic sign image is [32, 32]
* The number of unique classes/labels in the data set is 43

#### 2. Include an exploratory visualization of the dataset.

Here is an exploratory visualization of the data set. It is a bar chart showing how the traing/validation/test data are distributed respectively.
![alt text][image1]
![alt text][image2]
![alt text][image3]
Here is an example how the images look like:
![alt text][image4]

### Design and Test a Model Architecture

#### 1. Describe how you preprocessed the image data. What techniques were chosen and why did you choose these techniques? Consider including images showing the output of each preprocessing technique. Pre-processing refers to techniques such as converting to grayscale, normalization, etc. (OPTIONAL: As described in the "Stand Out Suggestions" part of the rubric, if you generated additional data for training, describe why you decided to generate additional data, how you generated the data, and provide example images of the additional data. Then describe the characteristics of the augmented training set like number of images in the set, number of images for each class, etc.)

As a first step, I decided to convert the images to grayscale because I can decrease the size of the model by using only one channel image as input as well as I worried that color information misguides learning, which means shape information matters.

Here is the image above after grayscaling:
![alt text][image5]

As a last step, I normalized the image data for the model to learn more efficiently.



#### 2. Describe what your final model architecture looks like including model type, layers, layer sizes, connectivity, etc.) Consider including a diagram and/or table describing the final model.

My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x1 normalized gray image					| 
| Convolution 5x5     	| 1x1 stride, valid padding, outputs 28x28x6 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 14x14x6  				|
| Convolution 5x5	    | 1x1 stride, valid padding, outputs 10x10x16	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 5x5x16  			    	|
| Flatten   		    | outputs 400        							|
| Fully connected		| outputs 120       							|
| RELU					|												|
| Fully connected		| outputs 84       							    |
| RELU					|												|
| Fully connected		| outputs 43       							    |
| Softmax				|								                |

#### 3. Describe how you trained your model. The discussion can include the type of optimizer, the batch size, number of epochs and any hyperparameters such as learning rate.
I used Adam Optimizer with learning rate 0.001, batch size 128, and epochs 20. In order to avoid over-fitting, I shuffled training data for each batch, and I make use of drop-out with keep probability 0.5.

#### 4. Describe the approach taken for finding a solution and getting the validation set accuracy to be at least 0.93. Include in the discussion the results on the training, validation and test sets and where in the code these were calculated. Your approach may have been an iterative process, in which case, outline the steps you took to get to the final solution and why you chose those steps. Perhaps your solution involved an already well known implementation or architecture. In this case, discuss why you think the architecture is suitable for the current problem.

My final model results were:
* training set accuracy of 0.996
* validation set accuracy of 0.936
* test set accuracy of 0.922

If a well known architecture was chosen:
* What architecture was chosen? LeNet-5
* Why did you believe it would be relevant to the traffic sign application?
I saw LeNet-5 achieve high performance for digit recognition, so I believe that the model can learn to classify the traffic sign with appropriate data set.
* How does the final model's accuracy on the training, validation and test set provide evidence that the model is working well?
The high accuracy for training and validation set does not guarantee that the model would work well mainly due to over-fitting; validation set can cause over-fitting indirectly. When all the data set are used prpoerly, the high performance for test set means the model is working well for other data set too.
 

### Test a Model on New Images

#### 1. Choose five German traffic signs found on the web and provide them in the report. For each image, discuss what quality or qualities might be difficult to classify.

Here are five German traffic signs that I found on the web:

![alt text][image6] ![alt text][image7] ![alt text][image8] 
![alt text][image9] ![alt text][image10]

The 5th image might be difficult to classify because the image is not square, and the traffic sign is tilted and left-biased.

#### 2. Discuss the model's predictions on these new traffic signs and compare the results to predicting on the test set. At a minimum, discuss what the predictions were, the accuracy on these new predictions, and compare the accuracy to the accuracy on the test set (OPTIONAL: Discuss the results in more detail as described in the "Stand Out Suggestions" part of the rubric).

Here are the results of the prediction:

| Image			                            |     Prediction	        					| 
|:-----------------------------------------:|:---------------------------------------------:| 
| Right-of-way at the next intersection		| Right-of-way at the next intersection        | 
| Priority road     			            | Priority road 								|
| No entry				                    | No entry										|
| Stop	      		                        | Stop				 				            |
| Bicycles crossing			                | Speed limit (30km/h)    						|


The model was able to correctly guess 4 of the 5 traffic signs, which gives an accuracy of 80%. The accuracy is lower than that for test set above, however, I guess if I gather more images, the accuracy would be close to 0.922.

#### 3. Describe how certain the model is when predicting on each of the five new images by looking at the softmax probabilities for each prediction. Provide the top 5 softmax probabilities for each image along with the sign type of each probability. (OPTIONAL: as described in the "Stand Out Suggestions" part of the rubric, visualizations can also be provided such as bar charts)

The code for making prediction for top 5 softmax probabilities on my final model is located in the 18th cell of the Ipython notebook. For the 1~4 images the model is sure for the prediction (probability close to 1.0)
The top five soft max probabilities for the 1st image were:
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .99         			| Right-of-way at the next intersection     	| 
| .0     				| Beware of ice/snow					    	|
| .0					| Pedestrians					        		|
| .0	      			| Roundabout mandatory			        		|
| .0				    | Double curve  					    		|

The top five soft max probabilities for the 2nd image were:
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .99         			| Priority road 								| 
| .0     				| Yield					    					|
| .0					| No passing									|
| .0	      			| No vehicles				     				|
| .0				    | No entry   					        		|

The top five soft max probabilities for the 3rd image were:
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .99         			| No entry  									| 
| .0     				| No passing									|
| .0					| Priority road									|
| .0	      			| Roundabout mandatory			 				|
| .0				    | Turn left ahead   							|

The top five soft max probabilities for the 4th image were:
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
|.99         			| Stop          								| 
|.0     				| Wild animals crossing							|
|.0					    | Speed limit (80km/h)							|
|.0	      			    | No entry				 	        			|
|.0				        | Speed limit (60km/h)   	    				|

For the 5th image, which the model misclassified, the highest probability (0.89) is relatively smaller.
The top five soft max probabilities for the 5th image were:
| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
|.89         			| Speed limit (30km/h)							| 
|.08     				| Speed limit (50km/h)							|
|.02				    | Priority road									|
|.0	      			    | Speed limit (80km/h)				 			|
|.0				        | Speed limit (100km/h)   						|

### (Optional) Visualizing the Neural Network (See Step 4 of the Ipython notebook for more details)
#### 1. Discuss the visual output of your trained network's feature maps. What characteristics did the neural network use to make classifications?