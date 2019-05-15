#include "MessageBus.h"
#include <limits>
#include "Serializable.h"
#include "cryoscom_defsAndUtils.h"
#include "globalMutexes.h"

size_t MessageBus::m_IDCounter = 0;



boost::python::object makePythonFunction(std::string scriptName, std::string functionName) {
	try
	{
		boost::python::object retObj = boost::python::import(boost::python::str(scriptName)).attr(boost::python::str(functionName));
		return retObj;
	}
	catch (const boost::python::error_already_set&)
	{
		PyErr_Print();
		while (true) {
			PyErr_Print();
		}
		return boost::python::object();
	}
}

void GameScript::pv_processMessage(const MessageData& tempMessage, MessageBus* bus) {

	bus->addMessage(new MessageData(tempMessage.processPythonFunc(m_pythonFunc, p_uniqueID, bus->getEntryIDs(), m_cachedSerializedGameData)));
}

void GameScript::createFromFile(std::string scriptName)
{
	m_pythonFunc = makePythonFunction(scriptName, "gameScript");
	m_gameData.type = "gameData";
	m_gameData.name = "scriptGameData";
	m_cachedSerializedGameData = m_gameData.serialize();
	//boost::python::object memoryBlockNameRetriever = makePythonFunction("shared_data", "generateMemoryBlock")(1000);
	//boost::python::object retObj = makePythonFunction("shared_data", "generateMemoryBlock")(10);
	//m_sharedMemoryBlockName = boost::python::extract<std::string>(retObj[0]);
}

decomposedData GameScript::getGameData()const {
	return m_gameData;
}

void GameScript::setGameData(const decomposedData &DData) {
	m_gameData = DData;
	m_cachedSerializedGameData = m_gameData.serialize();

}

MessageBus::MessageBus(){

}

void MessageBus::addEntry(std::string MCName, size_t uniqueID){

    m_entryIDs[MCName] = uniqueID;
    m_entryNames[uniqueID] = MCName;
}

size_t MessageBus::getEntryID(std::string MCName){
    return m_entryIDs[MCName];

}

std::string MessageBus::getEntryName(size_t uniqueID){
    return m_entryNames[uniqueID];
}


MessageBus &MessageBus::operator=(const MessageBus &rhs) {
	if (this == &rhs)
		return *this;


	m_maxChainLength = rhs.m_maxChainLength;
	m_chainLength = rhs.m_chainLength;
	m_lastMessage = rhs.m_lastMessage;
	m_threadPool = rhs.m_threadPool;
	m_addedTime = rhs.m_addedTime;
	m_updateInterval = rhs.m_updateInterval;
	m_entryIDs = rhs.m_entryIDs;
	m_entryNames = rhs.m_entryNames;
	m_dynamicMessagingComponents = rhs.m_dynamicMessagingComponents;
	m_echoMessages = rhs.m_echoMessages;
	m_messageQueue = rhs.m_messageQueue;
	m_endOfFrameGarbageCollectionQueue = rhs.m_endOfFrameGarbageCollectionQueue;
	m_messagingComponents = rhs.m_messagingComponents;
	return *this;
}

void MessageBus::onDestroy()
{
	for (auto const& x : m_dynamicMessagingComponents)
	{
		delete x;
	}

	m_joinThreads();

	while (!m_endOfFrameGarbageCollectionQueue.empty()) {
		delete m_endOfFrameGarbageCollectionQueue.front();
		m_endOfFrameGarbageCollectionQueue.pop();
	}

	while (!m_messageQueue.empty()) {
		delete m_messageQueue.front();
		m_messageQueue.pop();
	}
}

const std::map<std::string, size_t> &MessageBus::getEntryIDs()const{
    return m_entryIDs;
}

size_t MessageBus::addMessagingComponent(MessagingComponent *messagingComponent, bool dynamicObj){
    messagingComponent->init(m_IDCounter);
    m_messagingComponents[m_IDCounter] = messagingComponent;
    return m_IDCounter++;
	if (dynamicObj) {
		m_dynamicMessagingComponents.push_back(messagingComponent);
	}
}

MessagingComponent* MessageBus::getMessagingComponent(size_t ID) {
	return m_messagingComponents[ID];
}

void MessageBus::setUpdateInterval(float interval) {
	m_updateInterval = interval;
}

void MessageBus::startFrame(float delta) {
	//m_allMutex.lock();
	m_addedTime += delta;
	std::list<MessageData*>::iterator i = m_echoMessages.begin();
	while (i != m_echoMessages.end()) {
		(*i)->currentEchoAddedTime += delta;
		if ((*i)->currentEchoAddedTime >= (*i)->echoDelay) {
			(*i)->messageType = "messages";
			(*i)->messageContents.erase((*i)->messageContents.begin());
			addMessage((*i));
			m_echoMessages.erase(i++);
		}
		else {
			++i;
		}
	}
#if MULTITHREADED_SCRIPTING_AND_MESSAGING
	notify();
#endif
	//m_allMutex.unlock();
	

}
bool MessageBus::canMessage() {
	if (m_addedTime >= m_updateInterval) {
		return true;
	}
	return false;
}

void MessageBus::m_recursiveJoinAndProcessAll()
{
	m_joinThreads();
	if (!m_messageQueue.empty()) {
		notify();
		m_recursiveJoinAndProcessAll();
	}
}

void MessageBus::m_joinThreads(){
	while (!m_threadPool.empty()) {
		m_threadPool.top()->join();
		delete m_threadPool.top();
		m_threadPool.pop();
	}
	
}
void MessageBus::endFrame() {

	//m_allMutex.lock();

	if (canMessage()) {
		m_addedTime = 0;
	}

#if  MULTITHREADED_SCRIPTING_AND_MESSAGING
	m_joinThreads();

#else
	notify();
#endif

	while (!m_endOfFrameGarbageCollectionQueue.empty()) {
		delete m_endOfFrameGarbageCollectionQueue.front();
		m_endOfFrameGarbageCollectionQueue.pop();
	}

	//m_allMutex.unlock();
}

void MessageBus::addMessage(MessageData *message){
	m_allMutex.lock();
	if (message->messageType != "NULL") {
		//std::cout << message->messageType << std::endl;
		if (message->messageType == "messages") {
			for (size_t i = 0; i < message->messageContents.size(); i++)
			{
				MessageData *tempMessage = new MessageData();
				tempMessage->createFrom(message->messageContents[i]);
				addMessage(tempMessage);
			}
		}
		else if (message->messageType == "echoMessage") {
			message->echoDelay = ma_deserialize_float(message->messageContents[0].data[0]);
			m_echoMessages.push_back(message);
		}
		else if (message->messageType == "triggerNotification") {
			if (canMessage()) {
				notify();
			}
		}
		else if (message->messageType == "addLevelGameScript") {

			GameScript *dynamicGameScript = new GameScript();
			dynamicGameScript->createFromFile(message->messageContents[0].data[0]);
			addMessagingComponent(dynamicGameScript, true);
		}
		else if (message->messageType == "editGameData") {
			dynamic_cast<GameScript*>(m_messagingComponents[message->senderID])->setGameData(message->messageContents[0]);
		} else if (message->messageType == "launchScript") {
			auto pyFunc = makePythonFunction(message->messageContents[0].data[0], message->messageContents[0].data[0]);
			MessageData* tempMessageData = new MessageData(*message);
			tempMessageData->messageContents.erase(tempMessageData->messageContents.begin());
			tempMessageData->senderID = size_t(numeric_limits<size_t>::max);
			tempMessageData->intendedReceiverID = size_t(numeric_limits<size_t>::max);
			addMessage(new MessageData(tempMessageData->processPythonFunc(pyFunc, size_t(numeric_limits<size_t>::max), getEntryIDs(), "")));
		}
		else {
			m_messageQueue.push(message);
		}
	}
	m_allMutex.unlock();
}

void MessageBus::notify(){
    if(!m_messageQueue.empty()){
        MessageData *currentMessage = m_messageQueue.front();
		m_endOfFrameGarbageCollectionQueue.push(currentMessage);
        m_messageQueue.pop();
        for (auto const& x : m_messagingComponents)
        {
#if  MULTITHREADED_SCRIPTING_AND_MESSAGING
			m_threadPool.push(new std::thread(&MessagingComponent::processMessage, x.second, currentMessage, this));
#else
            x.second->processMessage(currentMessage, this);
#endif
        }
    }


}

MessageBus::~MessageBus()
{

	onDestroy();
    //dtor
}
