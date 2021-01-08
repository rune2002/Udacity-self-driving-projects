/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "helper_functions.h"

using std::string;
using std::vector;
using std::normal_distribution;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  num_particles = 100;

  std::default_random_engine gen;
  double std_x = std[0];
  double std_y = std[1];
  double std_theta = std[2];

  normal_distribution<double> dist_x(x, std_x);
  normal_distribution<double> dist_y(y, std_y);
  normal_distribution<double> dist_theta(theta, std_theta);

  for (int i = 0; i < num_particles; i++)
  {
	  double sample_x, sample_y, sample_theta;

	  sample_x = dist_x(gen);
	  sample_y = dist_y(gen);
	  sample_theta = dist_theta(gen);

	  Particle particle;
	  particle.id = i;
	  particle.x = sample_x;
	  particle.y = sample_y;
	  particle.theta = sample_theta;
	  particle.weight = 1.0;

	  particles.push_back(particle);
  }
  is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  std::default_random_engine gen;
  double std_x = std_pos[0];
  double std_y = std_pos[1];
  double std_theta = std_pos[2];

  normal_distribution<double> dist_x(0, std_x);
  normal_distribution<double> dist_y(0, std_y);
  normal_distribution<double> dist_theta(0, std_theta);

  for (std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it)
  {
	  double sample_x, sample_y, sample_theta;
	  double theta_f, theta_0;
	  theta_f = fmod(it->theta + yaw_rate * delta_t, 2 * M_PI);
	  theta_0 = fmod(it->theta, 2 * M_PI);

	  sample_x = dist_x(gen);
	  sample_y = dist_y(gen);
	  sample_theta = dist_theta(gen);

	  if (abs(yaw_rate) < 1e-8)
	  {
	    it->x = it->x + velocity * delta_t * cos(theta_0) + sample_x;
	    it->y = it->y + velocity * delta_t * sin(theta_0) + sample_y;
	  }
	  else
	  {
	    it->x = it->x + velocity / yaw_rate * (sin(theta_f) - sin(theta_0)) + sample_x;
	    it->y = it->y + velocity / yaw_rate * (cos(theta_0) - cos(theta_f)) + sample_y;
	  }
	  it->theta = it->theta + yaw_rate * delta_t + sample_theta;
  }
}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations,
                                     double threshold) {
  for (std::vector<LandmarkObs>::iterator it1 = observations.begin(); it1 != observations.end(); ++it1)
  {
    int id = -1;
    double min_distance = threshold;

    for (std::vector<LandmarkObs>::iterator it2 = predicted.begin(); it2 != predicted.end(); ++it2)
    {
      double distance = dist(it1->x, it1->y, it2->x, it2->y);
      if (distance < min_distance)
      {
        min_distance = distance;
        id = it2->id;
      }
    }

    it1->id = id;
  }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations,
                                   const Map &map_landmarks) {
  if (observations.empty())
    return;

  vector<LandmarkObs> predicted;
  int num_landmark = map_landmarks.landmark_list.size();
  for (int i = 0; i < num_landmark; i++)
  {
	  LandmarkObs landmark;
	  landmark.id = map_landmarks.landmark_list[i].id_i;
	  landmark.x = (double) map_landmarks.landmark_list[i].x_f;
	  landmark.y = (double) map_landmarks.landmark_list[i].y_f;

	  predicted.push_back(landmark);
  }

  double std_x, std_y;
  std_x = std_landmark[0];
  std_y = std_landmark[1];
  weights.clear();

  for (std::vector<Particle>::iterator it1 = particles.begin(); it1 != particles.end(); ++it1)
  {
    vector<LandmarkObs> observations_map = observations;
    double x_part, y_part, theta;
    x_part = it1->x;
    y_part = it1->y;
    theta = fmod(it1->theta, 2 * M_PI);

    for (std::vector<LandmarkObs>::iterator it2 = observations_map.begin(); it2 != observations_map.end(); ++it2)
    {
      double x_obs, y_obs, x_map, y_map;
      x_obs = it2->x;
      y_obs = it2->y;
      x_map = x_part + (cos(theta) * x_obs) - (sin(theta) * y_obs);
      y_map = y_part + (sin(theta) * x_obs) + (cos(theta) * y_obs);

      it2->x = x_map;
      it2->y = y_map;
    }

    dataAssociation(predicted, observations_map);

    double x_obs, y_obs, mu_x, mu_y, weight;
	  weight = 1.0;

	  for (std::vector<LandmarkObs>::iterator it3 = observations_map.begin(); it3 != observations_map.end(); ++it3)
	  {
	    mu_x = predicted[it3->id - 1].x;
	    mu_y = predicted[it3->id - 1].y;
	    x_obs = it3->x;
	    y_obs = it3->y;
	    if (dist(mu_x, mu_y, x_obs, y_obs) < sensor_range)
	      weight = weight * multiv_prob(std_x, std_y, x_obs, y_obs, mu_x, mu_y);
	  }
	  it1->weight = weight;
	  weights.push_back(weight);
  }
}

void ParticleFilter::resample() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> d{weights.begin(), weights.end()};
  vector<Particle> resampled;

  for (int i = 0; i <num_particles; i++)
  {
    Particle particle = particles[d(gen)];
    particle.id = i;
    resampled.push_back(particle);
  }
  particles = resampled;
}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}
