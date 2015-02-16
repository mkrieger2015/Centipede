#ifndef _HeadFactory
#define _HeadFactory

#include <stack>
#include <queue>
#include "Centipede.h"

class Head;

class HeadFactory
{
private:
	static std::stack<Head*> recycledItems;

	static HeadFactory *self;

	HeadFactory(){ };
	HeadFactory(const HeadFactory&){ };
	HeadFactory& operator= (const HeadFactory&){ };

	~HeadFactory(){ };
	
	static HeadFactory& Instance()
	{
		if (! self)
			self= new HeadFactory();
		return *self;
	};
	
public:
	static void CreateHead(float xpos,float ypos,bool initial_direction,int segments);
	static void TransformHead(float xpos, float ypos, bool direction, std::queue<Centipede::orders> in_queue, std::queue<Centipede::orders> old_out, Centipede *new_next);
	static void RecycleHead(GameObject* c);  
	static void Empty(); 
};


#endif _BulletFactory