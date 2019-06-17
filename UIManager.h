#pragma once
#include "Menu.h"
#include <map>

class UIManager
{
private:
	std::map<std::string, Menu*> m_menus;
public:
	UIManager();

	~UIManager();
};

