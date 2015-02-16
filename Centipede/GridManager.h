#ifndef _GridManager
#define _GridManager

#include "Game Components\TEAL\CommonElements.h"
#include <list>

class SpawningGrid;

class GridManager
{
private:
	static GridManager *self;

	GridManager(){ };
	GridManager(const GridManager&){ };
	GridManager& operator= (const GridManager&){ };

	~GridManager(){ };
	
	static GridManager& Instance()
	{
		if (!self)
			self= new GridManager();
		return *self;
	};

	SpawningGrid* current_grid;
	SpawningGrid* player_1_grid;
	SpawningGrid* player_2_grid;
	SpawningGrid* ai_player_grid;

public:

	static void Instantiate();
	static void SwapGrids();
	static void SpawnMushrooms();
	static int ReturnMushInPlayerArea();
	static void ClearGridPosition(float xcoord, float ycoord);
	static float ConvertGridUnitsToFloat(int grid_coord);
	static int ConvertFloatToGridUnits(float float_coord);
	static bool CheckGridPosition(float xcoord, float ycoord);
	static bool CheckPoison(float xcoord, float ycoord);
	static void ConvertPoisonMushroom(float xcoord, float ycoord);
	static void SpawnMushroomAtPosition(float xcoord, float ycoord);
	static void IncrementMushInPlayerArea();
	static void DecrementMushInPlayerArea();
	static void RepairMushroomField();
	static void SpawnGridObjectAtPosition(float xcoord, float ycoord);
	static void DeleteMushroomAtPosition(float xcoord, float ycoord);
	static void DeleteAllGrids();

};

#endif _GridManager