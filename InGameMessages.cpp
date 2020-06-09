#include "InGameMessages.h"


void InGameMessages::setMessageShowType(lineMessageType messageType)
{
	std::lock_guard<std::mutex> tempLockGuard(m_allLock);
	m_showMessageType = messageType;

}

lineMessageType InGameMessages::getMessageShowType()
{
	std::lock_guard<std::mutex> tempLockGuard(m_allLock);
	return m_showMessageType;
}

void InGameMessages::addLine(lineMessage line)
{
	std::lock_guard<std::mutex> tempLockGuard(m_allLock);
	m_allLines[lineMessageType::anyType].push(line);
	if (line.messageType != lineMessageType::anyType) {
		m_allLines[line.messageType].push(line);
	}
	for (auto& x : m_allLines)
	{
		if (x.second.size()>m_lineCap) {
			x.second.pop();
		}
	}
}

void InGameMessages::setLineCap(size_t lineCap)
{
	std::lock_guard<std::mutex> tempLockGuard(m_allLock);
	m_lineCap = lineCap;
}

size_t InGameMessages::getLineCap()
{
	std::lock_guard<std::mutex> tempLockGuard(m_allLock);
	return m_lineCap;
}

std::queue<lineMessage> InGameMessages::getLines()
{
	std::lock_guard<std::mutex> tempLockGuard(m_allLock);
	return m_allLines[m_showMessageType];
	
}


