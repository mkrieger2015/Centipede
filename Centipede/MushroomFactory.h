#ifndef _MushroomFactory
#define _MushroomFactory

#include <stack>

class Mushroom;

class MushroomFactory
{
private:
	static std::stack<Mushroom*> recycledItems;

	static MushroomFactory *self;

	MushroomFactory(){ };
	MushroomFactory(const MushroomFactory&){ };
	MushroomFactory& operator= (const MushroomFactory&){ };

	~MushroomFactory(){ };
	
	static MushroomFactory& Instance()
	{
		if (!self)
			self= new MushroomFactory();
		return *self;
	}

public:
	static Mushroom* CreateMushroom(float xpos, float ypos);

	static void RecycleMushroom(GameObject* m);

	static void Empty();
};


#endif _MushroomFactory
