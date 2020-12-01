# **Advanced Lane Finding**
---

**Advanced Lane Finding Project**
The goals / steps of this project are the following:
* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image1]: ./output_images/1_calibration/calibration3.jpg "Undistorted"
[image2]: ./output_images/2_undistorted/test1.jpg "Road Transformed"
[image3]: ./output_images/3_thresholded_binary/test1.jpg "Binary Example"
[image4]: ./output_images/4_birds_eye_view/test1.jpg "Warp Example"
[image5]: ./output_images/5_lane_detected/test1.jpg "Fit Visual"
[image6]: ./output_images/8_result/test1.jpg "Output"
[video1]: ./output_videos/project_video.mp4 "Video"

## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points

### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---

### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

Writeup(./writeup_template.md) and jupyter notebook(./P2.ipynb) are submitted. 

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the second code cell of the IPython notebook.  

I start by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image. Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image. `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to the test image using the `cv2.undistort()` function and obtained this result: 

![alt text][image1]

### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.

To demonstrate this step, I will describe how I apply the distortion correction to one of the test images like this one:
![alt text][image2]
As I applied the distortion correction to chessboard images above, I also used `cv2.undistort()` function.

#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.

I used a combination of color and gradient thresholds to generate a binary image. The code for this step is contained in the 4th code cell of the IPython notebook. Here's an example of my output for this step. 

![alt text][image3]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.

The code for my perspective transform includes a function called `birds_eye_view()`, which appears in the 5th code cell of the IPython notebook. The `birds_eye_view()` function takes as inputs an image (`img`), and return an output image (`warped`). I chose the hardcode the source and destination points in the following manner:

```python
    src = np.float32([[250,imshape[0]],[610, 460], [740, 460], [1130,imshape[0]]])
    dst = np.float32([[200, 700],[200, 0],[900, 0], [900, 700]])
```

This resulted in the following source and destination points:

| Source        | Destination   | 
|:-------------:|:-------------:| 
| 250, 720      | 200, 700        | 
| 610, 460      | 200, 0      |
| 740, 460     | 900, 0      |
| 1130, 720      | 900, 700        |

Here's an example of my output for this step.
![alt text][image4]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

Then I found left and right lane points through windows in the following manner:
```python
good_left_inds = ((nonzeroy >= win_y_low) & (nonzeroy < win_y_high) & 
(nonzerox >= win_xleft_low) &  (nonzerox < win_xleft_high)).nonzero()[0]
good_right_inds = ((nonzeroy >= win_y_low) & (nonzeroy < win_y_high) & 
(nonzerox >= win_xright_low) &  (nonzerox < win_xright_high)).nonzero()[0]
```
From the lane points, I fit my lane lines with a 2nd order polynomial using `np.polyfit()`. The entire code is contained in the 6th code cell of the IPython notebook. Here's an example of my output for this step.
![alt text][image5]

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

I calculated the radius of curvature of the lane as following equations:
![equation](https://latex.codecogs.com/gif.latex?f%28y%29%3DAy%5E%7B2%7D&plus;By&plus;C)
![equation](https://latex.codecogs.com/gif.latex?f%28y%29%3DAy%5E%7B2%7D&plus;By&plus;C%24%24%20%24%24R_%7Bcurve%7D%3D%7B%281&plus;%282Ay&plus;B%29%5E%7B2%7D%29%5E%7B3%5Cover2%7D%5Cover%7C2A%7C%7D) 

I averaged the value at the bottom of the image(`y=image.shape[0]`) of left and right lane. Then I got the position of the vehicle with respect to center as the difference between the center of ROI and the center of lane. The entire code is contained in the 7th code cell of the IPython notebook.

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

I implemented this step in the 9th code cell of the IPython notebook. Here is an example of my result on a test image:
![alt text][image6]

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).
![alt text][video1]
Here's a [link to my video result](./output_videos/project_video.mp4)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?
As the optional challenge problems show, there are several shortcomings. First, as I set the windows from maximum of histogram of thresholded binary image, my pipeline would fail when there are any non-lane line on the road. To solve the problem, it is necesasry to make use of other information such as color, and minimum lane width. Second, in order to make my pipeline more robust, I can use prior information from previous frame.
