#include "Proj.h"

void projx(double reg1=0, double reg2=0) {
  auto *proj = new Proj(Proj::AxisType::x, reg1, reg2);
}
