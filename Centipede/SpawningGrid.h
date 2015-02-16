#ifndef _SpawningGrid
#define _SpawningGrid

#include "Game Components\TEAL\CommonElements.h"
#include "GridObject.h"
#include "CommonFunctionsAndValues.h"
#include <queue>

class MushroomFactory;
class PoisonMushroomFactory;

extern const int columns;
extern const int rows;

class SpawningGrid : public GameObject
{
public:

	SpawningGrid();
	void SpawnMushrooms();
	void DeleteMushroomAtPosition(float xcoord, float ycoord);
	void ClearGridPosition(float xcoord, float ycoord);
	void SpawnMushroomAtPosition(float xcoord, float ycoord);
	void ConvertPoisonMushroom(float xcoord, float ycoord);
	void ConvertNormalMushroom(float xcoord, float ycoord);
	void RepairMushroomField();
	bool CheckGridPosition(float xcoord, float ycoord);
	bool CheckPoison(float xcoord, float ycoord);
	int ConvertFloatToGridUnits(float float_coord);
	float ConvertGridUnitsToFloat(int grid_coord);
	void SpawnGridObjectAtPosition(float xcoord, float ycoord);
	void SaveMushroomPositions();
	void LoadMushroomPositions();
	void ClearGrid();
	virtual void Alarm1();
	virtual void Alarm2();
	virtual void Destroy();
	int ReturnMushInPlayerArea();
	void IncreaseMushInPlayerArea();
	void DecreaseMushInPlayerArea();
	
private:
	int mush_in_player_area;
	int scoring_control;
	GridObject *gridArray[columns][rows];
	std::queue<GridObject*> repair_queue;
	std::queue<sf::Vector2f> mushroom_positions;
};

#endif