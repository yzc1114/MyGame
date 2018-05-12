#ifndef _CONTROLLAYER_
#define _CONTROLLAYER_

#include"MagicTower.h"

USING_NS_CC;
class ControlLayer : public Layer {
public:
	ControlLayer();
	~ControlLayer();

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

	bool MouseGoesAway;

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

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* unused_event);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* unused_event);

	void moveUpUpdate(float dt);
	void moveDownUpdate(float dt);
	void moveLeftUpdate(float dt);
	void moveRightUpdate(float dt);

};

#endif