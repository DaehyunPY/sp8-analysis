#ifndef ANALYSIS_JSONREADER_H
#define ANALYSIS_JSONREADER_H

#include <string>
#include <sstream>
#include <fstream>
#include "../rapidjson/document.h"

namespace Analysis {
class JSONReader {
 private:
  rapidjson::Document document;
  const rapidjson::Document &getDocument() const;
  const rapidjson::Value &getValue(std::string str1) const;

 public:
  JSONReader(const std::string &filename);
  ~JSONReader();
  const bool getBool(const std::string &str) const;
  const int getInt(const std::string &str) const;
  const double getDouble(const std::string &str) const;
};
}

#endif
