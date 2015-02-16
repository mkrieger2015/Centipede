#ifndef _PoisonMushroomFactory
#define _PoisonMushroomFactory

#include <stack>

class PoisonMushroom;

class PoisonMushroomFactory
{
private:
	static std::stack<PoisonMushroom*> recycledItems;

	static PoisonMushroomFactory *self;

	PoisonMushroomFactory(){ };
	PoisonMushroomFactory(const PoisonMushroomFactory&){ };
	PoisonMushroomFactory& operator= (const PoisonMushroomFactory&){ };

	~PoisonMushroomFactory(){ };
	
	static PoisonMushroomFactory& Instance()
	{
		if (!self)
			self= new PoisonMushroomFactory();
		return *self;
	}

public:
	static PoisonMushroom* CreatePoisonMushroom(float xpos, float ypos);
	static PoisonMushroom* ConvertPoisonMushroom(float xpos, float ypos, int prev_health);

	static void RecyclePoisonMushroom(GameObject* m);

	static void Empty();
};


#endif _PoisonMushroomFactory