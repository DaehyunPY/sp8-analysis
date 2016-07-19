#ifndef ANALYSIS_JSONREADER_H
#define ANALYSIS_JSONREADER_H

#include <sstream>
#include <fstream>
#include <string>
#ifdef _WIN32
#include "rapidjson.junc/document.h"
#else
#include "rapidjson/document.h"
#endif 
#include "JSONFlag.h"

namespace Analysis {
class JSONReader {
 private:
  std::string filename;
  rapidjson::Document document;
  const rapidjson::Document &getDocument() const;
  const rapidjson::Value &getValue(std::string) const;
  JSONFlag flag;

 public:
  JSONReader(const std::string &f = "Parameters.json");
  ~JSONReader();
  JSONFlag &setFlagMembers();
  const std::string &getFilename() const;
  const JSONFlag &getFlag() const;
  const bool getBoolAt(const std::string &) const;
  const int getIntAt(const std::string &) const;
  const double getDoubleAt(const std::string &) const;
  const std::string getStringAt(const std::string &) const;
  const bool hasMember(std::string) const;
};
}

#endif
