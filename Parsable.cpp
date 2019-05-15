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
			else{
				pv_parseStep(tokens);
			}
			

		}
		fileRead.close();
	}
}

Parsable::~Parsable()
{
}
