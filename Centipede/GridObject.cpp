#include "GridObject.h"

GridObject::GridObject()
{

}

bool GridObject::IsPoison()
{
	return is_poison;
}

int GridObject::ReturnHealth()
{
	return health;
}

void GridObject::Initialize()
{
	//generic initializer for gridobjects
	//health must be initialized to 4 even for these
	//this is so that the RepairMushroomField function in SpawningGrid skips over the grid objects used for the high score table
	health= 4;
	Pos.x= 0;
	Pos.y= 0;
}

sf::Vector2f GridObject::ReturnPosition()
{
	return Pos;
}