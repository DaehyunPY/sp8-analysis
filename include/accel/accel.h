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
typedef function<double(double)> SimpleFunc;

Acc genAcc(double E, double len);
template <typename... As> Acc mulAccs(Acc a1, Acc a2, As... as);
template <> Acc mulAccs<>(Acc a1, Acc a2);
template <typename... As>
Acc mulAccs(Acc a1, Acc a2, As... as) {
  return mulAccs(mulAccs(a1, a2), as...);
}

class AccMap {
  SimpleFunc flightTime;
  double tfr, tto;
  vector<double> parr, tarr;
  gsl_interp_accel *intpAcc;
  gsl_spline *spline;

 public:
  AccMap(Acc acc, double m, double q, double pfr, double pto, int bin);
  ~AccMap();
  tuple<double, double> getTimeLimit() const;
  bool isValidTime(double t) const;
  vector<double> getPArr() const;
  vector<double> getTArr() const;
  double getFlightTime(double p) const;
  double getInitMomentum(double t) const;
};
}

#endif //CMAKE_EXAMPLE_ACCEL_H
