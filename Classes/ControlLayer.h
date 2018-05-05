#ifndef _CONTROLLAYER_
#define _CONTROLLAYER_

#include"cocos2d.h"
#include"GameConstants.h"
#include"StartScene.h"
USING_NS_CC;
class ControlLayer : public Layer {
public:
	Layer * controllayer;

	virtual bool init();

	CREATE_FUNC(ControlLayer);

	Sprite* directionControl;

	Rect UpPart;
	Rect DownPart;
	Rect LeftPart;
	Rect RightPart;


	Sprite* TouchRight;
	Sprite* TouchLeft;
	Sprite* TouchUp;
	Sprite* TouchDown;

	typedef enum {
		up = 1,
		down,
		left,
		right,
		normal
	}Direction;

	Menu* exitButton;


	void exitButtonCallBack(Ref* psender);

	bool onTouchBegan(Touch* touch, Event* unused_Event);
	void onTouchMoved(Touch* touch, Event* unused_Event);
	void onTouchEnded(Touch* touch, Event* unused_Event);
};

#endif