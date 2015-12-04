#include "JSONReader.h"

Analysis::JSONReader::JSONReader(const std::string &filename) {
  std::stringstream stringStream;
  std::fstream fileStream;
  fileStream.open(filename, std::fstream::in);
  assert(fileStream.is_open());
  stringStream << fileStream.rdbuf();
  this->document.Parse(stringStream.str().c_str());
  assert(!this->document.HasParseError());
  fileStream.close();
  return;
}
Analysis::JSONReader::~JSONReader() { return; }
const rapidjson::Value &Analysis::JSONReader::getValue(std::string str1)
const {
  std::string str0;
  std::size_t found;
  const rapidjson::Value *value;
  value = &(this->getDocument());
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
const int Analysis::JSONReader::getInt(const std::string &str) const {
  assert(this->getValue(str).IsNumber());
  assert(this->getValue(str).IsInt());
  return this->getValue(str).GetInt();
}
const double Analysis::JSONReader::getDouble(const std::string &str) const {
  assert(this->getValue(str).IsNumber());
  return this->getValue(str).GetDouble();
}
const rapidjson::Document &Analysis::JSONReader::getDocument() const {
  return this->document;
}
const bool Analysis::JSONReader::getBool(const std::string &str) const {
  assert(this->getValue(str).IsBool());
  return this->getValue(str).GetBool();
}
