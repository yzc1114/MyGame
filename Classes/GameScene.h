#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include"cocos2d.h"
#include"GameLayer.h"
#include"ControlLayer.h"
#include"GameConstants.h"
USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

	

	void switchMap();


















































	/*Á·Ï°

	//bool keyUpHolding = false;
	//bool keyDownHolding = false;
	//bool keyLeftHolding = false;
	//bool keyRightHolding = false;

	void up(Ref* psender);
	void down(Ref* psender);
	void left(Ref* psender);
	void right(Ref* psender);

	void keyup(float dt);
	void keydown(float dt);
	void keyright(float dt);
	void keyleft(float dt);

	
	bool isCanReach(float x,float y);
	*/

	
private:



	/*Á·Ï°
	Size size;
	TMXTiledMap* map;
	TMXLayer* meta;
	
	Sprite* littleman;
	bool isMoving = false;
	*/
};

#endif 
