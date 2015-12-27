#include "EquipmentParameters.h"

Analysis::EquipmentParameters::EquipmentParameters(const double &lenD2,
                                                   const double &lenD1,
                                                   const double &lenL1,
                                                   const double &lenL2,
                                                   const double &lenL3,
                                                   const double &VElec,
                                                   const double &VIon1,
                                                   const double &VIon2,
                                                   const double &VIon0,
                                                   const double &B)
    : lengthOfD2(lenD2),
      lengthOfD1(lenD1),
      lengthOfL1(lenL1),
      lengthOfL2(lenL2),
      lengthOfL3(lenL3),
      electricPotentialOfElectronRegion(VElec),
      electricPotentialOfIon1st(VIon1),
      electricPotentialOfIon2nd(VIon2),
      electricPotentialOfIonMCP(VIon0),
      magneticFiled(B) {
  return;
}
Analysis::EquipmentParameters::EquipmentParameters(
    const Analysis::Unit &unit, const Analysis::JSONReader &reader)
    : EquipmentParameters(unit, reader, "equipment_parameters.") {
  return;
}
Analysis::EquipmentParameters::EquipmentParameters(const Analysis::Unit &unit,
                                                   const double &lenD2,
                                                   const double &lenD1,
                                                   const double &lenL1,
                                                   const double &lenL2,
                                                   const double &lenL3,
                                                   const double &VElec,
                                                   const double &VIon1,
                                                   const double &VIon2,
                                                   const double &VIon0,
                                                   const double &B)
    : EquipmentParameters(unit.readLength(lenD2),
                          unit.readLength(lenD1),
                          unit.readLength(lenL1),
                          unit.readLength(lenL2),
                          unit.readLength(lenL3),
                          unit.readElectricPotential(VElec),
                          unit.readElectricPotential(VIon1),
                          unit.readElectricPotential(VIon2),
                          unit.readElectricPotential(VIon0),
                          unit.readMagnaticField(B)) {
  return;
}
Analysis::EquipmentParameters::EquipmentParameters(const Analysis::Unit &unit,
                                                   const Analysis::JSONReader &reader,
                                                   const std::string &prefix)
    : EquipmentParameters(
    unit,
    reader.getDoubleAt(prefix + "length_of_D2"),
    reader.getDoubleAt(prefix + "length_of_D1"),
    reader.getDoubleAt(prefix + "length_of_L1"),
    reader.getDoubleAt(prefix + "length_of_L2"),
    reader.getDoubleAt(prefix + "length_of_L3"),
    reader.getDoubleAt(prefix + "electric_potential_of_electron_region"),
    reader.getDoubleAt(prefix + "electric_potential_of_ion_1st"),
    reader.getDoubleAt(prefix + "electric_potential_of_ion_2nd"),
    reader.getDoubleAt(prefix + "electric_potential_of_ion_MCP"),
    reader.getDoubleAt(prefix + "magnetic_filed")) {
  return;
}
Analysis::EquipmentParameters::~EquipmentParameters() { return; };
const double &Analysis::EquipmentParameters::getLengthOfD2() const {
  return this->lengthOfD2;
}
const double &Analysis::EquipmentParameters::getLengthOfD1() const {
  return this->lengthOfD1;
}
const double &Analysis::EquipmentParameters::getLengthOfL1() const {
  return this->lengthOfL1;
}
const double &Analysis::EquipmentParameters::getLengthOfL2() const {
  return this->lengthOfL2;
}
const double &Analysis::EquipmentParameters::getLengthOfL3() const {
  return this->lengthOfL3;
}
const double & Analysis::EquipmentParameters::getElectricPotentialOfElectronRegion()
const {
  return this->electricPotentialOfElectronRegion;
}
const double & Analysis::EquipmentParameters::getElectricPotentialOfIon1st()
const {
  return this->electricPotentialOfIon1st;
}
const double &Analysis::EquipmentParameters::getElectricPotentialOfIon2nd()
const {
  return this->electricPotentialOfIon2nd;
}
const double &Analysis::EquipmentParameters::getElectricPotentialOfIonMCP()
const {
  return this->electricPotentialOfIonMCP;
}
const double Analysis::EquipmentParameters::getElectricFiledOfD2() const {
  return 0e0;
}
const double Analysis::EquipmentParameters::getElectricFiledOfD1() const {
  return (this->getElectricPotentialOfElectronRegion()
      - this->getElectricPotentialOfIon1st())
      / (this->getLengthOfD1() + this->getLengthOfL1());
}
const double Analysis::EquipmentParameters::getElectricFiledOfL1() const {
  return this->getElectricFiledOfD1();
}
const double Analysis::EquipmentParameters::getElectricFiledOfL2() const {
  return (this->getElectricPotentialOfIon1st()
      - this->getElectricPotentialOfIon2nd()) / this->getLengthOfL2();
}
const double Analysis::EquipmentParameters::getElectricFiledOfL3() const {
  return (this->getElectricPotentialOfIon2nd()
      - this->getElectricPotentialOfIonMCP()) / this->getLengthOfL3();
}
const double &Analysis::EquipmentParameters::getMagneticFiled() const {
  return magneticFiled;
}
const double Analysis::EquipmentParameters::getLengthOfD2(const Analysis::Unit &unit) const {
  return unit.writeLength(getLengthOfD2());
}
const double Analysis::EquipmentParameters::getLengthOfD1(const Analysis::Unit &unit) const {
  return unit.writeLength(getLengthOfD1());
}
const double Analysis::EquipmentParameters::getLengthOfL1(const Analysis::Unit &unit) const {
  return unit.writeLength(getLengthOfL1());
}
const double Analysis::EquipmentParameters::getLengthOfL2(const Analysis::Unit &unit) const {
  return unit.writeLength(getLengthOfL2());
}
const double Analysis::EquipmentParameters::getLengthOfL3(const Analysis::Unit &unit) const {
  return unit.writeLength(getLengthOfL3());
}
const double Analysis::EquipmentParameters::getElectricPotentialOfElectronRegion(
    const Analysis::Unit &unit) const {
  return unit.writeElectricPotential(getElectricPotentialOfElectronRegion());
}
const double Analysis::EquipmentParameters::getElectricPotentialOfIon1st(
    const Analysis::Unit &unit) const {
  return unit.writeElectricPotential(getElectricPotentialOfIon1st());
}
const double Analysis::EquipmentParameters::getElectricPotentialOfIon2nd(
    const Analysis::Unit &unit) const {
  return unit.writeElectricPotential(getElectricPotentialOfIon2nd());
}
const double Analysis::EquipmentParameters::getElectricPotentialOfIonMCP(
    const Analysis::Unit &unit) const {
  return unit.writeElectricPotential(getElectricPotentialOfIonMCP());
}
const double Analysis::EquipmentParameters::getMagneticFiled(const Analysis::Unit &unit) const {
  return unit.writeMagnaticField(getMagneticFiled());
}
