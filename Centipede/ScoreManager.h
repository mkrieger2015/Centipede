#ifndef _ScoreManager
#define _ScoreManager

#include "Game Components\TEAL\CommonElements.h"
#include <list>

class ScoreManager : public GameObject
{
private:
	static ScoreManager *self;

	ScoreManager(){ };
	ScoreManager(const ScoreManager&){ };
	ScoreManager& operator= (const ScoreManager&){ };

	~ScoreManager(){ };
	
	static ScoreManager& Instance()
	{
		if (!self)
			self= new ScoreManager();
		return *self;
	};

	struct high_score
	{
		std::string user_name;
		int score;
	};

	std::list<high_score> high_score_master;
	std::list<high_score> high_score_queue;
	

	int p1_points;
	int p2_points;
	int ai_points;
	int high_score_points;
	int *current_points;

	virtual void Update();

	sf::Font ScoreFont;

	std::string Label;
	sf::Color ScoreColor;
	sf::Text ScoreDisplay;

	int p1_points_to_1up;
	int p2_points_to_1up;
	int* current_points_to_1up;

public:
	static void Instantiate();
	static void AddPoints(int point_value);
	static void SwapPlayerPoints();
	static void LoadHighScores();
	static std::string SendUserName();
	static int SendScore();
	static void CopyHighScoreList();
	static void PopHighScoreList();
	static int ReturnP1Score(){ return Instance().p1_points;};
	static int ReturnP2Score(){ return Instance().p2_points;};
	static int ReturnMinHighScore();
	static void InsertHighScore(std::string input, int score);
};

#endif _WaveManager