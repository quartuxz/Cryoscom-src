#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>

class InputManager
{
public:
	enum InputEventTypes {
		noInputEvent, dropItem, windowClosed, moveForward, moveBackward, moveLeft, moveRight, shoot, interactKeyPressed, escapeKeyPressed, inventoryKeyPressed, pickUpKeyPressed, showToolTipsKeyPressed, generalKeyReleased, useItem, dash, shiftKeyPressed, dashForward, dashBackward, dashLeft, dashRight,
		hotbar1, hotbar2, hotbar3, hotbar4, hotbar5, showImportantLinesKeyPressed, windowOutOfFocus
	};

	struct InputEvent {
		InputEventTypes InputEventType;
		bool keyPressed = true;

		InputEvent();
		InputEvent(InputEventTypes);
		InputEvent(InputEventTypes, bool);

	};

private:
	std::mutex m_allLock;
	std::queue<InputEvent> m_IOEvents;
	sf::RenderWindow *m_window;

	std::atomic<bool> m_isWindowCreated = false;
	std::atomic<bool> m_isProgramTerminated = false;
	std::atomic<bool> m_isWindowOutOfFocus = false;

	std::vector<InputEventTypes> m_getKeyPressedEvent()const;
	void m_waitForEvents();
	std::thread *m_eventThread = nullptr;
public:


	InputManager();
	InputEvent getEvent();

	sf::RenderWindow* getWindow();

	void addEventToQueue(const InputEvent&);

	bool isInputEventActive(InputEventTypes)const;

	void stop();
	void launchWindowIOThread();

	~InputManager();
};

