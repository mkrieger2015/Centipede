#include "WaveManager.h"
#include "SpawningGrid.h"
#include "Mushroom.h"
#include "PoisonMushroom.h"
#include "MushroomFactory.h"
#include "PoisonMushroomFactory.h"
#include "ScoreManager.h"
#include "GridObjectFactory.h"
#include "PlayerManager.h"

extern const int spawn_chance;
extern const float pixelvalue;
extern const float y_offset;
extern const int columns;
extern const int rows;
extern const int turn_offset;
extern const int player_boundary_int;

SpawningGrid::SpawningGrid()
{
	//initializes # of mushrooms in player area to zero
	mush_in_player_area= 0;
	//scoring control is utilized to keep track of each pair of mushrooms during the repair phase
	//it may seem out of scope, but consider that the mushroom repair event is repeated via the alarm system
	//thus, we have to keep this as an object-wide variable
	scoring_control= 0;
}

void SpawningGrid::SpawnMushrooms()
{
	float xcoord;
	float ycoord;
	int spawn_roll= 0;
	//this double-nested for loop goes through the whole field column by column from left to right
	for (int i= 0; i < columns; i++)
	{
		for (int j= 0; j < rows; j++)
		{
			//roll a percentage chance to spawn
			spawn_roll= (rand() % 100);
			if (spawn_roll < spawn_chance)
			{
				xcoord= i*pixelvalue;
				//we have a y_offset here because we do not want any mushrooms to spawn at the top row of the screen, which is reserved for score and lives
				ycoord= (j*pixelvalue)+(y_offset*pixelvalue);
				gridArray[i][j]= MushroomFactory::CreateMushroom(xcoord,ycoord);
			}
			else
			{
				//if you don't spawn a mushroom, make sure to mark the space in the array of grid objects as null
				gridArray[i][j]= NULL;
			}
		}
	}
}

int SpawningGrid::ReturnMushInPlayerArea()
{
	return mush_in_player_area;
}

void SpawningGrid::DeleteMushroomAtPosition(float xcoord, float ycoord)
{
	//converts the float values to int values to navitage the array
	int array_x= xcoord/pixelvalue;
	int array_y= (ycoord-y_offset)/pixelvalue;
	if(gridArray[array_x][array_y] != NULL)
	{
		gridArray[array_x][array_y]->MarkForDestroy();
		gridArray[array_x][array_y]= NULL;
	}
}

void SpawningGrid::ClearGridPosition(float xcoord, float ycoord)
{
	//converts the float values to int values to navitage the array
	int array_x= xcoord/pixelvalue;
	int array_y= (ycoord-y_offset)/pixelvalue;
	gridArray[array_x][array_y]= NULL;
}

float SpawningGrid::ConvertGridUnitsToFloat(int grid_coord)
{
	float return_float;
	return_float= grid_coord*pixelvalue;
	return return_float;
}

int SpawningGrid::ConvertFloatToGridUnits(float float_coord)
{
	int return_int;
	int int_pixelvalue= pixelvalue;
	int coordvalue= float_coord;
	return_int = (coordvalue - (coordvalue % int_pixelvalue))/pixelvalue;
	return return_int;
}

bool SpawningGrid::CheckGridPosition(float xcoord, float ycoord)
{
	//converts the float values to int values to navitage the array
	int x= ConvertFloatToGridUnits(xcoord);
	//remember, we have the y offset because we pass over the first row
	int y= ConvertFloatToGridUnits(ycoord) - y_offset;
	if( y >= 0 && y < rows && x < columns)
	{
		if(gridArray[x][y] != NULL)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool SpawningGrid::CheckPoison(float xcoord, float ycoord)
{
	//converts the float values to int values to navitage the array
	int x= ConvertFloatToGridUnits(xcoord);
	int y= ConvertFloatToGridUnits(ycoord) - 1;
	if( y >= 0 && y < rows && x > 0 && x < columns)
	{
		if (gridArray[x][y]->IsPoison() == true)
		{
			return true;
		}
		else if (gridArray[x][y]->IsPoison() == false)
		{
			return false;
		}
	}
	else
		return false;
}

void SpawningGrid::ConvertPoisonMushroom(float xcoord, float ycoord)
{
	//first, converts the float values to int values, which represent grid positions
	int grid_x= ConvertFloatToGridUnits(xcoord);
	int grid_y= ConvertFloatToGridUnits(ycoord);
	//then, we convert BACK to floats
	//we went through this process to convert the two floats so that they are directly snapped to the grid
	int x_position= ConvertGridUnitsToFloat(grid_x);
	int y_position= ConvertGridUnitsToFloat(grid_y);
	//if the position is filled and it's not filled by a poison mushroom...
	if(CheckPoison(xcoord,ycoord) == false && CheckGridPosition(xcoord,ycoord) == true)
	{
		//return the health of the current mushroom, we need it to convert it to poison mushroom
		int previous_health= gridArray[grid_x][grid_y-1]->ReturnHealth();
		gridArray[grid_x][grid_y-1]->MarkForDestroy();
		//this may appear redundant, but I'm doing it just to be sure
		gridArray[grid_x][grid_y-1]= NULL;
		gridArray[grid_x][grid_y-1]= PoisonMushroomFactory::ConvertPoisonMushroom(x_position,y_position,previous_health);
	}
}

void SpawningGrid::SpawnMushroomAtPosition(float xcoord, float ycoord)
{
	//first, converts the float values to int values, which represent grid positions
	int array_x= ConvertFloatToGridUnits(xcoord);
	int array_y= ConvertFloatToGridUnits(ycoord);
	//then, we convert BACK to floats
	//we went through this process to convert the two floats so that they are directly snapped to the grid
	float converted_x= ConvertGridUnitsToFloat(array_x);
	float converted_y= ConvertGridUnitsToFloat(array_y);
	//if the spawning position is within the grid area, and there's nothing currently occupying that spot on the grid...
	if( array_y > 0 && array_y <= rows && array_x >= 0 && array_x < columns && CheckGridPosition(xcoord,ycoord) == false)
	{
		gridArray[array_x][array_y-1]= MushroomFactory::CreateMushroom(converted_x,converted_y);
	}
}

void SpawningGrid::RepairMushroomField()
{
	//this double-nested for loop goes through the whole field column by column from left to right
	for (int i= 0; i < columns; i++)
	{
		for (int j= 0; j < rows; j++)
		{
			if(gridArray[i][j] != NULL)
			{
				//if the health of this mushroom is less than for, add it to the repair queue
				if(gridArray[i][j]->IsPoison() == true || gridArray[i][j]->ReturnHealth() < 4)
					repair_queue.push(gridArray[i][j]);
			}
		}
	}
	//if the repair queue has thing to repair, repair the top one, pop it, and then set an alarm to repeat the process
	if(repair_queue.empty() == false)
	{
		GridObject* temp;
		temp= repair_queue.front();
		temp->Repair();
		//increment scoring control - for every two mushrooms repaired, 5 points must be awarded
		scoring_control++;
		repair_queue.pop();
		SetAlarm(1,0.125);
	}
	//if the queue is empty, set the alarm to respawn the player
	else if(repair_queue.empty() == true)
	{
		SetAlarm(2,.25);
	}
}

void SpawningGrid::Alarm1()
{
	//simply repeats the events of RepairMushroomField
	//this is on a timer so that the repair sequence has the "rapid fire" effect
	if(repair_queue.empty() == false)
	{
		GridObject* temp;
		temp= repair_queue.front();
		temp->Repair();
		repair_queue.pop();
		scoring_control++;
		//this is where the scoring control is checked and points are awarded
		if(scoring_control == 2)
		{
			scoring_control= 0;
			ScoreManager::AddPoints(5);
		}
		SetAlarm(1,0.125);
	}
	else if(repair_queue.empty() == true)
	{
		SetAlarm(2,.25);
	}
}

void SpawningGrid::Alarm2()
{
	//reset scoring control
	scoring_control= 0;
	//if the player still has lives, reset the wave
	if( PlayerManager::ReturnLives() > 0)
		WaveManager::ResetWave();
	//begin respawn sequence (more complex than you think - please look at PlayerManager for more info)
	PlayerManager::Respawn();
}

void SpawningGrid::SpawnGridObjectAtPosition(float xcoord, float ycoord)
{
	//this event is for spawning a generic "grid object" 
	//used for the title screen high score table, these are invisible objects
	int array_x= xcoord/pixelvalue;
	int array_y= (ycoord-y_offset)/pixelvalue;
	if(gridArray[array_x][array_y] != NULL)
	{
		gridArray[array_x][array_y]->MarkForDestroy();
	}
	gridArray[array_x][array_y]= NULL;
	gridArray[array_x][array_y]= GridObjectFactory::CreateGridObject();
}

void SpawningGrid::SaveMushroomPositions()
{
	//saves the mushroom positions of this grid into a queue, to be loaded later
	for (int i= 0; i < columns; i++)
	{
		for (int j= 0; j < rows; j++)
		{
			if(gridArray[i][j] != NULL)
			{
				mushroom_positions.push(gridArray[i][j]->ReturnPosition());
			}
		}
	}
}

void SpawningGrid::LoadMushroomPositions()
{
	//if the queue is empty, create a new randomized field
	if(mushroom_positions.empty() == true)
	{
		SpawnMushrooms();
	}
	//if the queue is not empty, then read from the queue and repopulate the mushroom field
	else if(mushroom_positions.empty() == false)
	{
		while(mushroom_positions.empty() == false)
		{
			SpawnMushroomAtPosition(mushroom_positions.front().x,mushroom_positions.front().y);
			mushroom_positions.pop();
		}
	}
}

void SpawningGrid::ClearGrid()
{
	//double-nested for loop goes through the entire grid and destroys all objects/nulls all positions
	for (int i= 0; i < columns; i++)
	{
		for (int j= 0; j < rows; j++)
		{
			if(gridArray[i][j] != NULL)
			{
				gridArray[i][j]->MarkForDestroy();
				gridArray[i][j]= NULL;
			}
		}
	}
}

void SpawningGrid::Destroy()
{
	//ClearGrid();
}

void SpawningGrid::IncreaseMushInPlayerArea()
{
	mush_in_player_area++;
}

void SpawningGrid::DecreaseMushInPlayerArea()
{
	mush_in_player_area--;
}