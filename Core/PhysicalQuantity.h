//
// Created by Daehyun You on 10/16/16.
//

#ifndef ANALYSIS_PHYSICALQUANTITY_H
#define ANALYSIS_PHYSICALQUANTITY_H

#include <cassert>
#include <iostream>
#include "Unit.h"

namespace Analysis {

#define CONVERT_ERR \
else { \
  std::cout << "It is not supported convert type!" << std::endl; \
  assert(false); \
}

struct SimpleValue {
  const double v;
  const UnitType type;
  SimpleValue(const double v, const UnitType tp=None) : v(v), type(tp) {}
};

struct Angle {
  const double theta;
  const UnitType type; // Radian, Degree
  Angle(const double th, const UnitType tp=Radian) : theta(th), type(tp) {}
  Angle convert(UnitType newType=Radian) {
    if (type==newType) {
      return *this;
    } else if (type==Radian && newType==Degree) {
      return Angle(kUnit.writeDegree(theta), Degree);
    } else if (type==Degree && newType==Radian) {
      return Angle(kUnit.readDegree(theta), Radian);
    } CONVERT_ERR
  }
  Angle operator +(Angle angle) {
    if (angle.type != type) {
      return *this + angle.convert(type);
    } else {
      return Angle(theta + angle.theta, type);
    }
  }
  Angle operator *(double d) {
    return Angle(d*theta, type);
  }
  Angle operator /(double d) {
    return *this * (1.0/d);
  }
  Angle operator -(Angle angle) {
    return *this + angle*(-1.0);
  }
};

struct Image {
  const double x, y;
  const UnitType type; // AU, MilliMeter
  Image(const double x, const double y, const UnitType tp=AU): x(x), y(y), type(tp) {}
  Image convert(UnitType newType=AU) {
    if (type==newType) {
      return *this;
    } else if (type==AU && newType==MilliMeter) {
      return Image(kUnit.writeMilliMeter(x), kUnit.writeMilliMeter(y), MilliMeter);
    } else if (type==MilliMeter && newType==AU) {
      return Image(kUnit.readMilliMeter(x), kUnit.readMilliMeter(y), AU);
    } CONVERT_ERR
  }
  Image operator +(Image image) {
    if (image.type != type) {
      return *this + image.convert(type);
    } else {
      return Image(x + image.x, y + image.y, type);
    }
  }
  Image operator *(double d) {
    return Image(d*x, d*y, type);
  }
  Image operator /(double d) {
    return *this * (1.0/d);
  }
  Image operator -(Image image) {
    return *this + image*(-1.0);
  }
  SimpleValue norm() {
    return SimpleValue(pow(x*x + y*y, 0.5), type);
  }
};

struct Time {
  const double t;
  const UnitType type; // AU, NanoSec
  Time(const double t, const UnitType tp=AU): t(t), type(tp) {}
  Time convert(UnitType newType=AU) {
    if (type==newType) {
      return *this;
    } else if (type==AU && newType==NanoSec) {
      return Time(kUnit.writeNanoSec(t), NanoSec);
    } else if (type==NanoSec && newType==AU) {
      return Time(kUnit.readNanoSec(t), AU);
    } CONVERT_ERR
  }
  Time operator +(Time time) {
    if (time.type != type) {
      return *this + time.convert(type);
    } else {
      return Time(t + time.t, type);
    }
  }
  Time operator *(double d) {
    return Time(d*t, type);
  }
  Time operator /(double d) {
    return *this * (1.0/d);
  }
  Time operator -(Time time) {
    return *this + time*(-1.0);
  }
};

struct Mass {
  const double m;
  const UnitType type; // AU, AMU
  Mass(const double m, const UnitType tp=AU) : m(m), type(tp) {}
  Mass convert(UnitType newType=AU) {
    if (type==newType) {
      return *this;
    } else if (type==AU && newType==AMU) {
      return Mass(kUnit.writeAtomicMass(m), AMU);
    } else if (type==AMU && newType==AU) {
      return Mass(kUnit.readAtomicMass(m), AU);
    } CONVERT_ERR
  }
};

struct Momentum {
  const double x, y, z;
  const UnitType type = AU; // AU
  Momentum(const double x, const double y, const double z, const UnitType tp) :
      x(x), y(y), z(z), type(tp) {}
  Momentum convert(UnitType newType=AU) {
    if (type==newType) {
      return *this;
    } CONVERT_ERR
  }
  Momentum operator +(Momentum p) {
    if (type != p.type) {
      return *this + p.convert(type);
    } else {
      return Momentum(x + p.x, y + p.y, z + p.z, type);
    }
  }
  Momentum operator *(double d) {
    return Momentum(d*x, d*y, d*z, type);
  }
  Momentum operator /(double d) {
    return *this * (1.0/d);
  }
  Momentum operator -(Momentum p) {
    return *this + p*(-1.0);
  }
  SimpleValue norm() {
    return SimpleValue(pow(x*x + y*y + z*z, 0.5), type);
  }
};

struct Energy {
  const double e;
  const UnitType type = AU; // AU, ElectronVolt
  Energy(const double e, const UnitType tp) : e(e), type(tp) {}
  Energy convert(UnitType newType=AU) {
    if (type==newType) {
      return *this;
    } else if (type==AU && newType==ElectronVolt) {
      return Energy(kUnit.writeElectronVolt(e), ElectronVolt);
    } else if (type==ElectronVolt && newType==AU) {
      return Energy(kUnit.readElectronVolt(e), AU);
    } CONVERT_ERR
  }
  Energy operator +(Energy energy) {
    if (energy.type != type) {
      return *this + energy.convert(type);
    } else {
      return Energy(e + energy.e, type);
    }
  }
  Energy operator *(double d) {
    return Energy(d*e, type);
  }
  Energy operator /(double d) {
    return *this * (1.0/d);
  }
  Energy operator -(Energy energy) {
    return *this + energy*(-1.0);
  }
};


}

#endif //ANALYSIS_PHYSICALQUANTITY_H
