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
  static const unsigned parseFlags =
      rapidjson::kParseCommentsFlag+rapidjson::kParseTrailingCommasFlag+rapidjson::kParseNanAndInfFlag;
  std::vector<rapidjson::Document *> pDocs;
  void ReadFromFile(const std::string filename);
  void ReadFromStr(const std::string str);
 public:
  enum ReadingType {fromFile, fromStr};
  JSONReader(const bool v=false);
  ~JSONReader();
  void appendDoc(const ReadingType type, const std::string str);

 private:
  const rapidjson::Value *getOptValue(const std::string str1, const rapidjson::Value *v, const std::string str0 = "") const;
  const rapidjson::Value *getOptValue(const std::string str1) const;
  bool hasMember(const std::string str, const rapidjson::Value *&pV) const;
 public:
  bool hasMember(const std::string str) const;

 private:
  template <typename T>
  bool is(const std::string str, const rapidjson::Value *&pV) const {
    const bool hasMem = hasMember(str, pV);
    if (!hasMem) return false;
    return pV->Is<T>();
  }
 public:
  template <typename T>
  bool is(const std::string str) const {
    const rapidjson::Value *pV;
    return is<T>(str, pV);
  }
  template <typename T>
  const T *getOpt(const std::string str) const {
    const rapidjson::Value *pV;
    const bool isT = is<T>(str, pV);
    if (!isT) return nullptr;
    return new T(pV->Get<T>());
  }
  template <typename T>
  T get(const std::string str) const {
    auto pT = getOpt<T>(str);
    if (pT == nullptr) throw std::invalid_argument("Invalid member!");
    return *pT;
  }
  template <typename T>
  const std::map<std::string, T> *getOptMap(std::string str) const {
    const rapidjson::Value *pV;
    const bool hasMem = hasMember(str, pV);
    if (!hasMem) return nullptr;
    if (!pV->IsObject()) return nullptr;
    auto *pMap = new std::map<std::string, T>;
    for (auto &v: pV->GetObject()) (*pMap)[v.name.GetString()] = v.value.Get<T>();
    return pMap;
  };
  template <typename T>
  std::map<std::string, T> getMap(std::string str) const {
    auto pMap = getOptMap<T>(str);
    if (pMap == nullptr) throw std::invalid_argument("Invalid member!");
    return *pMap;
  }
  template <typename T>
  const std::vector<T> *getOptArr(std::string str) const {
    const rapidjson::Value *pV;
    const bool hasMem = hasMember(str, pV);
    if (!hasMem) return nullptr;
    if (!pV->IsArray()) return nullptr;
    auto *pArr = new std::vector<T>;
    for (auto &v: pV->GetArray()) pArr->push_back(v.Get<T>());
    return pArr;
  }
  template <typename T>
  std::vector<T> getArr(std::string str) const {
    auto pArr = getOptArr<T>(str);
    if (pArr == nullptr) throw std::invalid_argument("Invalid member!");
    return *pArr;
  }

  // delete this block
 public:
  const bool getBoolAt(const std::string str, const int i=-1) const;
  bool getBoolAtIfItIs(const std::string str, const bool def=false) const;
  const int getIntAt(const std::string str, const int i=-1) const;
  const double getDoubleAt(const std::string str, const int i=-1) const;
  const std::string getStringAt(const std::string str, const int i=-1) const;
  const int getListSizeAt(const std::string str) const;
};
}

#endif
