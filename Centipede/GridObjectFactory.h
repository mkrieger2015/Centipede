#ifndef _GridObjectFactory
#define _GridObjectFactory

#include <stack>

class GridObject;

class GridObjectFactory
{
private:
	static std::stack<GridObject*> recycledItems;

	static GridObjectFactory *self;

	GridObjectFactory(){ };
	GridObjectFactory(const GridObjectFactory&){ };
	GridObjectFactory& operator= (const GridObjectFactory&){ };

	~GridObjectFactory(){ };
	
	static GridObjectFactory& Instance()
	{
		if (!self)
			self= new GridObjectFactory();
		return *self;
	}

public:
	static GridObject* CreateGridObject();

	static void RecycleGridObject(GameObject* m);

	static void Empty();
};


#endif _GridObjectFactory
