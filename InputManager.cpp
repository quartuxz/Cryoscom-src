#include "InputManager.h"
#include <iostream>


void InputManager::addEventToQueue(const InputEvent &IE)
{
	m_allLock.lock();
	m_IOEvents.push(IE);
	m_allLock.unlock();
}

std::vector<InputManager::InputEventTypes> InputManager::m_getKeyPressedEvent()const
{
	std::vector<InputManager::InputEventTypes> retVal;
	if (m_isWindowOutOfFocus) {
		return retVal;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		retVal.push_back(shiftKeyPressed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		retVal.push_back(moveForward);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		retVal.push_back(moveBackward);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		retVal.push_back(moveLeft);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		retVal.push_back(moveRight);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		retVal.push_back(pickUpKeyPressed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		retVal.push_back(escapeKeyPressed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		retVal.push_back(inventoryKeyPressed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		retVal.push_back(interactKeyPressed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		retVal.push_back(dropItem);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		retVal.push_back(showToolTipsKeyPressed);
	}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		retVal.push_back(dash);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		retVal.push_back(hotbar1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		retVal.push_back(hotbar2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		retVal.push_back(hotbar3);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
		retVal.push_back(hotbar4);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
		retVal.push_back(hotbar5);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		retVal.push_back(showImportantLinesKeyPressed);
	}


	//from down here until the end, only mouse events
	retVal.push_back(noInputEvent);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		retVal.push_back(shoot);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		retVal.push_back(useItem);
	}
	
	return retVal;
}

void InputManager::m_waitForEvents()
{
	sf::Event event;
	m_window = new sf::RenderWindow();
	m_window->create(sf::VideoMode(1900, 900), "SFML works!");
	//m_window->setFramerateLimit(60);
	m_window->setActive(false);
	m_isWindowCreated = true;

	

	std::cout << "wait loop started"<< std::endl;

	while (m_window->waitEvent(event)) {
		if (m_isProgramTerminated) {
			return;
		}


		if (event.type == sf::Event::LostFocus) {
			addEventToQueue(windowOutOfFocus);
			m_isWindowOutOfFocus = true;
		}
		if (event.type == sf::Event::GainedFocus) {
			m_isWindowOutOfFocus = false;
		}

		if (event.type == sf::Event::Closed) {
			//m_window->close();
			addEventToQueue(windowClosed);
		}

		if (m_isWindowOutOfFocus) {
			continue;
		}

		
		

		if (event.type == sf::Event::KeyPressed) {
			std::vector<InputEventTypes> pressedEvents = m_getKeyPressedEvent();
			bool shiftPressed = false;
			for (size_t i = 0; i < pressedEvents.size(); i++)
			{
				if (pressedEvents[i] == noInputEvent) {
					break;
				}
				else if (pressedEvents[i] == shiftKeyPressed) {
					shiftPressed = true;
				}
				if (shiftPressed) {
					if (pressedEvents[i] == moveForward) {
						addEventToQueue(dashForward);
					}else if (pressedEvents[i] == moveBackward) {
						addEventToQueue(dashBackward);
					}else if (pressedEvents[i] == moveLeft) {
						addEventToQueue(dashLeft);
					}else if (pressedEvents[i] == moveRight) {
						addEventToQueue(dashRight);
					}
				}
				
				addEventToQueue(pressedEvents[i]);
			}
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				addEventToQueue(useItem);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				addEventToQueue(shoot);
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			addEventToQueue(generalKeyReleased);
		}
	}
	
	std::cout << "wait loop ended" << std::endl;
}

InputManager::InputManager()
{
}

InputManager::InputEvent InputManager::getEvent()
{
	std::lock_guard<std::mutex> tl(m_allLock);
	InputManager::InputEvent retVal;
	if (!m_IOEvents.empty()) {
		retVal = m_IOEvents.front();
		m_IOEvents.pop();
	}
	else {
		retVal.InputEventType = noInputEvent;
	}
	return retVal;
}

sf::RenderWindow* InputManager::getWindow()
{
	std::lock_guard<std::mutex> tl(m_allLock);
	return m_window;
}

bool InputManager::isInputEventActive(InputEventTypes IE)const
{
	std::vector<InputEventTypes> pressedEvents = m_getKeyPressedEvent();
	for (size_t i = 0; i < pressedEvents.size(); i++)
	{
		if (IE == pressedEvents[i]) {
			return true;
		}
	}
	return false;
}

void InputManager::stop()
{
	std::lock_guard<std::mutex> tl(m_allLock);
	m_isProgramTerminated = true;
	m_eventThread->join();
	delete m_eventThread;
}

void InputManager::launchWindowIOThread()
{
	std::cout << "createdWindow" << std::endl;
	std::lock_guard<std::mutex> tl(m_allLock);
    m_eventThread = new std::thread(&InputManager::m_waitForEvents, this);
	while (!m_isWindowCreated) {

	}
}


InputManager::~InputManager()
{
	delete m_window;
}

InputManager::InputEvent::InputEvent()
{
}

InputManager::InputEvent::InputEvent(InputEventTypes IEType):
	InputEventType(IEType)
{

}

InputManager::InputEvent::InputEvent(InputEventTypes IEType, bool isKeyPressed):
	InputEventType(IEType),
	keyPressed(isKeyPressed)
{
}
