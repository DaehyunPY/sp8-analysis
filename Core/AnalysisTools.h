#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#define LIMITATION_NUMBER_OF_LOOP 100

#include "EquipmentParameters.h"
#include "ObjectParameters.h"
#include "Ions.h"
#include "Electrons.h"
#include "EventDataReader.h"
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
  AnalysisTools(const Unit &, const JSONReader &);
  ~AnalysisTools();

 private:
  void resetCounter();
  const XY calculateRotation(const XY &, const double &) const;
  const double calculateDiffTOF(const Ion &, const double &) const;
  const double calculateDiffTOF(const Electron &, const double &) const;
  const XY calculateMomentumXY(const Object &) const;
  const double calculateMomentumZ(const Ion &, bool &info) const; // todo: find better way to calculate 
  const double calculateMomentumZ(const Electron &, bool &info) const; // todo: find better way to calculate
  void loadEventDataInputer(Ion &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const; // main inputer for ion
  void loadEventDataInputer(Electron &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const; // main inputer for electron
  void loadEventDataInputer(Ion &,
                            const Unit &,
                            const EventDataReader &,
                            const int &) const; // for reader
  void loadEventDataInputer(Electron &,
                            const Unit &,
                            const EventDataReader &,
                            const int &) const; // for reader

 public:
  const EquipmentParameters &getEquipmentParameters() const;
  const ObjectParameters &getIonParameters() const;
  const ObjectParameters &getElectronParameters() const;
  const std::string &getID() const;
  const int &getEventNumber() const;
  void loadEventCounter();
  const double calculateTOF(const Ion &ion,
                            const double &d) const;
  const double calculateTOF(const Unit &unit,
                            const Ion &ion,
                            const double &d) const;
  const double calculateTOF(const Electron &elec,
                            const double &d) const;
  const double calculateTOF(const Unit &unit,
                            const Electron &elec,
                            const double &d) const;
  const double calculateFrequencyOfCycle(const double &m,
                                         const double &q,
                                         const double &B) const;
  const double calculateFrequencyOfCycle(const Object &) const;
  const double calculatePeriodOfCycle(const double &m,
                                      const double &q, const double &B) const;
  const double calculatePeriodOfCycle(const Object &) const;
  const double calculatePeriodOfCycle(const Unit &,
                                      const Object &) const;
  void loadEventDataInputer(Ion &,
                            const Unit &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const; // for unit
  void loadEventDataInputer(Electron &,
                            const Unit &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const; // for unit
  void loadEventDataInputer(Ions &,
                            const Unit &,
                            const EventDataReader &) const; // for ions
  void loadEventDataInputer(Electrons &,
                            const Unit &,
                            const EventDataReader &) const; // for ions

 public:
  enum OptName {
    OnlyWithinMasterRegion, OnlyNotDead, AllRegion
  };
  void loadMomentumCalculator(Ion &ion, const OptName opt=OnlyWithinMasterRegion) const; // main momentum calculator
  void loadMomentumCalculator(Electron &elec, const OptName opt=OnlyWithinMasterRegion) const; // main momentum calculator
  void loadMomentumCalculator(Ions &ions, const OptName opt=OnlyWithinMasterRegion) const; // for ions
  void loadMomentumCalculator(Electrons &electrons, const OptName opt=OnlyWithinMasterRegion) const; // for electrons
};
}
#endif

