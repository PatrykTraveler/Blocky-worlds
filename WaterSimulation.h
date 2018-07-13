#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"
#include "utilities.h"
enum class FlowDirection {
	None, Right, Left, Both = Left | Right
};

enum class CellMatrix {
	None = 0,
	Bottom = 1,
	Left = 2,
	Right = 4,
	RightLeft = Left | Right,
	All = Bottom | Left | Right,
};

class WaterSimulation
{
public:
	WaterSimulation();
	~WaterSimulation();

	void Update();
	void putBlock(int x, int y, TileType type, int level);
	void draw(sf::RenderWindow& window);

private:
	void updateFluidCell(int x, int y);
	void FlowRight(Cell *cell, Cell *rightCell);
	void FlowLeft(Cell *cell, Cell *leftCell);
	void FlowLeftRight(Cell *cell, Cell *leftCell, Cell *rightCell);
	int FlowBottom(Cell *cell, Cell *bottomCell);
	float getAttenuation(int x, int y);

	sf::Color getColor(Cell cell, int x, int y);

//private:
	static const int WIDTH = 80;
	static const int HEIGHT = 45;

	Cell grid[WIDTH][HEIGHT];
	std::vector <sf::Vertex> vertices;
	std::vector <sf::Vector2i> lights;
};

