# **Path Planning** 
[//]: # (Image References)

[image1]: ./data/result.png "Result Image"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/1971/view) individually and describe how I addressed each point in my implementation.  

---
### Compilation

#### 1. The code compiles correctly.

I modified main.cpp and helpers.h files and added path_planner.h, path_planner.cpp, and spline.h files.
All functions I implemented is in PathPlanner class. The Code compiles without errors with cmake and make.

### Valid Trajectories

#### 1. The car is able to drive at least 4.32 miles without incident

The car can drive at least 4.32 miles without incident. Please refer to data/result.png.

![alt text][image1]

#### 2. The car drives according to the speed limit

The speed can't exceed the speed limit (50 MPH), since the car accelerates when its speed is smaller than 49 MPH and decelerates when there are obstacles ahead. The detail code is as follows: 
```
if (!empty_frnt)
    ref_vel -= 0.448;
else if (ref_vel < 49.0)
    ref_vel += 0.224;
```
#### 3. Max Acceleration and Jerk are not Exceeded

To reduce acceleration and jerk, I generated smooth trajectories using [spline library](http://kluge.in-chemnitz.de/opensource/spline/). The detail code is in generatePath() function of PathPlanner class (line 170-265, path_planner.cpp).

#### 4. Car does not have collisions

There are two points which make the car avoid collisions. First, as I mentioned above, When the car keeps its lane, the car slows down if there is obstacle ahead. Second, the car change its lane only when the target lane is empty. If target lane is not empty, cost function becomes max value (2.0). The detail code for determining whether the target lane is empty is as follows:
```
if ((obs.s + 5) > car_s && (obs.s - car_s) < 30)
{
  if (obs.d < (lane_width*(target_lane+1)) && obs.d > (lane_width*target_lane))
    empty_frnt = false;
  else if (obs.d < (lane_width*target_lane) && obs.d > (lane_width*(target_lane-1)))
    empty_left = false;
  else if (obs.d < (lane_width*(target_lane+2)) && obs.d > (lane_width*(target_lane+1)))
    empty_right = false;
}
```
The detail code for handling lane change cost when target lane isn't empty is as follows:
```
else if ((next_fsm_state == FSM_LANE_CHANGE_LEFT && (!empty_left))
    || (next_fsm_state == FSM_LANE_CHANGE_RIGHT && (!empty_right))
    || (next_fsm_state == FSM_LANE_CHANGE_LEFT && (target_lane == 0))
    || (next_fsm_state == FSM_LANE_CHANGE_RIGHT && (target_lane == 2)))
{
  cost = 2.0;
}
```
The detail code is in stateTransition function and computeCostFunction function of PathPlanner class (line 52-105, and 107-115 respectively, path_planner.cpp).

#### 5. The car stays in its lane, except for the time between changing lanes
When generating path, path planner uses constant d values corresponding to target lane and transform to XY coordinate using getXY function in helpers.h. Therefore, the car can stay in its lane. The detail code is as follows:
```
vector<double> next_wp0 = getXY(main_car.s+30,(2+lane_width*target_lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
vector<double> next_wp1 = getXY(main_car.s+60,(2+lane_width*target_lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
vector<double> next_wp2 = getXY(main_car.s+90,(2+lane_width*target_lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
```

#### 6. The car is able to change lanes
The finite state machine I implemented consists of 4 states: FSM_LANE_KEEP, FSM_LANE_CHANGE_LEFT, FSM_LANE_CHANGE_RIGHT, and FSM_WAIT. The path planner decides next state by calculating cost function, the summation of speed cost function and lane change penalty, for possible next state. The detail code is as follows:
```
vector<FsmState> possible_next_fsm_states = {FSM_LANE_KEEP, FSM_LANE_CHANGE_LEFT, FSM_LANE_CHANGE_RIGHT};
for (auto& next_fsm_state : possible_next_fsm_states)
{
  double cost = computeCostFunction(next_fsm_state, empty_frnt, empty_left, empty_right);
  if (cost < min_cost)
  {
    min_cost = cost;
    best_fsm_state = next_fsm_state;
  }
}
```

### Reflection

#### 1. There is a reflection on how to generate paths.

The code model is described in detail above.