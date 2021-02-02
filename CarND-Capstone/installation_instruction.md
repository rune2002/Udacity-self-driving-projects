### How to install

There is no additional libraries to run my code. 
1. Make and run styx
```bash
cd ros
catkin_make
source devel/setup.sh
roslaunch launch/styx.launch
```
2. Run the simulator and toggle off Manual switch

I didn't implement traffic light classifier, however
I implemented stop at traffic lights using /vehicle/traffic_lights.  
So, you don't have to toggle on Camera switch.
