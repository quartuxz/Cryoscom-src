#include "InGameMessages.h"

void InGameMessages::setShowOnlyImportantLines(bool showOnlyImportantLines)
{
	m_showOnlyImportantLines = showOnlyImportantLines;
}

bool InGameMessages::getShowOnlyImportantLines() const
{
	return m_showOnlyImportantLines;
}

void InGameMessages::addLine(lineMessage line)
{
	m_currentAllLines++;
	m_allLines.push(line);
	if (m_currentAllLines > m_lineCap) {
		m_allLines.pop();
	}
	if (line.isImportantMessage) {
		m_currentImportantLines++;
		m_importantLines.push(line);
		if (m_currentImportantLines>m_lineCap) {
			m_importantLines.pop();
		}
	}
}

void InGameMessages::setLineCap(size_t lineCap)
{
	m_lineCap = lineCap;
}

size_t InGameMessages::getLineCap() const
{
	return m_lineCap;
}

std::queue<lineMessage> InGameMessages::getLines() const
{
	if (m_showOnlyImportantLines) {
		return m_importantLines;
	}
	else {
		return m_allLines;
	}
	
}


