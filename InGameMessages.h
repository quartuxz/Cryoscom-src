#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <SFML/Graphics.hpp>

enum class lineMessageType {
	gameImportant, anyType, debugMessage
};

struct lineMessage {
	std::string message;
	sf::Color messageColor;
	lineMessageType messageType = lineMessageType::anyType;

};

class InGameMessages
{
public:
	static InGameMessages& getInstance() {
		static InGameMessages instance;
		return instance;
	}
private:
	InGameMessages(){}
	std::map<lineMessageType, std::queue<lineMessage>> m_allLines;
	lineMessageType m_showMessageType = lineMessageType::anyType;
	size_t m_lineCap = 15;

	std::mutex m_allLock;
public:
	InGameMessages(InGameMessages const&) = delete;
	void operator=(InGameMessages const&) = delete;

	void setMessageShowType(lineMessageType);
	lineMessageType getMessageShowType();

	void addLine(lineMessage);
	void setLineCap(size_t);
	size_t getLineCap();

	std::queue<lineMessage> getLines();

};

