#ifndef _ExplosionEffect
#define _ExplosionEffect

#include "Game Components\TEAL\CommonElements.h"

class ExplosionEffect : public GameObject
{
public:
	ExplosionEffect(){ };
	void Initialize(float xpos, float ypos);
	virtual void Draw();
	virtual void Update();

private:
	int life_iterator;
	AnimatedSprite MainSprite;
	sf::Vector2f Pos;

};
#endif 