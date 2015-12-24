#ifndef ANALYSIS_EQUIPMENTPARAMETERS_H
#define ANALYSIS_EQUIPMENTPARAMETERS_H

#include "Unit.h"
#include "JSONReader.h"

namespace Analysis {
class EquipmentParameters {
 private:
  const double lengthOfD2;
  const double lengthOfD1;
  const double lengthOfL1;
  const double lengthOfL2;
  const double lengthOfL3;
  const double electricPotentialOfElectronRegion;
  const double electricPotentialOfIon1st;
  const double electricPotentialOfIon2nd;
  const double electricPotentialOfIonMCP;
  const double magneticFiled;
  EquipmentParameters(const double &lenD2,
                      const double &lenD1,
                      const double &lenL1,
                      const double &lenL2,
                      const double &lenL3,
                      const double &VElec,
                      const double &VIon1,
                      const double &VIon2,
                      const double &VIon0,
                      const double &B); // main initializer
  EquipmentParameters(const Analysis::Unit &unit,
                      const double &lenD2,
                      const double &lenD1,
                      const double &lenL1,
                      const double &lenL2,
                      const double &lenL3,
                      const double &VElec,
                      const double &VIon1,
                      const double &VIon2,
                      const double &VIon0,
                      const double &B); // unit

 public:
  EquipmentParameters(const Unit &unit, const JSONReader &reader,
                      const std::string &prefix);
  EquipmentParameters(const Unit &unit, const JSONReader &reader);
  ~EquipmentParameters();
  const double &getLengthOfD2() const;
  const double &getLengthOfD1() const;
  const double &getLengthOfL1() const;
  const double &getLengthOfL2() const;
  const double &getLengthOfL3() const;
  const double &getElectricPotentialOfElectronRegion() const;
  const double &getElectricPotentialOfIon1st() const;
  const double &getElectricPotentialOfIon2nd() const;
  const double &getElectricPotentialOfIonMCP() const;
  const double &getMagneticFiled() const;
  const double getElectricFiledOfD2() const;
  const double getElectricFiledOfD1() const;
  const double getElectricFiledOfL1() const;
  const double getElectricFiledOfL2() const;
  const double getElectricFiledOfL3() const;
};
}

#endif
