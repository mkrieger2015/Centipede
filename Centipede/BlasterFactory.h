#ifndef _BlasterFactory
#define _BlasterFactory

#include <stack>

class Blaster;

class BlasterFactory
{
private:
	static std::stack<Blaster*> recycledItems;

	static BlasterFactory *self;

	BlasterFactory(){ };
	BlasterFactory(const BlasterFactory&){ };
	BlasterFactory& operator= (const BlasterFactory&){ };

	~BlasterFactory(){ };
	
	static BlasterFactory& Instance()
	{
		if (!self)
			self= new BlasterFactory();
		return *self;
	}

public:
	static Blaster* CreateBlaster(bool mode);

	static void RecycleBlaster(GameObject* b);

	static void Empty();
};


#endif _BlasterFactory
