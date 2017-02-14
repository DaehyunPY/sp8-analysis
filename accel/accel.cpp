//
// Created by daehyun on 2/11/17.
//

#include "accel/accel.h"

sp8::Acc sp8::genAcc(double E, double len) {
  return [=](double m, double q, double p0)->tuple<double, double> {
    double f = E*q;
    if (f==0.0) {
      double dt = len/(p0/m);
      if (dt < 0.0) throw invalid_argument("Flight time is minus! This object can not be accelerated!");
      return {p0, dt};
    }
    double e0 = pow(p0, 2.0)/(2.0*m);
    double e1 = e0+f*len;
    if (e1 < 0.0) throw invalid_argument("Kinetic energy is minus! This object can not be accelerated!");
    auto sign = [](double d) -> double {
      if (d > 0.0) return 1.0;
      else if (d < 0.0) return -1.0;
      else return 0;
    };
    double p1 = sign(f)*pow(2.0*m*e1, 0.5);
    double dt = (p1-p0)/f;
    if (dt < 0.0) throw invalid_argument("Flight time is minus! This object can not be accelerated!");
    return {p1, dt};
  };
}
template<> sp8::Acc sp8::mulAccs(sp8::Acc a1, sp8::Acc a2) {
  return [=](double m, double q, double p0)->tuple<double, double> {
  double p1, dt1;
  tie(p1, dt1) = a1(m, q, p0);
  double p2, dt2;
  tie(p2, dt2) = a2(m, q, p1);
  return {p2, dt1 + dt2};
  };
}
sp8::AccMap::AccMap(sp8::Acc acc, double m, double q, double pfr, double pto, int bin) {
  if (!(pfr < pto)) throw invalid_argument("Upper limit of p is smaller than lower limit!");
  if (!(bin > 1)) throw invalid_argument("Number of bins has to be 2 or larger!");

  flightTime = [=](double p)->double {
    return get<1>(acc(m, q, p));
  };

  parr.reserve((unsigned long) bin);
  parr.resize((unsigned long) bin);
  tarr.reserve((unsigned long) bin);
  tarr.resize((unsigned long) bin);
  double dp = (pto-pfr)/(bin-1);
  for(int i=0; i<bin; i++) {
    parr[i] = pfr + dp*i;
    tarr[i] = flightTime(parr[i]);
  }
  tfr = tarr[0];
  tto = tarr[bin-1];
  if (!(tfr < tto)) {
    auto tmp = tfr;
    tfr = tto;
    tto = tmp;
    reverse(parr.begin(), parr.end());
    reverse(tarr.begin(), tarr.end());
  }

  intpAcc = gsl_interp_accel_alloc();
  spline = gsl_spline_alloc(gsl_interp_cspline, (size_t) bin);
  gsl_spline_init(spline, tarr.data(), parr.data(), (size_t) bin);
}
double sp8::AccMap::getInitMomentum(double t) const {
  if (!isValidTime(t)) throw range_error("Arg t is over the range!");
  return gsl_spline_eval(spline, t, intpAcc);
}
sp8::AccMap::~AccMap() {
  gsl_spline_free(spline);
  gsl_interp_accel_free(intpAcc);
}
std::vector<double> sp8::AccMap::getPArr() const {
  return parr;
}
std::tuple<double, double> sp8::AccMap::getTimeLimit() const {
  return {tfr, tto};
}
std::vector<double> sp8::AccMap::getTArr() const {
  return tarr;
}
bool sp8::AccMap::isValidTime(double t) const {
  return (tfr<=t) && (t<=tto);
}
double sp8::AccMap::getFlightTime(double p) const {
  return flightTime(p);
}
