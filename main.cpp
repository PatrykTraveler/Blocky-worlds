#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <Windows.h>
#include <iostream>
#include "Cell.h"
#include "WaterSimulation.h"

const int types[4] = { 0,1,2,3 };

int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Test");
	window.setFramerateLimit(30);
	WaterSimulation simulation;
	int blockCounter = 0, waterLevel = 0;

	while (window.isOpen())
	{
		blockCounter %= 4;
		waterLevel %= 8;
		sf::Mouse mouse;
		sf::Event event;

		sf::Vector2i mPos = sf::Vector2i(mouse.getPosition(window).x/Cell::TILE_SIZE, mouse.getPosition(window).y/Cell::TILE_SIZE);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q)
					blockCounter++;
				if (event.key.code == sf::Keyboard::W)
					waterLevel++;
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					simulation.putBlock(mPos.x, mPos.y, (TileType)types[blockCounter], waterLevel);
				}
			}
		}
		simulation.Update();
		printf("%d %d", blockCounter, waterLevel);
		system("cls");

		window.clear(sf::Color::Black);
		simulation.draw(window);
		window.display();
	}
}