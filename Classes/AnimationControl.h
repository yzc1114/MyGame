#ifndef _ANIMATIONCONTROL_
#define _ANIMATIONCONTROL_

#include"MagicTower.h"
USING_NS_CC;



class AnimationControl : public Singleton<AnimationControl>
{
public:
	AnimationControl();
	~AnimationControl();

	//初始化动画模版缓存表
	bool initAnimationMap();

	//根据名字得到一个动画模板
	Animation* getAnimation(int key);

	//创建一个动画实例
	Animate* createAnimate(int key);

	//创建一个动画实例
	Animate* createAnimate(const char* key);

protected:
	//加载勇士行走动画模版
	Animation * createHeroMovingAnimationByDirection(HeroDirection direction);

	Animation * createNPCAnimationByImageID(int ID);

	Animation * createFightingAnimation();
	
};

#endif