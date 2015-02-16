#ifndef _BodyFactory
#define _BodyFactory

#include <stack>

class Body;
class Centipede;

class BodyFactory
{
private:
	static std::stack<Body*> recycledItems;

	static BodyFactory *self;

	BodyFactory(){ };
	BodyFactory(const BodyFactory&){ };
	BodyFactory& operator= (const BodyFactory&){ };

	~BodyFactory(){ };
	
	static BodyFactory& Instance()
	{
		if (!self)
			self= new BodyFactory();
		return *self;
	};

public:
	static Centipede* CreateBody(float xpos, float ypos, int num_segments, Centipede *prev);

	static void RecycleBody(GameObject* b);  

	static void Empty();

};

#endif _BodyFactory