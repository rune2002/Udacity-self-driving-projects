/**
 * particle_filter.cpp
 * Author: Joonsang Park
 */

#include "path_planner.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "spline.h"

#include "helpers.h"

using std::string;
using std::vector;

void PathPlanner::initSpeed(const double car_speed) {
  if (!is_speed_initialized)
  {
    ref_vel = car_speed;
    is_speed_initialized = true;
  }
}

void PathPlanner::initMapInfo(const std::vector<double> map_waypoints_x,
                              const std::vector<double> map_waypoints_y,
                              const std::vector<double> map_waypoints_s) {
  this->map_waypoints_x = map_waypoints_x;
  this->map_waypoints_y = map_waypoints_y;
  this->map_waypoints_s = map_waypoints_s;
}

void PathPlanner::updatePrevInfo(const Car main_car,
                                 const std::vector<double> prev_x_vals,
                                 const std::vector<double> prev_y_vals,
                                 const double end_path_s,
                                 const double end_path_d) {
  this->main_car = main_car;
  this->prev_x_vals = prev_x_vals;
  this->prev_y_vals = prev_y_vals;
  this->prev_size = prev_x_vals.size();
  this->end_path_s = end_path_s;
  this->end_path_d = end_path_d;
}

void PathPlanner::stateTransition(std::vector<std::vector<double>> sensor_fusion) {
  current_fsm_state = prev_size > 20 ? FSM_WAIT : FSM_LANE_KEEP;
  if (current_fsm_state == FSM_WAIT)
    return;

  double car_s = prev_size > 0 ? end_path_s : main_car.s;

  bool empty_frnt = true;
  bool empty_left = true;
  bool empty_right = true;
  for (int i = 0; i < sensor_fusion.size(); i++)
  {
    Obstacle obs;
    obs.id = sensor_fusion[i][0];
    obs.x = sensor_fusion[i][1];
    obs.y = sensor_fusion[i][2];
    obs.vx = sensor_fusion[i][3];
    obs.vy = sensor_fusion[i][4];
    obs.s = sensor_fusion[i][5];
    obs.d = sensor_fusion[i][6];

    double obs_speed = sqrt(obs.vx * obs.vx + obs.vy * obs.vy);
    obs.s += ((double)prev_size * 0.02 * obs_speed);
    if ((obs.s + 5) > car_s && (obs.s - car_s) < 30)
    {
      if (obs.d < (lane_width*(target_lane+1)) && obs.d > (lane_width*target_lane))
        empty_frnt = false;
      else if (obs.d < (lane_width*target_lane) && obs.d > (lane_width*(target_lane-1)))
        empty_left = false;
      else if (obs.d < (lane_width*(target_lane+2)) && obs.d > (lane_width*(target_lane+1)))
        empty_right = false;
    }
  }

  double min_cost = 3.0;
  FsmState best_fsm_state;
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

  current_fsm_state = best_fsm_state;

  if (!empty_frnt)
    ref_vel -= 0.448;
  else if (ref_vel < 49.0)
    ref_vel += 0.224;
}

double PathPlanner::computeCostFunction(const FsmState next_fsm_state,
                                        const bool empty_frnt,
                                        const bool empty_left,
                                        const bool empty_right) const {
  double speedCost = computeSpeedCost(next_fsm_state, empty_frnt, empty_left, empty_right);
  double laneChangeCost = computeLaneChangeCost(next_fsm_state, empty_left, empty_right);
  double cost = speedCost + laneChangeCost;
  return cost;
}

double PathPlanner::computeSpeedCost(const FsmState next_fsm_state,
                                     const bool empty_frnt,
                                     const bool empty_left,
                                     const bool empty_right) const {
  double cost;
  if ((next_fsm_state == FSM_LANE_KEEP && empty_frnt)
      || (next_fsm_state == FSM_LANE_CHANGE_LEFT && empty_left)
      || (next_fsm_state == FSM_LANE_CHANGE_RIGHT && empty_right))
  {
    cost = 0.0;
  }
  else
  {
    double target_vel = ref_vel;
    double slope1 = (-0.6 / 49.5);
    double slope2 = (1.0 / 0.5);
    if (target_vel > 0.0 && target_vel < 49.5)
      cost = 0.6 + slope1 * target_vel;
    else if (target_vel > 49.5 && target_vel < 50.0)
      cost = slope2 * (target_vel - 49.5);
    else if (target_vel > 50.0)
      cost = 1.0;
    else
      cost = 1.0;
  }
  return cost;
}

double PathPlanner::computeLaneChangeCost(const FsmState next_fsm_state,
                                          const bool empty_left,
                                          const bool empty_right) const {
  double cost;
  if (next_fsm_state == FSM_LANE_KEEP)
  {
    cost = 0.0;
  }
  else if ((next_fsm_state == FSM_LANE_CHANGE_LEFT && (!empty_left))
      || (next_fsm_state == FSM_LANE_CHANGE_RIGHT && (!empty_right))
      || (next_fsm_state == FSM_LANE_CHANGE_LEFT && (target_lane == 0))
      || (next_fsm_state == FSM_LANE_CHANGE_RIGHT && (target_lane == 2)))
  {
    cost = 2.0;
  }
  else
  {
    double delta_d = 4.0;
    double delta_s = 30.0;
    cost = 1 - exp(-delta_d/delta_s);
  }

  return cost;
}

void PathPlanner::generatePath() {
  next_x_vals = prev_x_vals;
  next_y_vals = prev_y_vals;

  if (current_fsm_state == FSM_WAIT)
    return;
  else if (current_fsm_state == FSM_LANE_CHANGE_LEFT)
    target_lane--;
  else if (current_fsm_state == FSM_LANE_CHANGE_RIGHT)
    target_lane++;

  vector<double> ptsx;
  vector<double> ptsy;
  double ref_x = main_car.x;
  double ref_y = main_car.y;
  double ref_yaw = deg2rad(main_car.yaw);

  if (prev_size < 2)
  {
    double prev_car_x = ref_x - cos(ref_yaw);
    double prev_car_y = ref_y - sin(ref_yaw);

    ptsx.push_back(prev_car_x);
    ptsx.push_back(ref_x);

    ptsy.push_back(prev_car_y);
    ptsy.push_back(ref_y);
  }
  else
  {
    ref_x = prev_x_vals[prev_size-1];
    ref_y = prev_y_vals[prev_size-1];

    double ref_x_prev = prev_x_vals[prev_size-2];
    double ref_y_prev = prev_y_vals[prev_size-2];

    ptsx.push_back(ref_x_prev);
    ptsx.push_back(ref_x);

    ptsy.push_back(ref_y_prev);
    ptsy.push_back(ref_y);
  }

  vector<double> next_wp0 = getXY(main_car.s+30,(2+lane_width*target_lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
  vector<double> next_wp1 = getXY(main_car.s+60,(2+lane_width*target_lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
  vector<double> next_wp2 = getXY(main_car.s+90,(2+lane_width*target_lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);

  ptsx.push_back(next_wp0[0]);
  ptsx.push_back(next_wp1[0]);
  ptsx.push_back(next_wp2[0]);

  ptsy.push_back(next_wp0[1]);
  ptsy.push_back(next_wp1[1]);
  ptsy.push_back(next_wp2[1]);

  for (int i = 0; i < ptsx.size(); i++)
  {
    double shift_x = ptsx[i] - ref_x;
    double shift_y = ptsy[i] - ref_y;

    ptsx[i] = (shift_x * cos(0 - ref_yaw) - shift_y * sin(0 - ref_yaw));
    ptsy[i] = (shift_x * sin(0 - ref_yaw) + shift_y * cos(0 - ref_yaw));
  }

  tk::spline s;
  s.set_points(ptsx, ptsy);

  double target_x = 30.0;
  double target_y = s(target_x);
  double target_dist = sqrt(target_x * target_x + target_y * target_y);

  double x_add_on = 0;
  int buffer_size = (current_fsm_state == FSM_LANE_CHANGE_LEFT || current_fsm_state == FSM_LANE_CHANGE_RIGHT) ? 50 : 20;

  for (int i = 0; i < buffer_size - prev_size; i++)
  {
    double N = (target_dist/(0.02*ref_vel/2.24));

    double x_point = x_add_on + target_x / N;
    double y_point = s(x_point);

    x_add_on = x_point;

    double x_ref = x_point;
    double y_ref = y_point;

    x_point = (x_ref * cos(ref_yaw) - y_ref * sin(ref_yaw));
    y_point = (x_ref * sin(ref_yaw) + y_ref * cos(ref_yaw));

    x_point += ref_x;
    y_point += ref_y;

    next_x_vals.push_back(x_point);
    next_y_vals.push_back(y_point);
  }
}
