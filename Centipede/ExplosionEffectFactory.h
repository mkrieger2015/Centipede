#ifndef _ExplosionEffectFactory
#define _ExplosionEffectFactory

#include <stack>

class ExplosionEffect; 

class ExplosionEffectFactory
{
private:
	static std::stack<ExplosionEffect*> recycledItems;

	static ExplosionEffectFactory *self;

	ExplosionEffectFactory(){ };
	ExplosionEffectFactory(const ExplosionEffectFactory&){ };
	ExplosionEffectFactory& operator= (const ExplosionEffectFactory&){ };

	~ExplosionEffectFactory(){ };
	
	static ExplosionEffectFactory& Instance()
	{
		if (!self)
			self= new ExplosionEffectFactory();
		return *self;
	}

public:
	static ExplosionEffect* CreateExplosionEffect(float xpos, float ypos);

	static void RecycleExplosionEffect(GameObject* e);

	static void Empty();

};


#endif

