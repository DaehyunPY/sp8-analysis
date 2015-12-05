#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#include "EquipmentParameters.h"
#include "IonParameters.h"
#include "ElectronParameters.h"
#include "Ions.h"
#include "Electrons.h"

#ifndef NOTFOR_COBOLDPC2002
#include "LMFReader.h"
#endif


namespace Analysis {
struct XY { double x; double y; };
class AnalysisTools {
 private:
  int eventNumber;
  const EquipmentParameters equipmentParameters;
  const IonParameters ionParameters;
  const ElectronParameters electronParameters;
  AnalysisTools(const EquipmentParameters &equip,
                const IonParameters &ion,
                const ElectronParameters &elec);
  void resetCounter();
  const EquipmentParameters &getEquipmentParameters() const;
  const IonParameters &getIonParameters() const;
  const ElectronParameters &getElectronParameters() const;
  const double calculateDiffTOF(const Ion &ion, const double &p0) const;
  const double calculateDiffTOF(const Electron &elec, const double &p0) const;
  const XY calculateMomentumXY(const Object &obj) const;
  const double calculateMomentumZ(const Ion &ion) const;
  const double calculateMomentumZ(const Electron &elec) const;
  void loadEventDataInputer(Ion &ion,
                            const double &x1,
                            const double &y1,
                            const double &t1,
                            const int &f1) const;
  void loadEventDataInputer(Electron &elec,
                            const double &x1,
                            const double &y1,
                            const double &t1,
                            const int &f1) const;

 public:
  AnalysisTools(const Unit &unit, const JSONReader &reader);
  ~AnalysisTools();
  const int &getEventNumber() const;
  void loadEventCounter();
  const double calculateTOF(const Ion &, const double &) const;
  const double calculateTOF(const Unit &, const Ion &, const double &) const;
  const double calculateTOF(const Electron &, const double &) const;
  const double calculateTOF(const Unit &,
                            const Electron &,
                            const double &) const;
  const double calculatePeriodOfCycle(const Object &) const;
  const double calculatePeriodOfCycle(const Unit &,
                                      const Object &) const;
  void loadEventDataInputer(Ion &ion,
                            Unit &unit,
                            const double &x,
                            const double &y,
                            const double &t,
                            const int &f) const;
  void loadEventDataInputer(Electron &electron,
                            Unit &unit,
                            const double &x,
                            const double &y,
                            const double &t,
                            const int &f) const;
  void loadMomentumCalculator(Ion &ion) const;
  void loadMomentumCalculator(Electron &elec) const;
  void loadMomentumCalculator(Ions &ions) const;
  void loadMomentumCalculator(Electrons &elecs) const;

#ifndef NOTFOR_COBOLDPC2002
  public:
   void loadEventDataInputer(Ion &ion,
                             Unit &unit,
                             LMFReader &reader,
                             const int &iHit) const;
   void loadEventDataInputer(Ions &ions,
                             Unit &unit,
                             LMFReader &reader) const;
   void loadEventDataInputer(Electron &electron,
                             Unit &unit, LMFReader
                             &reader,
                             const int &iHit) const;
   void loadEventDataInputer(Electrons &electrons,
                             Unit &unit,
                             LMFReader &reader) const;
#endif
};
}
#endif

