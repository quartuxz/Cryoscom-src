#pragma once
#include <vector>
#include <string>

class Parsable
{
protected:
	virtual void pv_parseStep(std::vector<std::string>) = 0;
public:
	Parsable();
	void createFromFile(std::string);
	virtual ~Parsable();
};

