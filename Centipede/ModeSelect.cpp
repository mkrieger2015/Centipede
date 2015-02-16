#include "ModeSelect.h"
#include "GameManager.h"


ModeSelect::ModeSelect()
{
	RegisterInput( InputFlags::KeyPressed );
}

void ModeSelect::KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{
	//simply a temp object that takes the 1 or 2 keys for 1 player or 2 player mode at the title screen
	//no recycling, because it only appears once per game
	if ( k == sf::Keyboard::Num1 )
	{
		GameManager::OnePlayerGame();
		MarkForDestroy();
	}
	else if ( k == sf::Keyboard::Num2 )
	{
		GameManager::TwoPlayerGame();
		MarkForDestroy();
	}

}

void ModeSelect::Destroy()
{
	DeregisterInput();
}