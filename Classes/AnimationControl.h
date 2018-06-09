#ifndef _ANIMATIONCONTROL_
#define _ANIMATIONCONTROL_

#include"MagicTower.h"
USING_NS_CC;



class AnimationControl : public Singleton<AnimationControl>
{
public:
	AnimationControl();
	~AnimationControl();

	int npcAnimationId = 26;

	//��ʼ������ģ�滺���
	bool initAnimationMap();

	//�������ֵõ�һ������ģ��
	Animation* getAnimation(int key);

	//����һ������ʵ��
	Animate* createAnimate(int key);

	//����һ������ʵ��
	Animate* createAnimate(const char* key);

protected:
	//������ʿ���߶���ģ��
	Animation * createHeroMovingAnimationByDirection(HeroDirection direction);

	Animation * createNPCAnimationByImageID(int ID);

	Animation * createFightingAnimation();
	
};

#endif