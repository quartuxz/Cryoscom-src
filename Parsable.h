#pragma once
#include <vector>
#include <string>

class Parsable
{
protected:
	//parses a single line, the first element of the vector is usually the type of the line and the others the parameters.
	virtual void pv_parseStep(std::vector<std::string>) = 0;

	std::vector<std::string> m_lastParseStep;
public:
	Parsable();
	void createFromFile(std::string);
	//use to only read a single line, be sure that line is valid and doesnt contain special tokens
	//such as //;, end; or repeat;
	void createFromSingleLine(std::vector<std::string>);
	virtual ~Parsable();
};

