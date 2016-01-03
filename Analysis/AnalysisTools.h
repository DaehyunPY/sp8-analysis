#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#define LIMITATION_NUMBER_OF_LOOP 1000

#include "EquipmentParameters.h"
#include "IonParameters.h"
#include "ElectronParameters.h"
#include "Ions.h"
#include "Electrons.h"
#include "EventDataReader.h"
namespace Analysis {
struct XY {
  XY(const double &x, const double &y)
      : x(x), y(y) {
  }
  const double x;
  const double y;
};
class AnalysisTools {
  const EquipmentParameters equipmentParameters;
  const IonParameters ionParameters;
  const ElectronParameters electronParameters;
  const std::string ID;
  int eventNumber;
  AnalysisTools(const EquipmentParameters &,
                const IonParameters &,
                const ElectronParameters &,
                const std::string &); // main initializer
  void resetCounter();
  const XY calculateRotation(const XY &, const double &) const;
  const double calculateDiffTOF(const Ion &, const double &) const;
  const double calculateDiffTOF(const Electron &, const double &) const;
  const XY calculateMomentumXY(const Object &) const;
  const double calculateMomentumZ(const Ion &, bool &info) const;
  const double calculateMomentumZ(const Electron &, bool &info) const;
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
  AnalysisTools(const Unit &, const JSONReader &);
  ~AnalysisTools();
  const EquipmentParameters &getEquipmentParameters() const;
  const IonParameters &getIonParameters() const;
  const ElectronParameters &getElectronParameters() const;
  const std::string &getID() const;
  const int &getEventNumber() const;
  void loadEventCounter();
  const double calculateTOF(const Ion &,
                            const double &) const;
  const double calculateTOF(const Unit &,
                            const Ion &,
                            const double &) const;
  const double calculateTOF(const Electron &,
                            const double &) const;
  const double calculateTOF(const Unit &,
                            const Electron &,
                            const double &) const;
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
  void loadMomentumCalculator(Ion &) const; // main momentum calculator
  void loadMomentumCalculator(Electron &) const; // main momentum calculator
  void loadMomentumCalculator(Ions &) const; // for ions
  void loadMomentumCalculator(Electrons &) const; // for electrons
};
}
#endif

