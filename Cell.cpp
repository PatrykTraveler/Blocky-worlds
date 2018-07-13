#include "Cell.h"
Cell::Cell(){}
Cell::Cell(TileType type, int level) : Tile(type), Level(level), NoCalc(false)
{
	
}

Cell::~Cell()
{
}

Cell Cell::None = Cell(TileType::Air, 0);
