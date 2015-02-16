#ifndef _GameFont
#define _GameFont

#include "SpriteSheet.h"

class GameFont : public SpriteSheet
{
public:
	GameFont();

private:
	virtual int CharToIndex( char& c ); 
};


#endif _GameFont