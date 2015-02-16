#ifndef _ScorpionFactory
#define _ScorpionFactory

#include <stack>

class Scorpion;

class ScorpionFactory
{
private:
	static std::stack<Scorpion*> recycledItems;

	static ScorpionFactory *self;

	ScorpionFactory(){ };
	ScorpionFactory(const ScorpionFactory&){ };
	ScorpionFactory& operator= (const ScorpionFactory&){ };

	~ScorpionFactory(){ };
	
	static ScorpionFactory& Instance()
	{
		if (!self)
			self= new ScorpionFactory();
		return *self;
	};

public:
	static Scorpion* CreateScorpion();

	static void RecycleScorpion(GameObject* s);  

	static void Empty();

};

#endif _ScorpionFactory
