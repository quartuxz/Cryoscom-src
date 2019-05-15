#include <limits>
#include "MessagingComponent.h"
#include "cryoscom_defsAndUtils.h"
#include "globalMutexes.h"


MessageData::MessageData(){

}

MessageData::~MessageData(){

}

MessageData::MessageTypes MessageData::stringToMessageType(std::string str){
	return MessageTypes();

}

MessageData::MessageData(std::string mProps, std::string mData){
    auto props = tokenizeDecomposedDataData(mProps);
    messageType = stringToMessageType(props[0]);
}

decomposedData MessageData::serialize() {
	decomposedData tempDData;
	tempDData.type = "MessageData";
	tempDData.name = "";
	tempDData.addData(messageType);
	tempDData.addData(ma_serialize(senderID));
	tempDData.addData(ma_serialize(intendedReceiverID));
	for (size_t i = 0; i < messageContents.size(); i++)
	{
		tempDData.addChildrenObject(messageContents[i]);
	}
	return tempDData;
}
void MessageData::createFrom(const decomposedData& DData) {
	messageType = DData.data[0];
#if CRYOSCOM_DEBUG
	senderID = fast_atou(DData.data[1].c_str());
	intendedReceiverID = fast_atou(DData.data[2].c_str());
#else

	if (std::atoi(DData.data[1].c_str()) == -1) {
		senderID = std::numeric_limits<size_t>::max();
	}
	else {
		senderID = std::atoi(DData.data[1].c_str());
	}
	
	intendedReceiverID = std::atoi(DData.data[2].c_str());
#endif
	for (size_t i = 0; i < DData.childrenObjects.size(); i++)
	{
		messageContents.push_back(DData.childrenObjects[i]);
	}

}

MessageData MessageData::processPythonFunc(boost::python::object &pyFunc, size_t selfID, const std::map<std::string, size_t> &nameIDMap, std::string gameData)const{
	//return MessageData();
	//without_gil no_gil;
	with_gil gil;
    boost::python::list paramList;
    boost::python::dict IDDictionary;

    for(size_t i = 0; i < messageContents.size(); i++){
        paramList.append(messageContents[i].serialize());
    }

    for (auto const& it : nameIDMap)
    {
        IDDictionary[it.first] = it.second;
    }
	boost::python::object pyFuncRetVal;
	{

		pyFuncRetVal = pyFunc(false, boost::python::str(messageType), senderID, intendedReceiverID, paramList, IDDictionary, selfID, boost::python::str(gameData));
	
	}
	MessageData retVal;
    std::string tempMessageType = boost::python::extract<std::string>(pyFuncRetVal[0]);
    retVal.messageType = tempMessageType;
    retVal.senderID = selfID;
    retVal.intendedReceiverID = boost::python::extract<int>(pyFuncRetVal[1]);
    retVal.messageContents = decomposeString(boost::python::extract<std::string>(pyFuncRetVal[2]));
    return retVal;
}

void MessagingComponent::init(std::size_t ID){
    p_uniqueID = ID;

}

void MessagingComponent::processMessage(const MessageData *tempMessage, MessageBus *bus){
    if(tempMessage->messageType != "NULL"){
        pv_processMessage(*tempMessage, bus);
    }

}

MessagingComponent::MessagingComponent()
{
    //ctor
}

MessagingComponent::~MessagingComponent()
{
    //dtor
}
