#include "Serializable.h"
#include <sstream>
#include <iostream>
#include <cstdlib>



Serializable::Serializable()
{
    //ctor
}



std::pair<std::vector<std::string>, tokenizerInfo> tokenizeFromTo(std::string data, unsigned int startingIndex,std::string tokenToToken, std::string from, std::string to){
	return std::pair<std::vector<std::string>, tokenizerInfo>();
}


void decomposedData::m_parseData(std::string passedData){
    data.clear();
    type = "";
    name = "";
    std::vector<std::string> tempData = tokenizeDecomposedDataData(passedData);
    type = tempData[0];
    name = tempData[1];
    tempData.erase(tempData.begin(), tempData.begin() + 2);
    data = tempData;
}



decomposedData decomposedData::createFrom(std::string passedData){
    std::vector<std::string> allData = processBlock(passedData);
    m_parseData(allData[0]);
    childrenObjects.clear();
    for(size_t i = 1; i < allData.size(); i++){
        childrenObjects.push_back(decomposedData());
        childrenObjects.back().createFrom(allData[i]);
    }
    return *this;
}

decomposedData* decomposedData::getChildByName(std::string childName) {
	for (size_t i = 0; i < childrenObjects.size(); i++)
	{
		if (childrenObjects[i].name == childName) {
			return &childrenObjects[i];
		}
		else {
			decomposedData* tempRetVal = childrenObjects[i].getChildByName(childName);
			if (tempRetVal != nullptr) {
				return tempRetVal;
			}
		}
	}
	return nullptr;
}

std::string decomposedData::serialize(unsigned int indentation)const{
    std::stringstream ss;
    ss << "{" << type << "," << name << ",";
    for(int i = 0; i < data.size(); i++){
        ss << data[i] << ",";
    }
    for(int i = 0; i < childrenObjects.size(); i++){
        ss << childrenObjects[i].serialize(++indentation);
    }
    ss << "}";
    return ss.str();
}

std::string decomposedData::getMushedData()const{
    std::stringstream ss;
    for(int i = 0; i < data.size(); i++){
        ss << data[i] << ",";
    }
    return ss.str();
}

decomposedData &decomposedData::setType(std::string newType){
    type = newType;
    return *this;
}

decomposedData &decomposedData::setName(std::string newName){
    name = newName;
    return *this;
}

decomposedData &decomposedData::addData(std::string addedData){
    data.push_back(addedData);
    return *this;
}

decomposedData &decomposedData::addChildrenObject(const decomposedData &childrenObject){
    childrenObjects.push_back(childrenObject);
    return *this;
}

std::vector<decomposedData> decomposeString(std::string data){
    std::vector<decomposedData> retVal;

    data.erase(std::remove(data.begin(), data.end(), '\n'),data.end());

    std::vector<std::string> tokens;

    std::string delimiter = ";";

    size_t pos = 0;
    pos = data.find(delimiter);
    while (pos != std::string::npos) {
        tokens.push_back(data.substr(0, pos));
        data.erase(0, pos + delimiter.length());
        pos = data.find(delimiter);
    }


    for(size_t i = 0; i < tokens.size(); i++){
        retVal.push_back(decomposedData());
        retVal[i].createFrom(tokens[i]);
    }
    return retVal;
}

std::string composeString(std::vector<decomposedData> decomString){
    std::stringstream ss;
    for(size_t i = 0; i < decomString.size(); i++){
        ss << decomString[i].serialize();
        ss << ";";
    }
    return ss.str();
}

template<class T>
std::string ma_serialize(T obj){
    return obj.serialize();
}

template<class T>
T ma_deserialize(std::string data){
    T tempObj;
    tempObj.createFrom(data);
    return tempObj;
}

template<>
std::string ma_serialize<float>(float val){
    return std::to_string(val);
}


float ma_deserialize_float(std::string data){
    return std::atof(data.c_str());
}


template<>
std::string ma_serialize<int>(int val){
	return std::to_string(val);
}


int ma_deserialize_int(std::string data){
    return std::atoi(data.c_str());
}

template<>
std::string ma_serialize(unsigned int val)
{
	return std::to_string(val);
}

unsigned int ma_deserialize_uint(std::string data) {
	return std::atoi(data.c_str());
}

Serializable::~Serializable()
{
    //dtor
}
