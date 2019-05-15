#pragma once
#include "MessagingComponent.h"
#include <sstream>
#include <string>

enum loggerModes {
	logAll, logMessage, logLoggerSpecific
};

class Logger :
	public MessagingComponent
{
protected:
	void pv_processMessage(const MessageData&, MessageBus*)override;
	loggerModes m_mode;
	std::stringstream m_outputString;
	bool m_logToString = false;
public:
	Logger();
	void setMode(loggerModes);
	void logToString(bool);
	void saveLogToFile(std::string);
	~Logger();
};

