#ifndef _KeyboardInputFactory
#define _KeyboardInputFactory

#include <stack>

class KeyboardInput;
class Blaster;

class KeyboardInputFactory
{
private:
	static std::stack<KeyboardInput*> recycledItems;

	static KeyboardInputFactory *self;

	KeyboardInputFactory(){ };
	KeyboardInputFactory(const KeyboardInputFactory&){ };
	KeyboardInputFactory& operator= (const KeyboardInputFactory&){ };

	~KeyboardInputFactory(){ };
	
	static KeyboardInputFactory& Instance()
	{
		if (!self)
			self= new KeyboardInputFactory();
		return *self;
	}

public:
	static KeyboardInput* CreateKeyboardInput(Blaster* blast);

	static void RecycleKeyboardInput(GameObject* k);

	static void Empty();
};


#endif _KeyboardInputFactory
