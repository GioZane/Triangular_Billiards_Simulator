#include "biliardo_statistica.hpp"
#include "biliardo.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace pf { 
StatsResult simulate_stats(int N, double mu_y0, double sigma_y0, double mu_th0,
                           double sigma_th0, pf::Border b1, pf::Border b2,
                           double L) {
  std::random_device rd;
  std::default_random_engine eng{rd()};
  std::normal_distribution<double> dist_y(mu_y0, sigma_y0);
  std::normal_distribution<double> dist_th(mu_th0, sigma_th0);

  std::vector<double> y_result;
  std::vector<double> th_result;

  int success_count = 0;

  for (int i = 0; i < N; ++i) {
    double y0 = dist_y(eng);
    double th0 = dist_th(eng);


    try {
      pf::Ball ball({0.0, y0}, th0);
      pf::Result res = pf::Result::BallSimulation(b1, b2, ball);

      if (res.result.coordba().x >= L && b2.r2() <= res.result.coordba().y &&
          res.result.coordba().y <= b1.r2()) {
        y_result.push_back(res.result.coordba().y);
        th_result.push_back(res.result.d());
        success_count++;
      }
    } catch (const std::runtime_error &e) {
    }
  }

  struct Stats {
    double mean;
    double stdev;
    double coeff_simm;
    double coeff_app;
  };

  auto parameters = [](const std::vector<double> &data) -> Stats {
    int n = static_cast<int>(data.size());

    if (n == 0)
      return Stats{0, 0, 0, 0};

    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    double mean = sum / n;

    double variance =
        std::accumulate(data.begin(), data.end(), 0.0,
                        [mean](double acc, double val) {
                          return acc + (val - mean) * (val - mean);
                        }) /
        n;
    double stdev = std::sqrt(variance);

    double coeff_simm = 0.0;
    double coeff_app = 0.0;

    if (stdev != 0.0) {
      coeff_simm = std::accumulate(data.begin(), data.end(), 0.0,
                                   [mean, stdev](double acc, double val) {
                                     double z = (val - mean) / stdev;
                                     return acc + z * z * z;
                                   }) /
                   n;

      coeff_app = std::accumulate(data.begin(), data.end(), 0.0,
                                  [mean, stdev](double acc, double val) {
                                    double z = (val - mean) / stdev;
                                    return acc + z * z * z * z;
                                  }) /
                  n;
    }

    return Stats{mean, stdev, coeff_simm, coeff_app};
  };

  Stats y_stats = parameters(y_result);
  Stats th_stats = parameters(th_result);

  return {y_stats.mean,        y_stats.stdev,      y_stats.coeff_simm,
          y_stats.coeff_app,   th_stats.mean,      th_stats.stdev,
          th_stats.coeff_simm, th_stats.coeff_app, success_count};
}

} 