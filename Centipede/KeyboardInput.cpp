#include "KeyboardInput.h"
#include "Blaster.h"


KeyboardInput::KeyboardInput()
{
	player= NULL;
	RegisterInput( InputFlags::KeyPressed );
}

KeyboardInput::KeyboardInput(Blaster *blast)
{
	player= blast;
	RegisterInput( InputFlags::KeyPressed );
}

void KeyboardInput::Update()
{
	// Continuous key-down tests
	// each key has proper callback function to the player, passing in a 1 so that movement actually occurs
	//for more information on the 1 (vs 0), see the AIInput 
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left)) player->Left(1);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right)) player->Right(1);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up)) player->Up(1);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down)) player->Down(1);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space)) player->Fire();
}

void KeyboardInput::Destroy()
{
	DeregisterInput();
}