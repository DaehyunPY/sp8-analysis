//
// Created by daehyun on 2/11/17.
//

#include "pybind11/functional.h"
#include "accel/accel.h"

using pybind11::module;
using pybind11::class_;
using pybind11::init;
using namespace pybind11::literals;
using namespace sp8;

PYBIND11_PLUGIN(accel) {
  module m("accel");
  m.def("genAcc", &genAcc, "E"_a, "len"_a);
  m.def("mulAccs", &mulAccs<>, "acc1"_a, "acc2"_a);
  class_<AccMap>(m, "AccMap")
      .def(init<Acc, double, double, double, double, int>(),
           "acc"_a, "m"_a, "q"_a, "pfr"_a, "pto"_a, "bin"_a)
      .def("getMomentumLimit", &AccMap::getMomentumLimit)
      .def("getTimeLimit", &AccMap::getTimeLimit)
      .def("isValidTime", &AccMap::isValidTime, "t"_a)
      .def("getPArr", &AccMap::getPArr)
      .def("getTArr", &AccMap::getTArr)
      .def("getFlightTime", &AccMap::getFlightTime, "p"_a)
      .def("getInitMomentum", &AccMap::getInitMomentum, "p"_a);
  return m.ptr();
}