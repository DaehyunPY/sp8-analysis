//
// Created by Daehyun You on 12/1/15.
//

#ifndef ANALYSIS_EVENTDATAREADER_H
#define ANALYSIS_EVENTDATAREADER_H

#include <assert.h>
#include <string>
#include <vector>

namespace Analysis {
class EventDataReader {
 private:
  const int maxIons, maxElecs;
  const int intDum=-1;
  const double doubleDum=-1e9;
  int numIons, numElecs;
  std::vector<double> eventData;
  std::vector<int> flagData;
 public:
  EventDataReader(const int maxNumOfIons, const int maxNumOfElecs);
  ~EventDataReader();
  void reset();

 public:
  enum TreeName{
    IonNum, IonT, IonX, IonY, IonFlag,
    ElecNum, ElecT, ElecX, ElecY, ElecFlag
  };
  double &setEventDataAt(const TreeName name, const int i);
  int &setFlagDataAt(const TreeName name, const int i);
  int &setNumObjs(const TreeName name);
  double getEventDataAt(const TreeName name, const int i) const;
  int getFlagDataAt(const TreeName name, const int i) const;
  int getNumObjs(const TreeName name) const;
  static std::string getTreeName(const TreeName name, const int i=-1);

 private:
  int getAdressAt(const TreeName name, const int i) const;
  bool returnDum(const TreeName name, const int i) const;
  bool isAnyIonTree(const TreeName name) const;
  bool isAnyElecTree(const TreeName name) const;
  bool isAnyNumTree(const TreeName name) const;
  bool isAnyEventTree(const TreeName name) const;
  bool isAnyTTree(const TreeName name) const;
  bool isAnyXTree(const TreeName name) const;
  bool isAnyYTree(const TreeName name) const;
  bool isAnyFlagTree(const TreeName name) const;
};
}

#endif //ANALYSIS_LMFREADER_H
