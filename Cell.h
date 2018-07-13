#pragma once
enum Direction {
	None,
	Left,
	Right,
};

enum TileType {
	Air = 0,
	Dirt = 1,
	Light = 2,
	Stone = 3,
};

class Cell
{
public:
	Cell(TileType type, int level);
	Cell();
	~Cell();

	TileType Tile;
	int Level;
	Direction Direction;
	bool NoCalc;

	const static int MaxLevel = 8;
	const static int RenderWidth = 8;
	const static int RenderHeight = 8;
	const static int TILE_SIZE = 16;
	static Cell None;

};

