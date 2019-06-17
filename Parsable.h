#pragma once
#include <vector>
#include <string>

class Parsable
{
protected:
	//parses a single line, the first element of the vector is usually the type of the line and the others the parameters.
	virtual void pv_parseStep(std::vector<std::string>) = 0;
public:
	Parsable();
	void createFromFile(std::string);
	void createFromSingleLine(std::vector<std::string>);
	virtual ~Parsable();
};

