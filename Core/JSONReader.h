#ifndef ANALYSIS_JSONREADER_H
#define ANALYSIS_JSONREADER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include "rapidjson/document.h"

namespace Analysis {
class JSONReader {
 private:
  const bool vvv;
  std::vector<rapidjson::Document *> pDocs;
  static const unsigned parseFlags =
      rapidjson::kParseCommentsFlag+rapidjson::kParseTrailingCommasFlag+rapidjson::kParseNanAndInfFlag;
  const rapidjson::Value *getOptValue(const std::string str1) const;
  const rapidjson::Value *_getOptValue(const std::string str1, const rapidjson::Value *v, const std::string str0 = "") const;

 public:
  const bool getBoolAt(const std::string str, const int i=-1) const;
  bool getBoolAtIfItIs(const std::string str, const bool def=false) const;
  const int getIntAt(const std::string str, const int i=-1) const;
  const double getDoubleAt(const std::string str, const int i=-1) const;
  const std::string getStringAt(const std::string str, const int i=-1) const;
  const int getListSizeAt(const std::string str) const;
  bool hasMember(const std::string str) const;
  template <typename T>
      const std::map<std::string, T> getMap(std::string str) const;

 public:
  enum ReadingType {fromFile, fromStr};
  JSONReader(const bool v=false);
  ~JSONReader();
  void appendDoc(const ReadingType type, const std::string str);
 private:
  void ReadFromFile(const std::string filename);
  void ReadFromStr(const std::string str);

 public:
  template <typename T>
  const T *getOpt(const std::string str) const {
    auto pV = getOptValue(str);
    if (pV == nullptr) return nullptr;
    if(!pV->Is<T>()) {
      std::cout << "Check the type of member " << str << std::endl;
      return nullptr;
    }
    return new T(pV->Get<T>());
  }
};
}

#endif
