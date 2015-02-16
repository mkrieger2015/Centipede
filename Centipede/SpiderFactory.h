#ifndef _SpiderFactory
#define _SpiderFactory

#include <stack>

class Spider;
class Blaster;

class SpiderFactory
{
private:
	static std::stack<Spider*> recycledItems;

	static SpiderFactory *self;

	SpiderFactory(){ };
	SpiderFactory(const SpiderFactory&){ };
	SpiderFactory& operator= (const SpiderFactory&){ };

	~SpiderFactory(){ };
	
	static SpiderFactory& Instance()
	{
		if (!self)
			self= new SpiderFactory();
		return *self;
	};

public:
	static Spider* CreateSpider();

	static void RecycleSpider(GameObject* s);  

	static void Empty();

};

#endif _SpiderFactory
