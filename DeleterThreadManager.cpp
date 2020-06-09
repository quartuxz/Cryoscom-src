#include "DeleterThreadManager.h"
#include <iostream>

void DeleterThreadManager::m_deleterThreadFunction()
{
	while (!m_isProgramEnded) {
		if (m_startWorking) {
			auto m_tempToBeDeletedBullets = m_toBeDeletedBullets;
			m_toBeDeletedBullets = std::queue<Bullet*>();
			m_startWorking = false;
			
			
			while (!m_tempToBeDeletedBullets.empty()) {

				delete m_tempToBeDeletedBullets.front();
				m_tempToBeDeletedBullets.pop();
			}
			

		}
	}
}

void DeleterThreadManager::deleteSomething(Bullet *bullet)
{
	while (m_startWorking) {

	}
	m_toBeDeletedBullets.push(bullet);
	
	
}

void DeleterThreadManager::endFrame()
{
	while (m_startWorking) {

	}
	m_startWorking = true;
}

DeleterThreadManager::~DeleterThreadManager()
{
	m_isProgramEnded = true;
	m_deleterThread->join();
	delete m_deleterThread;
}
