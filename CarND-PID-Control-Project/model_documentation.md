# **PID Control** 
[//]: # (Image References)

[image1]: ./data/result.png "Result Image"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/1972/view) individually and describe how I addressed each point in my implementation.  

---
### Compilation

#### 1. Your code should compile.

The Code compiles without errors with cmake and make.

### Implementation

#### 1. The PID procedure follows what was taught in the lessons.

I implemented PID controller in the following manner:
First, update p_error, i_error, and d_error, and calculate total error. (line 17-28, PID.cpp)
```
void PID::UpdateError(double cte) {
  static double prev_cte = 0.0;
  p_error = cte;
  i_error += cte;
  d_error = cte - prev_cte;
  prev_cte = cte;
}

double PID::TotalError() {
  double total_error = Kp * p_error + Ki * i_error + Kd * d_error;
  return total_error;
}
```
Second, compute steer_value from total error by multiplying -1 and clipping so that absolute value of the steer_value cannot exceed 1.0.
```
pid.UpdateError(cte);
double steer_value = -pid.TotalError();
double abs_clip_steer_value = abs(steer_value) > 1.0 ? 1.0 : abs(steer_value);
steer_value = steer_value > 0 ? abs_clip_steer_value : - abs_clip_steer_value;
```

### Reflection

#### 1. Describe the effect each of the P, I, D components had in your implementation.
* P component is essential to PID controller. When Kp is too small, the car converges to its reference path slowly (Kp: 0.01, Ki: 0.01, and Kd: 3.0), however, when Kp is too large, it results in overshoot, and even the car diverges from the path. (Kp:1.0, Ki: 0.01, and Kd: 3.0)
* I could not check the effect of I component, since bias of cte value, I think, is small. There is no clear difference even when I set Ki to 0. (Kp: 0.2, Ki: 0, and Kd: 3.0).   
* D component improves transient response; it reduces overshoot. The car diverges when I set Kd to 0. (Kp: 0.2, Ki: 0.01, and Kd: 0.0) 

#### 2. Describe how the final hyperparameters were chosen.

I started with the value the lecture provided (Kp: 0.2, Ki: 0.01, and Kd: 3.0), and I chose final hyperparameters through manual tuning. (Kp: 0.1, Ki: 0.01, and Kd: 4.0)

### Simulation

#### 1. The vehicle must successfully drive a lap around the track.

The vehicle can drive a lap around the track successfully.
