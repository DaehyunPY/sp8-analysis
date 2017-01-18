//
// Created by daehyun on 1/17/17.
//

#ifndef ANALYSIS_SORTWRAPPER_H
#define ANALYSIS_SORTWRAPPER_H

#include <cstdlib>
#include <cmath>
#include <termios.h>
#include <map>
#include "resort64c.h"
#include "LMF_IO.h"
#include "../Core/JSONReader.h"

#define NUM_IONS 200
#define NUM_CHANNELS 80

void readline_from_config_file(FILE *ffile, char *text, __int32 max_len);
int read_int(FILE *ffile);
double read_double(FILE *ffile);
bool read_calibration_tables(const char *filename, sort_class *sorter);
bool create_calibration_tables(const char *filename, sort_class *sorter);

namespace Analysis {
struct LMFWrapper {
  LMF_IO *pLMF;
  std::vector<std::string> filenames;
  const double TDCRes = 0.025; // 25ps tdc bin size
  double timestamp;
  int TDC[NUM_CHANNELS][NUM_IONS];
  double TDCns[NUM_CHANNELS][NUM_IONS];
  unsigned int count[NUM_CHANNELS];
  bool readConfig(const JSONReader &reader);
  bool readFile(const int i);
  bool readNextEvent();
  void cleanup();
};

class SortWrapper {
 public:
  enum SortCmd {
    // -1 = detector does not exist
    //  0 = only convert to new file format
    //  1 = sort and write new file
    //  2 = calibrate fv, fw, w_offset
    //  3 = create calibration table files
    kNoDetector = -1,
    kOnlyConvert = 0,
    kSort = 1,
    kCalib = 2,
    kGenCalibTab = 3
  };

 private:
  LMFWrapper *pLMFSource;
  sort_class *pSorter;
  SortCmd cmd = kNoDetector;
  std::map<std::string, int> chMap;
  std::map<std::string, double> factors;
  std::string calibTabFilename;
  int numHits;
 public:
  SortWrapper(LMFWrapper *p);
  ~SortWrapper();
  bool readConfig(const JSONReader &reader, const std::string prefix);
  bool readCalibTab();
  bool init();
  bool convertTDC();
  bool sort();
  bool calibFactors() const;
  bool genClibTab() const;

 public:
  SortCmd getCmd() const;
  bool isCmd(SortCmd c) const;
  bool isNull() const;
  bool isFull() const;
  const sort_class &getSorter() const;
  int getNumHits() const;
  hit_class **getOutputArr() const;

 public:
  const double *getMCP() const;
  const double *getXDev() const;
  const double *getYDev() const;
  const double *getWeightDev() const;
  const double *getXRaw() const;
  const double *getYRaw() const;
  const double *getUTimesum() const;
  const double *getUTimediff() const;
  const double *getVTimesum() const;
  const double *getVTimediff() const;
  const double *getWTimesum() const;
  const double *getWTimediff() const;
};
}

#endif //ANALYSIS_SORTWRAPPER_H
