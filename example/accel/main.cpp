//
// Created by daehyun on 2/11/17.
//
#include <iostream>
#include "accel/accel.h"

using namespace sp8;
int main() {
//  Generate accelerator
  double e=1, l=1;
  auto acc = genAcc(
      /* electric field */ e,
      /* field length */ l
  );
  std::cout << "Electric Field: " << e << std::endl;
  std::cout << "Field Length: " << l << std::endl;

//  Calculate momentum and flight time after acceleration
  double m=3, q=8, p0=1;
  double p1, dt;
  tie(p1,dt) = acc(
      /* mass */ m,
      /* charge */ q,
      /* initial momentum */ p0
  );
  std::cout << "Mass: " << m << std::endl;
  std::cout << "Charge: " << q << std::endl;
  std::cout << "Initial Momentum: " << p0 << std::endl;
  std::cout << "Momentum after Acceleration: " << p1 << std::endl;
  std::cout << "Flight Time: " << dt << std::endl;

//  Multiple accelerators
  auto twoAccs = mulAccs(acc, acc);

//  Make a instance of AccMap to calculate momentum from flight time
  double pfr=-8, pto=8;
  int bin=100;
  AccMap map(
      /* accelerator */ twoAccs,
      /* mass */ m,
      /* charge */ q,
      /* lower limit of p */ pfr,
      /* upper limit of p */ pto,
      /* number of bins */ bin);
  std::cout << "Mapping Momentum and Flight Time of Two Accelerators..." << std::endl;
  std::cout << "Lower Limit of Momentum: " << pfr << std::endl;
  std::cout << "Upper Limit of Momentum: " << pto << std::endl;
  std::cout << "Number of Bins: " << bin << std::endl;

//  Calculate momentum from flight time
  double t=2;
  double p=map.getInitMomentum(t);
  std::cout << "Input Flight Time: " << t << std::endl;
  std::cout << "Result Momentum: " << p << std::endl;
  std::cout << "Check the Result: " << map.getFlightTime(p)-t << std::endl;

  return 0;
}