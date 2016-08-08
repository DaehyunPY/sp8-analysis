#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#define LIMITATION_NUMBER_OF_LOOP 100

#include "EquipmentParameters.h"
#include "ObjectParameters.h"
#include "EventDataReader.h"
#include "Object.h"
#include "Objects.h"
namespace Analysis {
class AnalysisTools {
  // tpyes
 private:
  struct XY {
    XY(const double &x, const double &y)
        : x(x), y(y) {
    }
    const double x;
    const double y;
  };

  // datas
 private:
  EquipmentParameters equipParameters;
  ObjectParameters ionParameters;
  ObjectParameters elecParameters;
  std::string ID;
  int eventNumber;

 private:
  AnalysisTools( // main initializer
      const EquipmentParameters &equip,
      const ObjectParameters &ion,
      const ObjectParameters &elec,
      const std::string ID = "");

 public:
  AnalysisTools(const Unit &unit, const JSONReader &reader);
  ~AnalysisTools();

 private:
  void resetCounter();
  const XY calculateRotation(const XY &, const double &) const;
  const double calculateDiffTOF(const Object &obj, const double &) const;
  const XY calculateMomentumXY(const Object &obj) const;
  const double calculateMomentumZ(const Object &obj, bool &info) const; // todo: find better way to calculate
  void loadEventDataInputer(Object &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const;
  void loadEventDataInputer(Object &,
                            const Unit &,
                            const EventDataReader &,
                            const int &) const;

 public:
  const EquipmentParameters &getEquipmentParameters() const;
  const ObjectParameters &getIonParameters() const;
  const ObjectParameters &getElectronParameters() const;
  const std::string &getID() const;
  const int &getEventNumber() const;
  void loadEventCounter();
  const double calculateTOF(const Object obj,
                            const double d) const;
  const double calculateFrequencyOfCycle(const double &m,
                                         const double &q,
                                         const double &B) const;
  const double calculateFrequencyOfCycle(const Object &) const;
  const double calculatePeriodOfCycle(const double &m,
                                      const double &q, const double &B) const;
  const double calculatePeriodOfCycle(const Object &) const;
  void loadEventDataInputer(Object &,
                            const Unit &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const; // for unit
  void loadEventDataInputer(Objects &,
                            const Unit &,
                            const EventDataReader &) const; // for ions
 public:
  enum OptName {
    OnlyWithinMasterRegion, OnlyNotDead, AllRegion
  };
  void loadMomentumCalculator(Object &obj, const OptName opt=OnlyWithinMasterRegion) const; // main momentum calculator
  void loadMomentumCalculator(Objects &objs, const OptName opt=OnlyWithinMasterRegion) const; // for ions
};
}
#endif

