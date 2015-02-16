#ifndef _FleaFactory
#define _FleaFactory

#include <stack>

class Flea;

class FleaFactory
{
private:
	static std::stack<Flea*> recycledItems;

	static FleaFactory *self;

	FleaFactory(){ };
	FleaFactory(const FleaFactory&){ };
	FleaFactory& operator= (const FleaFactory&){ };

	~FleaFactory(){ };
	
	static FleaFactory& Instance()
	{
		if (!self)
			self= new FleaFactory();
		return *self;
	};

public:
	static Flea* CreateFlea();

	static void RecycleFlea(GameObject* f);  

	static void Empty();

};

#endif _FleaFactory