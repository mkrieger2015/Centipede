// AnimatedSprite
// Andre Berthiaume, Aug 2012
// Simpe animated sprite class.
// Original source: docflabby's AniSprite class https://github.com/LaurentGomila/SFML/wiki/SpriteAnimated

#include "AnimatedSprite.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(sf::Texture& Text, int FrameCountWidth, int FrameCountHeight, float FramesPerSecond, int FirstFrame, int LastFrame, bool IsAnimated, bool IsLooped)
{
	setTexture(Text, true);
    _fps = FramesPerSecond;
    _interval = 1.0f / _fps;
    _frameWidth = Text.getSize().x / FrameCountWidth;
    _frameHeight = Text.getSize().y / FrameCountHeight;
    _frameCount = LastFrame - FirstFrame;
    _firstFrame = FirstFrame;
    _currentFrame = FirstFrame;
    _lastFrame = LastFrame;
    _isAnimated = IsAnimated;
    _isLooped = IsLooped;
    _clock = 0;
    setTextureRect( GetFramePosition(_currentFrame) );	
}

sf::IntRect AnimatedSprite::GetFramePosition(int frame)
{
    int WCount = (int) this->getTexture()->getSize().x / _frameWidth;
    int XPos = frame % WCount;
    int YPos = frame / WCount;

    sf::IntRect Position = sf::IntRect(_frameWidth * XPos, _frameHeight * YPos, _frameWidth, _frameHeight);
    return Position;
}

void AnimatedSprite::NextFrame()
{
    if (_currentFrame < _lastFrame)
        _currentFrame++;
    else
        _currentFrame = _firstFrame;

	setTextureRect(GetFramePosition(_currentFrame));
}

void AnimatedSprite::Update()
{
    _clock += Game::FrameTime();

	//TEALShow::Text("clock = " + Tools::ToString(_clock));

    if (_isAnimated && _clock >= _interval)
    {
		NextFrame();
        _clock = 0;
    }

    if (!_isLooped & (_currentFrame == _lastFrame))
    {
        _isAnimated = false;
    }
}

void AnimatedSprite::SetLoopSpeed(float fps)
{
	_fps = fps;
	_interval = 1.0f / _fps;
}

void AnimatedSprite::Play()
{
	_isAnimated = true;
}

void AnimatedSprite::Pause()
{
    _isAnimated = false;
}

void AnimatedSprite::Reset()
{
    _isAnimated = false;
    _currentFrame = 0;
    setTextureRect( sf::IntRect(0, 0, _frameWidth, _frameHeight) );
}

void AnimatedSprite::SetAnimation(int FirstFrame, int LastFrame, bool IsAnimated, bool IsLooped)
{
    _firstFrame = FirstFrame;
    _lastFrame = LastFrame;
	_currentFrame =_firstFrame;
    _isAnimated = IsAnimated;
    _isLooped = IsLooped;

    _frameCount = (LastFrame + 1) - FirstFrame;
    //if (!IsAnimated)  // AB: I'm not seeing why this condition is there...
    {
        setTextureRect(GetFramePosition(FirstFrame));
    }
}

void AnimatedSprite::SetFrame(int Frame)
{
    _currentFrame = Frame;
    _isAnimated = true;
    _isLooped = false;

	setTextureRect(GetFramePosition(_currentFrame));
}

int AnimatedSprite::GetFrame()
{
	return _currentFrame;
}

bool AnimatedSprite::IsLastAnimationFrame()
{
	return _currentFrame == _lastFrame;
}
