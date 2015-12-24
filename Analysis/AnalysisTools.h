#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#include "EquipmentParameters.h"
#include "IonParameters.h"
#include "ElectronParameters.h"
#include "Ions.h"
#include "Electrons.h"
#include "EventDataReader.h"

namespace Analysis {
struct XY { const double x; const double y; };

class AnalysisTools {
 private:
  const EquipmentParameters equipmentParameters;
  const IonParameters ionParameters;
  const ElectronParameters electronParameters;
  const std::string ID;
  int eventNumber;
  AnalysisTools(const EquipmentParameters &,
                const IonParameters &,
                const ElectronParameters &,
                const std::string &ID = "0000"); // main initializer
  void resetCounter();
  const EquipmentParameters &getEquipmentParameters() const;
  const IonParameters &getIonParameters() const;
  const ElectronParameters &getElectronParameters() const;
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

 public:
  AnalysisTools(const Unit &, const JSONReader &);
  ~AnalysisTools();
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
  const double calculateFrequencyOfCycle
      (const double &m, const double &q, const double &B) const;
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
                            const int &) const;
  void loadEventDataInputer(Electron &,
                            const Unit &,
                            const double &,
                            const double &,
                            const double &,
                            const int &) const;
  void loadMomentumCalculator(Ion &) const;
  void loadMomentumCalculator(Electron &) const;
  void loadMomentumCalculator(Ions &) const;
  void loadMomentumCalculator(Electrons &) const;

  public:
   void loadEventDataInputer(Ion &,
                             const Unit &,
                             const EventDataReader &,
                             const int &) const;
   void loadEventDataInputer(Ions &,
                             const Unit &,
                             const EventDataReader &) const;
   void loadEventDataInputer(Electron &,
                             const Unit &,
                             const EventDataReader &,
                             const int &) const;
   void loadEventDataInputer(Electrons &,
                             const Unit &,
                             const EventDataReader &) const;
};
}
#endif

