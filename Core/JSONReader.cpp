#include "JSONReader.h"

Analysis::JSONReader::JSONReader(const std::string f) {
  filename = f;
  std::stringstream stringStream;
  std::fstream fileStream;

//  Open json file.
  std::cout << "reading a JSON file... ";
  fileStream.open(f, std::fstream::in);
  if(fileStream.is_open()) {
    setFlagMembers().fileIsOpen();
  } else {
    setFlagMembers().fileIsNotExist();
    std::cout << "fail" << std::endl;
    std::cout << "The JSON file does not exist!" << std::endl;
    assert(false);
  }

//  Save the json file data.
  stringStream << fileStream.rdbuf();
  document.Parse<parseFlags>(stringStream.str().c_str());
  fileStream.close();
  setFlagMembers().fileIsClosedAndDataIsSaved();

//  Check the parse.
  if(document.HasParseError()) {
    setFlagMembers().hasParseError();
    std::cout << "fail" << std::endl;
    std::cout << "The JSON file has parse error!" << std::endl;
    assert(false);
  } else {
    setFlagMembers().hasNoParseError();
  }
  std::cout << "okay" << std::endl;
  return;
}
Analysis::JSONReader::~JSONReader() { }
const rapidjson::Value &Analysis::JSONReader::getValue(std::string str1) const {
  const auto tmp = str1;
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
  if (!(value->HasMember(str1.c_str()))) {
    std::cout << "Member " << tmp << " does not exist!" << std::endl;
    assert(false);
  }
  return (*value)[str1.c_str()];
}
const int Analysis::JSONReader::getIntAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = &getValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsNumber());
	assert(pV->IsInt());
	return pV->GetInt();
}
const double Analysis::JSONReader::getDoubleAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = &getValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsNumber());
	return pV->GetDouble();
}
const rapidjson::Document &Analysis::JSONReader::getDocument() const {
  return document;
}
const bool Analysis::JSONReader::getBoolAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = &getValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsBool());
	return pV->GetBool();
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
const std::string Analysis::JSONReader::getStringAt(const std::string str, const int i) const {
	const rapidjson::Value *pV = &getValue(str);
	if(i != -1) { 
		assert(i >= 0);
		assert(pV->IsArray());
		pV = &((*pV)[i]);
	}
	assert(pV->IsString());
	return pV->GetString();
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
const int Analysis::JSONReader::getListSizeAt(const std::string str) const {
	const rapidjson::Value *pV = &getValue(str);
	if(pV->IsArray()) {
		return pV->Size();
	} else {
		return -1;
	}
}
bool Analysis::JSONReader::getBoolAtIfItIs(const std::string str, const bool def) const {
  if (hasMember(str)) return getBoolAt(str);
  else return def;
}
