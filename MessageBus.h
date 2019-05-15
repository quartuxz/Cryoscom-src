#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H
#include<queue>
#include<map>
#include <list>
#include <mutex>
#include <thread>
#include <stack>
#include <string>
#include <boost/python.hpp>
#include "MessagingComponent.h"


boost::python::object makePythonFunction(std::string, std::string);

class GameScript : public MessagingComponent {
private:
	boost::python::object m_pythonFunc;
	decomposedData m_gameData;
	std::string m_cachedSerializedGameData;
protected:
	void pv_processMessage(const MessageData&, MessageBus*)override;
public:
	void createFromFile(std::string);
	decomposedData getGameData()const;
	void setGameData(const decomposedData&);
};

class MessageBus
{
    public:
        MessageBus();
        //add identifiers for MessagingComponents that last the entire lifetime of the MessageBus (can be reffered to at any time)
        void addEntry(std::string, size_t);
        size_t getEntryID(std::string);
        std::string getEntryName(size_t);
        const std::map<std::string, size_t> &getEntryIDs()const;
        //


		void setUpdateInterval(float);

		void startFrame(float);
		bool canMessage();
		void endFrame();
		//bool autoCapFunction = false;

		MessagingComponent *getMessagingComponent(size_t);
        size_t addMessagingComponent(MessagingComponent*, bool dynamicObj = false);
        void addMessage(MessageData*);
        void notify();

		MessageBus &operator=(const MessageBus&);

		void onDestroy();

        ~MessageBus();

    private:

		void m_recursiveJoinAndProcessAll();
		void m_joinThreads();

		std::recursive_mutex m_allMutex;

        //debugging infinite loops
        size_t m_maxChainLength = 10;
        size_t m_chainLength = 0;
        MessageData m_lastMessage;
        //~

		
		std::stack<std::thread*> m_threadPool;
		//capping messaging per second
		float m_addedTime = 0;
		float m_updateInterval = 0.5;

		//

        //entry handling(named MessagingComponents)
        std::map<std::string, size_t> m_entryIDs;
        std::map<size_t, std::string> m_entryNames;
        //

		//memory management
		std::vector<MessagingComponent*> m_dynamicMessagingComponents;
		//

        //core messaging
		std::list<MessageData*> m_echoMessages;

        std::queue<MessageData*> m_messageQueue;
		std::queue<MessageData*> m_endOfFrameGarbageCollectionQueue;

        static size_t m_IDCounter;
        std::map<size_t, MessagingComponent*> m_messagingComponents;
};

#endif // MESSAGEBUS_H
