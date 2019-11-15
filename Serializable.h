#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <sstream>
#include <string>
#include <type_traits>
#include <SFML/Graphics.hpp>
#include<vector>
#include <boost/python.hpp>
#include "serializationUtils.h"





//use this:
//input.erase(std::remove(input.begin(), input.end(), '\n'),input.end());
//for parsing user input formatted with \n (new line) characters

struct tokenizerInfo{
    std::string lastDelimiter;
    unsigned int lastPos;
    bool isEmpty;
};

std::pair<std::vector<std::string>, tokenizerInfo> tokenizeFromTo(std::string, unsigned int,std::string tokenToToken = ",", std::string from = "{", std::string to = "}");



struct decomposedData{
    public:
        std::string type;
        std::string name;
        std::vector<std::string> data;
        std::vector<decomposedData> childrenObjects;
    private:
        void m_parseData(std::string);
    public:
        decomposedData createFrom(std::string);
        std::string serialize(unsigned int indentation = 0)const;
        std::string getMushedData()const;

		//returns null if not found
		//tries to find the specified name in the childrenObjects, only 1 layer deep.
		decomposedData *getChildByName(std::string);

		//returns null if not found
		//tries to find the specified name in the children tree, recursively, returns the first occurence found if done so.
		decomposedData *getChildByNameRecursive(std::string);
        //for constructing a decomposedData object inline
        decomposedData &setType(std::string);
        decomposedData &setName(std::string);
        decomposedData &addData(std::string);
        decomposedData &addChildrenObject(const decomposedData&);
};


class Serializable
{
    public:
        Serializable();
        virtual void createFrom(const decomposedData&) = 0;
        virtual decomposedData serialize() = 0;
        virtual ~Serializable();
};


//creates a vector of data objects
//used to receive and send(respectively) collections of objects in a single string
//decomposedData objects can be deserialized in Serializable children
std::vector<decomposedData> decomposeString(std::string);
std::string composeString(std::vector<decomposedData>);




template<class T>
std::string ma_serialize(T);

template<class T>
T ma_deserialize(std::string);

template<>
std::string ma_serialize<float>(float);

template<>
float ma_deserialize(std::string);

float ma_deserialize_float(std::string);

template<>
std::string ma_serialize<int>(int);

template<>
int ma_deserialize(std::string);

int ma_deserialize_int(std::string);

template<>
std::string ma_serialize<unsigned int>(unsigned int);

template<>
unsigned int ma_deserialize(std::string);

unsigned int ma_deserialize_uint(std::string);




#endif // SERIALIZABLE_H
