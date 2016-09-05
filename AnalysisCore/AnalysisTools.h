#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#define LIMITATION_NUMBER_OF_LOOP 500

#include "EquipmentParameters.h"
#include "ObjectParameters.h"
#include "EventDataReader.h"
#include "Object.h"
#include "Objects.h"
namespace Analysis {
class AnalysisTools {
  struct XY {
    XY(const double &x, const double &y)
        : x(x), y(y) {
    }
    const double x;
    const double y;
  };
  EquipmentParameters equipParameters;
  ObjectParameters ionParameters;
  ObjectParameters elecParameters;
  int eventNumber;
  AnalysisTools(const EquipmentParameters &equip, const ObjectParameters &ion, const ObjectParameters &elec);
 public:
  AnalysisTools(const Unit &unit, const JSONReader &reader);
  ~AnalysisTools();

 private:
  void resetCounter();
  const XY calculateRotation(const XY &, const double &) const;
  const double calculateDiffTOF(const Object &obj, const double &) const;
  const XY calculateMomentumXY(const Object &obj) const;
 public:
  const EquipmentParameters &getEquipmentParameters() const;
  const ObjectParameters &getIonParameters() const;
  const ObjectParameters &getElectronParameters() const;
  const int &getEventNumber() const;
  void loadEventCounter();
  const double calculateTOF(const Object obj, const double d) const;
  const double calculateFrequencyOfCycle(const double &m, const double &q, const double &B) const;
  const double calculateFrequencyOfCycle(const Object &) const;
  const double calculatePeriodOfCycle(const double &m, const double &q, const double &B) const;
  const double calculatePeriodOfCycle(const Object &) const;
  const double calculateMomentumZ(const Object &obj, bool &info) const; // todo: find better way to calculate

 private:
  void loadEventDataInputer(Object &, const double &, const double &, const double &, const int &) const;
  void loadEventDataInputer(Analysis::Object &, const EventDataReader &, const int &) const;
 public:
  void loadEventDataInputer(Analysis::Objects &, const EventDataReader &) const;

 private: 
  void loadMomentumCalculator(Object &obj) const;
 public:
  void loadMomentumCalculator(Objects &objs) const;
};
}
#endif

