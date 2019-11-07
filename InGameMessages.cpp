#include "InGameMessages.h"

void InGameMessages::addLine(lineMessage line)
{
	m_currentLines++;
	m_lines.push(line);
	if (m_currentLines > m_lineCap) {
		m_lines.pop();
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
	return m_lines;
}
