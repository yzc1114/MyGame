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

	//����NPC����
	AnimationCache::getInstance()->addAnimation(createNPCAnimationByImageID(0), std::string("npc" + std::to_string(0)));

	//����ս������
	AnimationCache::getInstance()->addAnimation(createFightingAnimation(), "Fighting");



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

Animation * AnimationControl::createNPCAnimationByImageID(int ID)
{
	std::string ImagePath("npc" + std::to_string(ID) + ".png");
	Texture2D *heroTexture = TextureCache::getInstance()->addImage(ImagePath);

	//�ڶ���������ʾ��ʾ�����x, y, width, height������direction��ȷ����ʾ��y����
	SpriteFrame *frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 0, 0, 32, 32));
	SpriteFrame *frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 1, 0, 32, 32));
	SpriteFrame *frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 2, 0, 32, 32));
	SpriteFrame *frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32 * 3, 0, 32, 32));

	Vector<SpriteFrame*> animFrames;

	animFrames.pushBack(frame0);
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);
	animFrames.pushBack(frame3);

	//0.2f��ʾÿ֡������ļ��
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);

	return animation;
}

Animation * AnimationControl::createFightingAnimation()
{
	std::string ImagePath("sword.png");
	Texture2D *fightingTexture = TextureCache::getInstance()->addImage(ImagePath);

	SpriteFrame *frame0 = SpriteFrame::createWithTexture(fightingTexture, Rect(192*0, 192*0, 192, 192));
	SpriteFrame *frame1 = SpriteFrame::createWithTexture(fightingTexture, Rect(192*1, 192*1, 192, 192));
	SpriteFrame *frame2 = SpriteFrame::createWithTexture(fightingTexture, Rect(192*2, 192*2, 192, 192));
	SpriteFrame *frame3 = SpriteFrame::createWithTexture(fightingTexture, Rect(192*3, 192*3, 192, 192));
	SpriteFrame *frame4 = SpriteFrame::createWithTexture(fightingTexture, Rect(192*0, 192*4, 192, 192));

	Vector<SpriteFrame*> Frames;

	Frames.pushBack(frame0);
	Frames.pushBack(frame1);
	Frames.pushBack(frame2);
	Frames.pushBack(frame3);
	Frames.pushBack(frame4);

	Animation* animation = Animation::createWithSpriteFrames(Frames, 0.15f);

	return animation;
}
