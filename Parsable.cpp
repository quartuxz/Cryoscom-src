#include "Parsable.h"
#include <fstream>


Parsable::Parsable()
{
}

void Parsable::createFromFile(std::string fileName) {
	std::string line;
	std::ifstream fileRead(fileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}
			else if (tokens[0] == "end") {
				break;
			}
			else if (tokens[0] == "load") {
				createFromFile(tokens[1]);
			}
			else if (tokens[0] == "repeat") {
				pv_parseStep(m_lastParseStep);
				continue;
			}
			else{
				pv_parseStep(tokens);
			}
			
			m_lastParseStep = tokens;
		}
		fileRead.close();
	}
}

void Parsable::createFromSingleLine(std::vector<std::string> line)
{
	pv_parseStep(line);
}

Parsable::~Parsable()
{
}
