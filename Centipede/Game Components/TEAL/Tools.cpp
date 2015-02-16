// Tools.h
// Andre Berthiaume, June 2012
//
// a list of convenient functions used all over

#include "Tools.h"

const float Tools::PI = 3.14159f;

/// \brief Locks up and waits for the release of key k. 
///
/// \param k The sf::Keyboard::Key to process.
void Tools::WaitForRelease(sf::Keyboard::Key k)
{
	while ( sf::Keyboard::isKeyPressed( k ) ) 
				;
}


