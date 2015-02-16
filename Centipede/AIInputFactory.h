#ifndef _AIInputFactory
#define _AIInputFactory

#include <stack>

class AIInput;
class Blaster;

class AIInputFactory
{
private:
	static std::stack<AIInput*> recycledItems;

	static AIInputFactory *self;

	AIInputFactory(){ };
	AIInputFactory(const AIInputFactory&){ };
	AIInputFactory& operator= (const AIInputFactory&){ };

	~AIInputFactory(){ };
	
	static AIInputFactory& Instance()
	{
		if (!self)
			self= new AIInputFactory();
		return *self;
	}

public:
	static AIInput* CreateAIInput(Blaster* blast);

	static void RecycleAIInput(GameObject* k);

	static void Empty();
};


#endif