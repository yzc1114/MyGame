#include "AudioControl.h"
#include "SimpleAudioEngine.h"

DECLARE_SINGLETON_MEMBER(AudioControl)

AudioControl::AudioControl()
{
}


AudioControl::~AudioControl()
{
}

bool AudioControl::PreloadMusic()
{
	//音频文件的预加载
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/StartBGM.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/DeathBGM.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/VictoryBGM.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/STARTBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/ButtonBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/ContactBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/FightingBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/ItemBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/KeyBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/OpenBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("music/TransportBGS.mp3");


	return true;
}
