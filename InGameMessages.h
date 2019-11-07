#pragma once
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>


struct lineMessage {
	std::string message;
	sf::Color messageColor;
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
	std::queue<lineMessage> m_lines;
	size_t m_currentLines = 0;
	size_t m_lineCap = 15;
public:
	InGameMessages(InGameMessages const&) = delete;
	void operator=(InGameMessages const&) = delete;

	void addLine(lineMessage);
	void setLineCap(size_t);
	size_t getLineCap()const;

	std::queue<lineMessage> getLines()const;

};

