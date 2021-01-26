/**
 * path_planner.h
 * Author: Joonsang Park
 */

#ifndef PATH_PLANNER_H_
#define PATH_PLANNER_H_

#include <string>
#include <vector>

struct Car {
  double x;
  double y;
  double s;
  double d;
  double yaw;
  double speed;
};

struct Obstacle {
  int id;
  double x;
  double y;
  double vx;
  double vy;
  double s;
  double d;
};

enum FsmState
{
  FSM_LANE_KEEP = 0,
  FSM_LANE_CHANGE_LEFT,
  FSM_LANE_CHANGE_RIGHT,
  FSM_WAIT
};

class PathPlanner {
 public:
  PathPlanner() {
    lane_width = 4;
    ref_vel = 29.5;
    prev_size = 0;
    target_lane = 1;
    current_fsm_state = FSM_LANE_KEEP;
    end_path_s = 0;
    end_path_d = 0;
    is_speed_initialized = false;
  }
  ~PathPlanner() {}
  void initSpeed(const double car_speed);
  void initMapInfo(const std::vector<double> map_waypoints_x,
                   const std::vector<double> map_waypoints_y,
                   const std::vector<double> map_waypoints_s);
  void updatePrevInfo(const Car main_car,
                      const std::vector<double> prev_x_vals,
                      const std::vector<double> prev_y_vals,
                      const double end_path_s,
                      const double end_path_d);

  void stateTransition(std::vector<std::vector<double>> sensor_fusion);
  void generatePath();

  std::vector<double> next_x_vals;
  std::vector<double> next_y_vals;

 private:
  double computeCostFunction(const FsmState next_fsm_state,
                             const bool empty_frnt,
                             const bool empty_left,
                             const bool empty_right) const;
  double computeSpeedCost(const FsmState next_fsm_state,
                          const bool empty_frnt,
                          const bool empty_left,
                          const bool empty_right) const;
  double computeLaneChangeCost(const FsmState next_fsm_state,
                               const bool empty_left,
                               const bool empty_right) const;

  double lane_width;
  double ref_vel;
  int target_lane;
  FsmState current_fsm_state;

  std::vector<double> map_waypoints_x;
  std::vector<double> map_waypoints_y;
  std::vector<double> map_waypoints_s;

  Car main_car;
  std::vector<double> prev_x_vals;
  std::vector<double> prev_y_vals;
  int prev_size;
  double end_path_s;
  double end_path_d;
  bool is_speed_initialized;
};

#endif  // PATH_PLANNER_H_
