#ifndef _AUDIOCONTROL_
#define _AUDIOCONTROL_

#include"cocos2d.h"
#include"Singleton.h"
USING_NS_CC;
class AudioControl : public Singleton<AudioControl>
{
public:
	AudioControl();
	~AudioControl();

	bool PreloadMusic();
};

#endif