// AnimatedSprite
// Andre Berthiaume, Aug 2012
// Simpe animated sprite class.
// Modified from Laurent Gomila's SpriteAnimated class https://github.com/LaurentGomila/SFML/wiki/SpriteAnimated

#ifndef _AnimatedSprite
#define _AnimatedSprite

#include "SFML\Graphics.hpp"

class AnimatedSprite : public sf::Sprite
{
public:
	AnimatedSprite(){}; 
		
	//initialtion list 
	AnimatedSprite(sf::Texture& Text, int FrameCountWidth, int FrameCountHeight, float FramesPerSecond = 60, int FirstFrame = 0,  int LastFrame = 0, bool IsAnimated = true, bool IsLooped = true);
	~AnimatedSprite(){};

	int GetFrameCount();

	//Sets current frame
	void SetFrame(int frame);
	int GetFrame();
	bool IsLastAnimationFrame();
	//sets loop speed in fps
	void SetLoopSpeed(float fps);
	void NextFrame();
	float GetLoopSpeed();
	void SetAnimation(int FirstFrame, int LastFrame, bool IsAnimated = true, bool IsLooped = true);

	void Play();
	void Pause();
	void Reset();
	void Update();
private:
	int _frameWidth, _frameHeight, _frameCount, _currentFrame, _firstFrame, _lastFrame;
	float _fps, _clock, _interval;
    bool _isAnimated;
    bool _isLooped;

	sf::IntRect GetFramePosition(int frame);
	void SetFrameSize(int frameWidth, int frameHeight);
};

#endif _AnimatedSprite