#pragma once
#include <string>
#include <map>

class fileLoader
{
private:
	std::map<std::string, std::string> m_files;
public:
	fileLoader();
	std::string getFile(std::string);
	void loadFile(std::string);
	void loadDirectory(std::string);
	
	~fileLoader();
};

