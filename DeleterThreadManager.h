#pragma once
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "Bullet.h"

class DeleterThreadManager
{
private:
	std::mutex m_deleterMutex;
	std::condition_variable cv;
	bool m_startWorking = false;
	bool m_firstUse = true;
	bool m_isProgramEnded = false;
	std::thread* m_deleterThread;
	std::queue<Bullet*> m_toBeDeletedBullets;

	void m_deleterThreadFunction();

public:
	static DeleterThreadManager& getInstance()
	{

		static DeleterThreadManager instance; // Guaranteed to be destroyed.
		if (instance.m_firstUse) {
			instance.m_deleterThread = new std::thread(&DeleterThreadManager::m_deleterThreadFunction, &instance);
			instance.m_firstUse = false;
		}
		// Instantiated on first use.
		return instance;
	}


private:
	DeleterThreadManager(){}
public:
	DeleterThreadManager(DeleterThreadManager const&) = delete;
	void operator=(DeleterThreadManager const&) = delete;
	void deleteSomething(Bullet*);
	void endFrame();
	~DeleterThreadManager();
};

