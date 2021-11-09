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

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */
  num_particles = 50;  // TODO: Set the number of particles
  
  std::default_random_engine gen;
  
  std::normal_distribution<double> dist_x(x, std[0]);
  std::normal_distribution<double> dist_y(y, std[1]);
  std::normal_distribution<double> dist_theta(theta, std[2]);

  for (int i = 0; i < num_particles; ++i) {
    double sample_x, sample_y, sample_theta;
    Particle particle;
    
    sample_x = dist_x(gen);
    sample_y = dist_y(gen);
    sample_theta = dist_theta(gen);   
    
    particle.id = i;
    particle.x = sample_x;
    particle.y = sample_y;
    particle.theta = sample_theta;
    particle.weight = 1.0;
    particles.push_back(particle);
    
    weights.push_back(particle.weight);
  }
  
  is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */
  
  std::default_random_engine gen;
  
  for (int i = 0; i < num_particles; ++i) {
    
    double x = particles[i].x;
    double y = particles[i].y;
    double theta = particles[i].theta;
    double next_x;
    double next_y;
    double next_theta;
    
    if (fabs(yaw_rate) < 0.001) {
      next_x = x + velocity * cos(theta) * delta_t;
      next_y = y + velocity * sin(theta) * delta_t;
      next_theta = theta;
    }
    else{
      next_x = x + (velocity/yaw_rate) * (sin(theta + (yaw_rate * delta_t)) - sin(theta));
      next_y = y + (velocity/yaw_rate) * (cos(theta) - cos(theta + (yaw_rate * delta_t)));
      next_theta = theta + (yaw_rate * delta_t);
    }
      
    std::normal_distribution<double> dist_x(next_x, std_pos[0]);
    std::normal_distribution<double> dist_y(next_y, std_pos[1]);
    std::normal_distribution<double> dist_theta(next_theta, std_pos[2]);
    
    next_x = dist_x(gen);
    next_y = dist_y(gen);
    next_theta = dist_theta(gen);   
    
    particles[i].x = next_x;
    particles[i].y = next_y;
    particles[i].theta = next_theta;
  }

}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  
  
  for (int i = 0; i < observations.size(); ++i) {
    double distance_min = std::numeric_limits<double>::infinity();
    int closest_id = -1;
    
    double obs_x = observations[i].x;
    double obs_y = observations[i].y;

    for (int j = 0; j < predicted.size(); ++j) {
      double pred_x = predicted[j].x;
      double pred_y = predicted[j].y;
      int pred_id = predicted[j].id;
      
      double distance = dist(obs_x, obs_y, pred_x, pred_y);

      if (distance < distance_min) {
        distance_min = distance;
        closest_id = pred_id;
      }
    }
    observations[i].id = closest_id;
  }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */
  double sig_x = std_landmark[0];
  double sig_y = std_landmark[1];
  
  for (int i = 0; i < num_particles; ++i){
    double x = particles[i].x;
    double y = particles[i].y;
    double theta = particles[i].theta;
    particles[i].weight = 1.0;
    
    std::vector<LandmarkObs> close_landmarks;
    LandmarkObs landmark;
    
    
    for(int j = 0; j < map_landmarks.landmark_list.size(); ++j){
      double landmark_dist = dist(x, y, map_landmarks.landmark_list[j].x_f, map_landmarks.landmark_list[j].y_f);
      
      if(landmark_dist <= sensor_range){
        landmark.id = map_landmarks.landmark_list[j].id_i;
        landmark.x = map_landmarks.landmark_list[j].x_f;
        landmark.y = map_landmarks.landmark_list[j].y_f;
        close_landmarks.push_back(landmark);
      } 
    }
    
    std::vector<LandmarkObs> converted_observations;
    
    for (int j = 0; j < observations.size(); ++j){
      landmark.x = observations[j].x*cos(theta) - observations[j].y*sin(theta) + x;
      landmark.y = observations[j].x*sin(theta) + observations[j].y*cos(theta) + y;
      landmark.id = observations[j].id;
      converted_observations.push_back(landmark);
    }
    
    dataAssociation(close_landmarks, converted_observations);

    for (int j = 0; j < converted_observations.size(); ++j) {

      // find the landmark
      int observe_id = converted_observations[j].id;
      double observe_x = converted_observations[j].x;
      double observe_y = converted_observations[j].y;
      double match_x, match_y;

      for (int k = 0; k < close_landmarks.size(); ++k) {
        if (close_landmarks[k].id == observe_id) {
          match_x = close_landmarks[k].x;
          match_y = close_landmarks[k].y;
        }
      }

      // multivariate Gaussian
      double w = ( 1/(2*M_PI*sig_x*sig_y)) * exp( -(pow(match_x-observe_x,2)/(2*pow(sig_x, 2)) + (pow(match_y-observe_y,2)/(2*pow(sig_y, 2)))));

      particles[i].weight *= w;
    }
    
    weights[i] = particles[i].weight;
  }
}



void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */
  
  std::default_random_engine gen;
  
  vector<Particle> resample_particles;
  
  std::discrete_distribution<int> weight_distribution(weights.begin(), weights.end());
  
  for (int i = 0; i < num_particles; ++i){
    resample_particles.push_back(particles[weight_distribution(gen)]);
  }
  
  particles = resample_particles;
  
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