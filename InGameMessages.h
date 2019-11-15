#pragma once
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>


struct lineMessage {
	std::string message;
	sf::Color messageColor;
	bool isImportantMessage = false;
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
	std::queue<lineMessage> m_allLines;
	std::queue<lineMessage> m_importantLines;
	size_t m_currentAllLines = 0;
	size_t m_currentImportantLines;
	bool m_showOnlyImportantLines = false;
	size_t m_lineCap = 15;
public:
	InGameMessages(InGameMessages const&) = delete;
	void operator=(InGameMessages const&) = delete;

	void setShowOnlyImportantLines(bool);
	bool getShowOnlyImportantLines()const;

	void addLine(lineMessage);
	void setLineCap(size_t);
	size_t getLineCap()const;

	std::queue<lineMessage> getLines()const;

};

