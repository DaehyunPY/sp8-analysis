//
// Created by daehyun on 2/11/17.
//

#ifndef CMAKE_EXAMPLE_ACCEL_H
#define CMAKE_EXAMPLE_ACCEL_H

#include <array>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <vector>
#include <gsl/gsl_spline.h>
#include <algorithm>

namespace sp8 {
using std::function;
using std::tuple;
using std::get;
using std::tie;
using std::pow;
using std::invalid_argument;
using std::range_error;
using std::vector;
using std::reverse;

typedef function<tuple<double, double>(double, double, double)> Acc;
Acc genAcc(double E, double len);

template <typename... As> Acc mulAccs(Acc a1, Acc a2, As... as);
template <> Acc mulAccs<>(Acc a1, Acc a2);
template <typename... As>
Acc mulAccs(Acc a1, Acc a2, As... as) {
  return mulAccs(mulAccs(a1, a2), as...);
}

class AccMap {
  const double m, q;
  const Acc acc;
  const double pfr, pto;
  const int bin;
  double tfr, tto;
  vector<double> parr, tarr;
  gsl_interp_accel *intpAcc;
  gsl_spline *spline;

 public:
  AccMap(Acc acc, double m, double q, double pfr, double pto, int bin);
  ~AccMap();
  double getTAtP(double p) const;
  double getPAtT(double t) const;
  tuple<double, double> getPLimit() const;
  tuple<double, double> getTLimit() const;
  vector<double> getPArr() const;
  vector<double> getTArr() const;
};
}

#endif //CMAKE_EXAMPLE_ACCEL_H
