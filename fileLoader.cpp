#include "fileLoader.h"
#include <filesystem>


fileLoader::fileLoader()
{
}

std::string fileLoader::getFile(std::string fileName)
{
	return m_files[fileName];
}


void fileLoader::loadDirectory(std::string dirName)
{

		
}

void fileLoader::loadFile(std::string fileName)
{
}

fileLoader::~fileLoader()
{
}
