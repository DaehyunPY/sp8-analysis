//
// Created by daehyun on 2/11/17.
//

#include <pybind11/functional.h>
#include "accel.h"

namespace py = pybind11;
using namespace sp8;

PYBIND11_PLUGIN(accel) {
  py::module m("accel", R"pbdoc(
    accel module binded by pybind11
    -------------------------------

    .. currentmodule:: accel

    .. autosummary::
       :toctree: _generate

    genAcc
    mulAccs
    AccMap
  )pbdoc");
  m.def("genAcc", &genAcc);
  m.def("mulAccs", &mulAccs<>);
  py::class_<AccMap>(m, "AccMap")
      .def(py::init<Acc, double, double, double, double, int>())
      .def("getTAtP", &AccMap::getTAtP)
      .def("getPAtT", &AccMap::getPAtT)
      .def("getPLimit", &AccMap::getPLimit)
      .def("getTLimit", &AccMap::getTLimit)
      .def("getPArr", &AccMap::getPArr)
      .def("getTArr", &AccMap::getTArr);
  return m.ptr();
}