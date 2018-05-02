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

	//加载勇士向下走的动画
	sprintf(temp, "%d", kdown);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kdown), temp);

	//加载勇士向右走的动画
	sprintf(temp, "%d", kright);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kright), temp);

	//加载勇士向左走的动画
	sprintf(temp, "%d", kleft);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kleft), temp);

	//加载勇士向上走的动画
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
	//获取指定动画模版
	Animation* anim = getAnimation(key);

	if (anim == NULL)
	{
		return NULL;
	}

	//根据动画模版生成一个动画实例
	return Animate::create(anim);
}

Animate * AnimationControl::createAnimate(const char * key)
{
	//获取指定动画模版
	Animation* anim = AnimationCache::getInstance()->animationByName(key);

	if (anim == NULL)
	{
		return NULL;
	}

	//根据动画模版生成一个动画实例
	return Animate::create(anim);
}

Animation * AnimationControl::createHeroMovingAnimationByDirection(HeroDirection direction)
{
	Texture2D *heroTexture = TextureCache::getInstance()->addImage("hero.png");

	//第二个参数表示显示区域的x, y, width, height，根据direction来确定显示的y坐标
	SpriteFrame *frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 0, 32 * direction, 32, 32));
	SpriteFrame *frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 1, 32 * direction, 32, 32));
	SpriteFrame *frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 2, 32 * direction, 32, 32));
	SpriteFrame *frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 3, 32 * direction, 32, 32));

	Vector<SpriteFrame*> animFrames;

	animFrames.pushBack(frame0);
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);
	animFrames.pushBack(frame3);

	//0.05f表示每帧动画间的间隔
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.05f);

	return animation;
}
