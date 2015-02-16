#ifndef _HUDManager
#define _HUDManager

#include "Game Components/TEAL/CommonElements.h"
#include <list>
#include "GameFont.h"
#include <queue>

class HUDManager : public GameObject
{
private:
	static HUDManager *self;

	HUDManager(){ };
	HUDManager(const HUDManager&){ };
	HUDManager& operator= (const HUDManager&){ };

	~HUDManager(){ };
	
	static HUDManager& Instance()
	{
		if (! self)
			self= new HUDManager();
		return *self;
	};
	virtual void Draw();
	void InternalRegistration();

	struct highscore
	{
		std::string user_name;
		int score;
	};

	std::queue<highscore> high_score_queue;

	GameFont myfont;
	std::list<Glyph> p1_score;
	std::list<Glyph> p2_score;
	std::list<Glyph> high_score;
	std::list<Glyph> *current_score;
	std::list<Glyph> high_score_text;
	sf::Vector2f high_score_nextpos;
	std::list<Glyph> hs[8];
	std::list<Glyph> credits_2;
	std::list<Glyph> bonus_every_12000;
	AnimatedSprite blaster_sprite;
	sf::Vector2f p1_nextpos;
	sf::Vector2f p2_nextpos;
	sf::Vector2f hs_nextpos;
	sf::Vector2f *current_nextpos;
	float p1_scorepos;
	float p2_scorepos;
	float *current_scorepos;


public:
	static void Instantiate();
	static void UpdatePlayerScore(int new_score);
	static void UpdateHighScore(int new_score);
	static void SwapPlayerHUDs();
	static void TitleScreenOn();
	static void TitleScreenOff();
	static void Delete();
};

#endif _HUDManager