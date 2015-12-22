#ifndef ANALYSIS_ANALYSISTOOLS_H
#define ANALYSIS_ANALYSISTOOLS_H

#include "EquipmentParameters.h"
#include "IonParameters.h"
#include "ElectronParameters.h"
#include "Ions.h"
#include "Electrons.h"

#ifdef FOR_COBOLDPC2002
#include "LMFReader.h"
#endif


namespace Analysis {
class AnalysisTools {
 private:
  int eventNumber;
  const EquipmentParameters equipmentParameters;
  const IonParameters ionParameters;
  const ElectronParameters electronParameters;
  struct XY { const double x; const double y; };
  AnalysisTools(const EquipmentParameters &,
                const IonParameters &,
                const ElectronParameters &);
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
  void loadEventDataInputer(Ion &, // main inputer for ion 
                            const double &,
                            const double &,
                            const double &,
                            const int &) const;
  void loadEventDataInputer(Electron &, // main inputer for electron 
                            const double &,
                            const double &,
                            const double &,
                            const int &) const;

 public:
  AnalysisTools(const Unit &, const JSONReader &);
  ~AnalysisTools();
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

#ifdef FOR_COBOLDPC2002
  public:
   void loadEventDataInputer(Ion &,
                             const Unit &,
                             const LMFReader &,
                             const int &) const;
   void loadEventDataInputer(Ions &,
                             const Unit &,
                             const LMFReader &) const;
   void loadEventDataInputer(Electron &,
                             const Unit &,
                             const LMFReader &,
                             const int &) const;
   void loadEventDataInputer(Electrons &,
                             const Unit &,
                             const LMFReader &) const;
#endif
};
}
#endif

