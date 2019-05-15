#include "Logger.h"
#include <iostream>


void Logger::pv_processMessage(const MessageData &message, MessageBus *bus) {
	MessageData tempMessage = message;
	//std::cout << tempMessage.serialize().serialize() << std::endl;
}

Logger::Logger()
{
}

void Logger::setMode(loggerModes mode) {

}
void Logger::logToString(bool logsToString) {

}
void Logger::saveLogToFile(std::string fileName) {

}

Logger::~Logger()
{
}
