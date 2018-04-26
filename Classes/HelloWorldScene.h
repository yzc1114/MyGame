#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	
	
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	bool keyUpHolding = false;
	bool keyDownHolding = false;
	bool keyLeftHolding = false;
	bool keyRightHolding = false;

	void up(Ref* psender);
	void down(Ref* psender);
	void left(Ref* psender);
	void right(Ref* psender);

	void keyup(float dt);
	void keydown(float dt);
	void keyright(float dt);
	void keyleft(float dt);

	
	bool isCanReach(float x,float y);
	

	
private:
	Size size;
	TMXTiledMap* map;
	TMXLayer* meta;
	Sprite* littleman;

	
};

#endif // __HELLOWORLD_SCENE_H__
