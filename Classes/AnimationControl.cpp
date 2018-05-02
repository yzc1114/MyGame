#include "AnimationControl.h"

DECLARE_SINGLETON_MEMBER(AnimationControl);

AnimationControl::AnimationControl()
{
}


AnimationControl::~AnimationControl()
{
}

bool AnimationControl::initAnimationMap()
{
	char temp[20];

	//������ʿ�����ߵĶ���
	sprintf(temp, "%d", kdown);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kdown), temp);

	//������ʿ�����ߵĶ���
	sprintf(temp, "%d", kright);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kright), temp);

	//������ʿ�����ߵĶ���
	sprintf(temp, "%d", kleft);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kleft), temp);

	//������ʿ�����ߵĶ���
	sprintf(temp, "%d", kup);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kup), temp);

	

	return true;
}

Animation * AnimationControl::getAnimation(int key)
{
	char temp[20];
	sprintf(temp, "%d", key);
	return AnimationCache::getInstance()->animationByName(temp);
}

Animate * AnimationControl::createAnimate(int key)
{
	//��ȡָ������ģ��
	Animation* anim = getAnimation(key);

	if (anim == NULL)
	{
		return NULL;
	}

	//���ݶ���ģ������һ������ʵ��
	return Animate::create(anim);
}

Animate * AnimationControl::createAnimate(const char * key)
{
	//��ȡָ������ģ��
	Animation* anim = AnimationCache::getInstance()->animationByName(key);

	if (anim == NULL)
	{
		return NULL;
	}

	//���ݶ���ģ������һ������ʵ��
	return Animate::create(anim);
}

Animation * AnimationControl::createHeroMovingAnimationByDirection(HeroDirection direction)
{
	Texture2D *heroTexture = TextureCache::getInstance()->addImage("hero.png");

	//�ڶ���������ʾ��ʾ�����x, y, width, height������direction��ȷ����ʾ��y����
	SpriteFrame *frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 0, 32 * direction, 32, 32));
	SpriteFrame *frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 1, 32 * direction, 32, 32));
	SpriteFrame *frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 2, 32 * direction, 32, 32));
	SpriteFrame *frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 3, 32 * direction, 32, 32));

	Vector<SpriteFrame*> animFrames;

	animFrames.pushBack(frame0);
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);
	animFrames.pushBack(frame3);

	//0.05f��ʾÿ֡������ļ��
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.05f);

	return animation;
}