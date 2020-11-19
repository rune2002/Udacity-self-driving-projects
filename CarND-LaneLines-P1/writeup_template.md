# **Finding Lane Lines on the Road** 
---

**Finding Lane Lines on the Road**
The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report
---

### Reflection

### 1. Describe your pipeline. As part of the description, explain how you modified the draw_lines() function.

My pipeline consisted of 5 steps. 
* Convert the images to grayscale
* Smooth the image using Gaussian filter with kernel size 5
* Do Canny edge detection
* Extract ROI by applying mask
* Find lines using Hough transform

In order to draw a single line on the left and right lanes, I modified the draw_lines() function by
extrapolating each line to the top and bottom of the ROI, and averaging start and end point. Also, I divided lines into left and right lanes based on the position of points, and excluded outliers of which slope is too large or small. 

### 2. Identify potential shortcomings with your current pipeline
As the optional challenge problem shows, there are several shortcomings. One potential shortcoming would be what would happen when the vehicle drive on uphill or downhill road because ROI of my pipeline is fixed. Another shortcoming could be on curved road. This is because, when extrapolating each line, my pipeline assumes the slope of lines is constant. 


### 3. Suggest possible improvements to your pipeline
A possible improvement would be to adapt ROI based on slope of the road, which possibly comes from IMU sensor, map, or the image itself. Another potential improvement could be to divide ROI and apply my pipelines to each of them, which can decrease the effect of changing slope of lines. 
