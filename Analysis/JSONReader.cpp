#include "JSONReader.h"

Analysis::JSONReader::JSONReader(const std::string &f) {
  filename = f;
  std::stringstream stringStream;
  std::fstream fileStream;
  fileStream.open(f, std::fstream::in);
  if(fileStream.is_open()) {
    setFlagMembers().loadIsOpenFlager();
  } else {
    assert(false);
  }
  stringStream << fileStream.rdbuf();
  document.Parse(stringStream.str().c_str());
  if(document.HasParseError()) {
    assert(false);
  } else {
    setFlagMembers().loadHasNoParseErrorFlager();
  }
  fileStream.close();
  return;
}
Analysis::JSONReader::~JSONReader() { return; }
const rapidjson::Value &Analysis::JSONReader::getValue(std::string str1)
const {
  std::string str0;
  std::size_t found;
  const rapidjson::Value *value;
  value = &getDocument();
  found = str1.find(".");
  while (found != std::string::npos) {
    str0 = str1.substr(0, found);
    str1 = str1.substr(found + 1);
    assert(value->HasMember(str0.c_str()));
    assert((*value)[str0.c_str()].IsObject());
    value = &((*value)[str0.c_str()]);
    found = str1.find(".");
  }
  assert(value->HasMember(str1.c_str()));
  return (*value)[str1.c_str()];
}
const int Analysis::JSONReader::getIntAt(const std::string &str) const {
  assert(getValue(str).IsNumber());
  assert(getValue(str).IsInt());
  return getValue(str).GetInt();
}
const double Analysis::JSONReader::getDoubleAt(const std::string &str) const {
  assert(getValue(str).IsNumber());
  return getValue(str).GetDouble();
}
const rapidjson::Document &Analysis::JSONReader::getDocument() const {
  return document;
}
const bool Analysis::JSONReader::getBoolAt(const std::string &str) const {
  assert(getValue(str).IsBool());
  return getValue(str).GetBool();
}
const std::string &Analysis::JSONReader::getFilename() const {
  return filename;
}
const Analysis::JSONFlag &Analysis::JSONReader::getFlag() const {
  return flag;
}
Analysis::JSONFlag &Analysis::JSONReader::setFlagMembers() {
  return flag;
}
const std::string Analysis::JSONReader::getStringAt(const std::string &str) const {
  assert(getValue(str).IsString());
  return getValue(str).GetString();
}
const bool Analysis::JSONReader::hasMember(std::string str1) const {
  std::string str0;
  std::size_t found;
  const rapidjson::Value *value;
  value = &getDocument();
  found = str1.find(".");
  while (found != std::string::npos) {
    str0 = str1.substr(0, found);
    str1 = str1.substr(found + 1);
    if(!value->HasMember(str0.c_str())) { return false; }
    if(!(*value)[str0.c_str()].IsObject()) { return false; }
    value = &((*value)[str0.c_str()]);
    found = str1.find(".");
  }
  return value->HasMember(str1.c_str());
}
