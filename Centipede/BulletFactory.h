#ifndef _BulletFactory
#define _BulletFactory

#include <stack>
#include "Bullet.h"

class Blaster;

class BulletFactory
{
private:
	static BulletFactory *self;

	BulletFactory(){ };
	BulletFactory(const BulletFactory&){ };
	BulletFactory& operator= (const BulletFactory&){ };

	~BulletFactory();
	
	static BulletFactory& Instance()
	{
		if (! self)
			self= new BulletFactory();
		return *self;
	};

	static std::stack<Bullet*> recycledItems;
	

public:
	static Bullet* CreateBullet(float xpos,float ypos);

	static void RecycleBullet(GameObject* b);  

	static void ReloadPlayer();

	static void Empty();
};


#endif _BulletFactory