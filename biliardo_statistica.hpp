#ifndef BILIARDO_STATISTICA_HPP
#define BILIARDO_STATISTICA_HPP

#include "biliardo.hpp"
namespace pf { 
struct StatsResult {
  double mean_yf;
  double stdev_yf;
  double coeff_simm_yf;
  double coeff_app_yf;
  double mean_thf;
  double stdev_thf;
  double coeff_simm_thf;
  double coeff_app_thf;
  int success_count;
};

StatsResult simulate_stats(int N, double mu_y0, double sigma_y0, double mu_th0,
                           double sigma_th0, pf::Border b1, pf::Border b2,
                           double L);
}
#endif