#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  this->Kp = Kp_;
  this->Ki = Ki_;
  this->Kd = Kd_;
}

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
