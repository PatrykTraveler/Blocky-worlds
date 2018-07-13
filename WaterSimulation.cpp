#include "WaterSimulation.h"



WaterSimulation::WaterSimulation()
{
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			if (y < 3)
				grid[x][y] = Cell(TileType::Air, rand() % Cell::MaxLevel + 1);
			else
				grid[x][y] = Cell(TileType::Air, 0);
		}
	}
}

WaterSimulation::~WaterSimulation()
{
}

void WaterSimulation::Update() {
	// loop from bottom to top
	for (int y = HEIGHT - 1; y >= 0; y--)
	{
		// loop from right to left
		for (int x = 0; x < WIDTH; x++)
		{
			updateFluidCell(x, y);
		}
	}
	// loop from bottom to top
	for (int y = HEIGHT - 1; y >= 0; y--)
	{
		// loop from right to left
		for (int x = 0; x < WIDTH; x++)
		{
			grid[x][y].NoCalc = false;
		}
	}
	vertices.clear();
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			Cell cell = grid[x][y];
			if (!(cell.Level == 0 && cell.Tile == TileType::Air)) {
				sf::Color color = getColor(cell,x,y);
				vertices.push_back(sf::Vertex(sf::Vector2f(x*Cell::TILE_SIZE, y*Cell::TILE_SIZE), color));
				vertices.push_back(sf::Vertex(sf::Vector2f((x + 1)*Cell::TILE_SIZE, y*Cell::TILE_SIZE), color));
				vertices.push_back(sf::Vertex(sf::Vector2f((x + 1)*Cell::TILE_SIZE, (y + 1)*Cell::TILE_SIZE), color));
				vertices.push_back(sf::Vertex(sf::Vector2f(x*Cell::TILE_SIZE, (y + 1)*Cell::TILE_SIZE), color));
			}
		}
	}
}

void WaterSimulation::updateFluidCell(int x, int y) {
	Cell *cell = &grid[x][y], *bottomCell, *leftCell, *rightCell;
	leftCell = new Cell();
	rightCell = new Cell();
	bottomCell = new Cell();
	*leftCell = *rightCell = *bottomCell = Cell::None;

	CellMatrix matrix = CellMatrix::None;

	if (cell->Tile != TileType::Air) return;
	if (cell->Level == 0) return;
	if (cell->NoCalc || bottomCell->NoCalc || leftCell->NoCalc || rightCell->NoCalc)
		return;

	if (y + 1 < HEIGHT) {
		bottomCell = &grid[x][y + 1];
		if (bottomCell->Tile == TileType::Air && bottomCell->Level < Cell::MaxLevel)
			matrix |= CellMatrix::Bottom;
	}

	if (x - 1 >= 0) {
		leftCell = &grid[x - 1][y];
		if (leftCell->Tile == TileType::Air && leftCell->Level < Cell::MaxLevel && leftCell->Level < cell->Level)
			matrix |= CellMatrix::Left;
	}

	if (x + 1 < WIDTH) {
		rightCell = &grid[x + 1][y];
		if (rightCell->Tile == TileType::Air && rightCell->Level < Cell::MaxLevel && rightCell->Level < cell->Level)
			matrix |= CellMatrix::Right;
	}
	int leftOverFluid;
	switch (matrix)
	{
	case CellMatrix::Bottom:
		cell->Level = FlowBottom(cell, bottomCell);
		break;

	case CellMatrix::Bottom | CellMatrix::Left:
		leftOverFluid = FlowBottom(cell, bottomCell);
		if (leftOverFluid > 0)
		{
			FlowLeft(cell, leftCell);
		}
		break;

	case CellMatrix::Bottom | CellMatrix::Right:
		leftOverFluid = FlowBottom(cell, bottomCell);
		if (leftOverFluid > 0)
		{
			FlowRight(cell, rightCell);
		}
		break;

	case CellMatrix::Bottom | CellMatrix::Right | CellMatrix::Left:
		leftOverFluid = FlowBottom(cell, bottomCell);
		if (leftOverFluid > 0)
		{
			FlowLeftRight(cell, leftCell, rightCell);
		}
		break;

	case CellMatrix::Left:
		FlowLeft(cell, leftCell);
		break;

	case CellMatrix::Right:
		FlowRight(cell, rightCell);
		break;

	case CellMatrix::RightLeft:
		FlowLeftRight(cell, leftCell, rightCell);
		break;
	}
}

void WaterSimulation::FlowRight(Cell *cell, Cell *rightCell) {
	int amountToSpread = (rightCell->Level + cell->Level) / 2;
	int remainder = (rightCell->Level + cell->Level) % 2;

	rightCell->Level = amountToSpread + remainder;
	rightCell->NoCalc = true;
	rightCell->Direction = Direction::Right;
	cell->Level = amountToSpread;
	cell->NoCalc = true;
}

void WaterSimulation::FlowLeft(Cell *cell, Cell *leftCell)
{
	int amountToSpread = (leftCell->Level + cell->Level) / 2;
	int remainder = (leftCell->Level + cell->Level) % 2;

	leftCell->Level = amountToSpread + remainder;
	leftCell->NoCalc = true;
	leftCell->Direction = Direction::Left;
	cell->Level = amountToSpread;
	cell->NoCalc = true;
}

void WaterSimulation::FlowLeftRight(Cell *cell, Cell *leftCell, Cell *rightCell)
{
	int amountToSpread = (leftCell->Level + rightCell->Level + cell->Level) / 3;
	int remainder = (leftCell->Level + rightCell->Level + cell->Level) % 3;
	// if we have a remainder...
	if (remainder > 0)
	{
		// 
		if (cell->Direction == Direction::Left)
		{
			leftCell->Level = amountToSpread + remainder;
			leftCell->Direction = Direction::Left;
			rightCell->Level = amountToSpread;
		}
		else
		{
			leftCell->Level = amountToSpread;
			rightCell->Level = amountToSpread + remainder;
			rightCell->Direction = Direction::Right;
		}

	}
	else
	{
		// otherwise it's an even split
		leftCell->Level = amountToSpread;
		leftCell->Direction = Direction::None;
		rightCell->Level = amountToSpread;
		rightCell->Direction = Direction::None;
	}

	cell->Level = amountToSpread;
	cell->NoCalc = true;
	cell->Direction = Direction::None;
	leftCell->NoCalc = true;
	rightCell->NoCalc = true;
}

int WaterSimulation::FlowBottom(Cell *cell, Cell *bottomCell)
{
	// check to see how much fluid can fall down
	int spaceAvailable = Cell::MaxLevel - bottomCell->Level;
	int amountToMove = (int)min(spaceAvailable, cell->Level);

	// move all fluid that can be moved
	bottomCell->Level += amountToMove;
	bottomCell->NoCalc = true;
	bottomCell->Direction = Direction::None;
	cell->Level -= amountToMove;
	cell->NoCalc = true;

	return cell->Level;
}

void WaterSimulation::putBlock(int x, int y, TileType type, int level)
{
	if (type == TileType::Light) {
		lights.push_back(sf::Vector2i(x, y));
	}
	this->grid[x][y] = Cell(type, level);
}

sf::Color WaterSimulation::getColor(Cell cell, int x, int y)
{
	float attenuation = getAttenuation(x, y);
	sf::Color color;
	switch (cell.Tile) {
	case TileType::Air:
		if (cell.Level > 0) {
			float c = (float)(cell.Level) / (float)(Cell::MaxLevel);
			color = sf::Color::Blue;
			//color.r = 255 * (1 - c);
			//color.g = 255 * (1 - c);
			color.b *= c;
			color.b *= attenuation;
		}
		else {
			color = sf::Color::White;
		}
		break;
	case TileType::Dirt:
		color = sf::Color::Green;
		color.g = 255 * attenuation;
		break;
	case TileType::Light:
		color = sf::Color::White;
		break;
	default:
		color = sf::Color::Magenta;
		break;
	}
	return color;
}

float WaterSimulation::getAttenuation(int x, int y) {
	float sum = 0;
	std::vector <float> distances;
	for (int i = 0; i < lights.size(); i++) {
		int dist = std::sqrt((lights[i].x - x)*(lights[i].x - x) + (lights[i].y - y)*(lights[i].y - y));
		distances.push_back(dist);
	}

	for (int i = 0; i < distances.size(); i++) {
		sum += 1/distances[i];
	}
	
	return min(1.0f, sum);
}

void WaterSimulation::draw(sf::RenderWindow & window)
{
	window.draw(&vertices[0], vertices.size(), sf::Quads);
}
