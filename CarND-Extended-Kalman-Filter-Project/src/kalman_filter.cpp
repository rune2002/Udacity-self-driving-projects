#include "kalman_filter.h"
#include <math.h>

using Eigen::MatrixXd;
using Eigen::VectorXd;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  x_ = F_ * x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  VectorXd y = z - H_ * x_;
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K =  P_ * Ht * Si;

  MatrixXd I = MatrixXd::Identity(4, 4);
  x_ = x_ + (K * y);
  P_ = (I - K * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  VectorXd hx(3);
  float ro = sqrt(float(x_(0)*x_(0)+x_(1)*x_(1)));
  float theta = atan2(x_(1), x_(0));
  float ro_dot = (x_(0)*x_(2)+x_(1)*x_(3)) / ro;
  hx << ro, theta, ro_dot;

  VectorXd y = z - hx;
  if (y(1) > M_PI)
    y(1) = y(1) - 2 * M_PI;
  else if (y(1) < -M_PI)
    y(1) = y(1) + 2 * M_PI; 

  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K =  P_ * Ht * Si;

  MatrixXd I = MatrixXd::Identity(4, 4);
  x_ = x_ + (K * y);
  P_ = (I - K * H_) * P_;
}

