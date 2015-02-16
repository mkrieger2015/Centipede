#include "GridManager.h"
#include "SpawningGrid.h"

GridManager* GridManager::self= NULL;

void GridManager::Instantiate()
{
	//creates three grids, one for the ai, one for p1, and one for p2. sets the currently selected grid to ai
	Instance();
	Instance().player_1_grid= new SpawningGrid();
	Instance().player_2_grid= new SpawningGrid();
	Instance().ai_player_grid= new SpawningGrid();
	Instance().current_grid= Instance().ai_player_grid;
}

void GridManager::SwapGrids()
{
	//if the current player is player 1, swap grids to player 2
	if(Instance().current_grid == Instance().player_1_grid)
	{
		//this saves the mushroom positions before the screen is cleared of mushrooms
		Instance().current_grid->SaveMushroomPositions();
		//clear the screen of mushrooms
		Instance().current_grid->ClearGrid();
		//swap the grid this manager is pointing to
		Instance().current_grid= Instance().player_2_grid;
		//load its mushrooms
		Instance().current_grid->LoadMushroomPositions();
	}
	//if the current player is player 1, swap grids to player 2
	else if(Instance().current_grid == Instance().player_2_grid)
	{
		Instance().current_grid->SaveMushroomPositions();
		Instance().current_grid->ClearGrid();
		Instance().current_grid= Instance().player_1_grid;
		Instance().current_grid->LoadMushroomPositions();
	}
	//if the current player is ai, swap grids to player 1
	else if(Instance().current_grid == Instance().ai_player_grid)
	{
		Instance().current_grid->SaveMushroomPositions();
		Instance().current_grid->ClearGrid();
		Instance().current_grid= Instance().player_1_grid;
		Instance().current_grid->LoadMushroomPositions();
	}
}

//all of these functions are simply accessors to functions within the SpawningGrid class. please look there for more information

void GridManager::SpawnMushrooms()
{
	Instance().current_grid->SpawnMushrooms();
}

int GridManager::ReturnMushInPlayerArea()
{
	return Instance().current_grid->ReturnMushInPlayerArea();
}

void GridManager::ClearGridPosition(float xcoord, float ycoord)
{
	Instance().current_grid->ClearGridPosition(xcoord,ycoord);
}

float GridManager::ConvertGridUnitsToFloat(int grid_coord)
{
	return Instance().current_grid->ConvertGridUnitsToFloat(grid_coord);
}

int GridManager::ConvertFloatToGridUnits(float float_coord)
{
	return Instance().current_grid->ConvertFloatToGridUnits(float_coord);
}

bool GridManager::CheckGridPosition(float xcoord, float ycoord)
{
	return Instance().current_grid->CheckGridPosition(xcoord,ycoord);
}

bool GridManager::CheckPoison(float xcoord, float ycoord)
{
	return Instance().current_grid->CheckPoison(xcoord,ycoord);
}

void GridManager::ConvertPoisonMushroom(float xcoord, float ycoord)
{
	Instance().current_grid->ConvertPoisonMushroom(xcoord,ycoord);
}

void GridManager::SpawnMushroomAtPosition(float xcoord, float ycoord)
{
	Instance().current_grid->SpawnMushroomAtPosition(xcoord,ycoord);
}

void GridManager::IncrementMushInPlayerArea()
{
	Instance().current_grid->IncreaseMushInPlayerArea();
}

void GridManager::DecrementMushInPlayerArea()
{
	Instance().current_grid->DecreaseMushInPlayerArea();
}

void GridManager::RepairMushroomField()
{
	Instance().current_grid->RepairMushroomField();
}

void GridManager::SpawnGridObjectAtPosition(float xcoord, float ycoord)
{
	Instance().current_grid->SpawnGridObjectAtPosition(xcoord, ycoord);
}

void GridManager::DeleteMushroomAtPosition(float xcoord, float ycoord)
{
	Instance().current_grid->DeleteMushroomAtPosition(xcoord, ycoord);
}

void GridManager::DeleteAllGrids()
{
	//clears mushrooms on screen and marks grids for destroy
	Instance().current_grid->ClearGrid();
	//delete all grid objects
	Instance().player_1_grid->MarkForDestroy();
	Instance().player_2_grid->MarkForDestroy();
	Instance().ai_player_grid->MarkForDestroy();
}
