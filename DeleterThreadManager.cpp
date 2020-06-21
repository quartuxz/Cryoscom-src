#include "DeleterThreadManager.h"
#include <iostream>


//VERY BAD IMPLEMENTATION USES 99% OF THREAD(BAD BAD)
void DeleterThreadManager::m_deleterThreadFunction()
{
	while (!m_isProgramEnded) {
		std::unique_lock<std::mutex> lk(m_deleterMutex);
		cv.wait(lk, [this]{return m_startWorking;});
		auto m_tempToBeDeletedBullets = m_toBeDeletedBullets;
		m_toBeDeletedBullets = std::queue<Bullet*>();
		m_startWorking = false;
			
			
		while (!m_tempToBeDeletedBullets.empty()) {

			delete m_tempToBeDeletedBullets.front();
			m_tempToBeDeletedBullets.pop();
		}
	}
}

void DeleterThreadManager::deleteSomething(Bullet *bullet)
{
	std::lock_guard<std::mutex> lk(m_deleterMutex);
	m_toBeDeletedBullets.push(bullet);
	
	
}

void DeleterThreadManager::endFrame()
{
	{
		std::lock_guard<std::mutex> lk(m_deleterMutex);
		m_startWorking = true;
	}
	cv.notify_one();
}

DeleterThreadManager::~DeleterThreadManager()
{
	m_isProgramEnded = true;
	m_deleterThread->join();
	delete m_deleterThread;
}
