#include "LevelMaker.h"
#include<fstream>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Animator.h"

LevelMaker::LevelMaker()
{
}

void LevelMaker::levelMakerLoop(std::string outputFile, std::string mapTexFile) {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
	window.setFramerateLimit(60);

	Animator::getInstance().setWindow(&window);

	sf::View view;

	Map tempMap;

	std::string line;
	std::ifstream fileRead(outputFile);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			std::string tempLine;


			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}

			if (tokens[0] == "wall") {
				//std::cout << tokens.size() << std::endl;
				//system("pause");
				tempMap.addWall(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())), (sf::Vector2f(std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str()))));
			}


		}
		fileRead.close();
		//m_player->getTexture().setTexture();
	}

	

	sf::Texture  mapTex;
	mapTex.loadFromFile(mapTexFile);
	sf::Sprite mapSprite;
	mapSprite.setTexture(mapTex);

	bool firstClick = true;
	sf::Vector2f firstMousePos = sf::Vector2f(0,0);
	sf::Vector2f lastMousePos = sf::Vector2f(0,0);
	std::ofstream outFile;
	outFile.open(outputFile, std::ios::app);
	while (window.isOpen())
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			view.move(0,-10);

		}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			view.move(0, 10);

		}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			view.move(-10, 0);

		}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			view.move(10, 0);

		}


		sf::Vector2f mousePosition = sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					
					
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
					if (firstClick) {
						firstClick = false;
						firstMousePos = mousePosition;
						outFile << "wall;" << mousePosition.x << ";" << mousePosition.y << ";";
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
						if(!firstClick) {
							firstClick = true;
							lastMousePos = mousePosition;
							tempMap.addWall(firstMousePos, mousePosition);
							outFile << mousePosition.x << ";" << mousePosition.y << ";" << std::endl;
						}

				}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
					if (firstClick) {
						tempMap.addWall(lastMousePos, mousePosition);
						outFile << "wall;" << lastMousePos.x << ";" << lastMousePos.y << ";" << mousePosition.x << ";" << mousePosition.y << ";" << std::endl;
						lastMousePos = mousePosition;
						
					}

				}
			}

			if (event.type == sf::Event::Closed)
				window.close();
		}

		

		window.clear();

		window.setView(view);

		window.draw(mapSprite);
		tempMap.update(nullptr);
		window.display();
	}
	outFile.close();
	return;
}


LevelMaker::~LevelMaker()
{
}
