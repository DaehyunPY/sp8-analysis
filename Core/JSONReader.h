#ifndef ANALYSIS_JSONREADER_H
#define ANALYSIS_JSONREADER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "../rapidjson/document.h"
#include "JSONFlag.h"

namespace Analysis {
class JSONReader {
 private:
  std::string filename;
  rapidjson::Document document;
  const rapidjson::Document &getDocument() const;
  const rapidjson::Value &getValue(std::string) const;
  JSONFlag flag;
    static const unsigned parseFlags =
            rapidjson::kParseCommentsFlag
            +rapidjson::kParseTrailingCommasFlag
            +rapidjson::kParseNanAndInfFlag;

 public:
  JSONReader(const std::string f);
  ~JSONReader();
  JSONFlag &setFlagMembers();
  const std::string &getFilename() const;
  const JSONFlag &getFlag() const;
  const bool getBoolAt(const std::string str, const int i=-1) const;
  bool getBoolAtIfItIs(const std::string str, const bool def=false) const;
  const int getIntAt(const std::string str, const int i=-1) const;
  const double getDoubleAt(const std::string str, const int i=-1) const;
  const std::string getStringAt(const std::string str, const int i=-1) const;
  const int getListSizeAt(const std::string str) const;
  const bool hasMember(std::string str) const;
};
}

#endif
