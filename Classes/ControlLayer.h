#ifndef _CONTROLLAYER_
#define _CONTROLLAYER_

#include"MagicTower.h"

USING_NS_CC;
class ControlLayer : public Layer {
public:
	ControlLayer();
	~ControlLayer();

	typedef enum {
		up = 1,
		down,
		left,
		right,
		normal
	}Direction;

	CREATE_FUNC(ControlLayer);
private:

	virtual bool init();

	

	//普通状态的方向按钮
	Sprite* directionControl;
	//方向按钮四个方向的触发四边形
	Rect UpPart;
	Rect DownPart;
	Rect LeftPart;
	Rect RightPart;
	//方向按钮四个方向的精灵图片
	Sprite* TouchRight;
	Sprite* TouchLeft;
	Sprite* TouchUp;
	Sprite* TouchDown;
	//鼠标按下后是否移开方向按钮的触发区域 若移开 则不进行移动
	bool MouseGoesAway;
	//退出按钮的目录按钮
	Menu* exitButton;
	//退出按钮的回调函数
	void exitButtonCallBack(Ref* psender);
	//点击事件的三个回调函数
	bool onTouchBegan(Touch* touch, Event* unused_Event);
	void onTouchMoved(Touch* touch, Event* unused_Event);
	void onTouchEnded(Touch* touch, Event* unused_Event);
	//键盘事件的回调函数
	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* unused_event);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* unused_event);
	//四个方向移动的计时器函数
	void moveUpUpdate(float dt);
	void moveDownUpdate(float dt);
	void moveLeftUpdate(float dt);
	void moveRightUpdate(float dt);

};

#endif