#include "AIInput.h"
#include "Blaster.h"
#include "PlayerManager.h"
#include "CommonFunctionsAndValues.h"

extern const int ai_minlength;
extern const int ai_maxlength;
extern const float y_upperbound;
extern const float y_lowerbound;

AIInput::AIInput()
{
	player= NULL;
	iterator= 0;
	up= 0;
	down= 0;
	left= 0;
	right= 0;
}

AIInput::AIInput(Blaster *blast)
{
	player= blast;
	iterator= 0;
	up= 0;
	down= 0;
	left= 0;
	right= 0;
}

void AIInput::MoveUpLeft()
{
	iterator= (rand() % ai_maxlength) + ai_minlength;
	up= 1;
	down= 0;
	left= 1;
	right= 0;
}

void AIInput::MoveUpRight()
{
	iterator= (rand() % ai_maxlength) + ai_minlength;
	up= 1;
	down= 0;
	left= 0;
	right= 1;
}

void AIInput::MoveDownLeft()
{
	iterator= (rand() % ai_maxlength) + ai_minlength;
	up= 0;
	down= 1;
	left= 1;
	right= 0;
}

void AIInput::MoveDownRight()
{
	iterator= (rand() % ai_maxlength) + ai_minlength;
	up= 0;
	down= 1;
	left= 0;
	right= 1;
}

void AIInput::StandStill()
{
	iterator= (rand() % ai_maxlength) + ai_minlength;
	up= 0;
	down= 0;
	left= 0;
	right= 0;
}

void AIInput::Update()
{
	//constantly firing
	player->Fire();
	//read in player y
	float player_y= PlayerManager::ReturnPlayerY();
	//if the player is almost below bounds, only set values for an upward movement or stand still
	if (player_y >= y_lowerbound)
	{
		int up_picker= rand() %3;
		if(up_picker == 0)
			MoveUpLeft();
		else if(up_picker == 1)
			MoveUpRight();
		else if(up_picker == 2)
			StandStill();
	}
	//if the player is almost above bounds, only set values for a downward movement or stand still
	else if (player_y <= y_upperbound)
	{
		int down_picker= rand() %3;
		if(down_picker == 0)
			MoveDownLeft();
		else if(down_picker == 1)
			MoveDownRight();
		else if (down_picker == 2)
			StandStill();
	}

	//if the iterator (which stores # of moves) is at zero, roll a random and then pick one of 5 possible moves
	if (iterator == 0)
	{
		int move_picker= rand() %5;
		if(move_picker == 0)
			MoveUpLeft();
		else if(move_picker == 1)
			MoveUpRight();
		else if(move_picker == 2)
			MoveDownLeft();
		else if(move_picker == 3)
			MoveDownRight();
		else if(move_picker == 4)
			StandStill();
	}
	//if the iterator is greater than zero, call all four movement events in the player
	//HOWEVER, this is where the parameter into the player movement events comes into play
	//to avoid constant checking on behalf of the player, this system uses 1's and 0's to control which direction the player will be moving in
	//if a 0 is passed into one of these movement callbacks, no movement will occur
	//if a 1 is passed in, movement will occur
	//which recieves a 0 and which recieves a 1 are updated in the movement events within this object (see them above)
	if(iterator > 0)
	{
		player->Up(up);
		player->Down(down);
		player->Left(left);
		player->Right(right);
		iterator--;
	}


}
