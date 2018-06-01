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

	

	//��ͨ״̬�ķ���ť
	Sprite* directionControl;
	//����ť�ĸ�����Ĵ����ı���
	Rect UpPart;
	Rect DownPart;
	Rect LeftPart;
	Rect RightPart;
	//����ť�ĸ�����ľ���ͼƬ
	Sprite* TouchRight;
	Sprite* TouchLeft;
	Sprite* TouchUp;
	Sprite* TouchDown;
	//��갴�º��Ƿ��ƿ�����ť�Ĵ������� ���ƿ� �򲻽����ƶ�
	bool MouseGoesAway;
	//�˳���ť��Ŀ¼��ť
	Menu* exitButton;
	//�˳���ť�Ļص�����
	void exitButtonCallBack(Ref* psender);
	//����¼��������ص�����
	bool onTouchBegan(Touch* touch, Event* unused_Event);
	void onTouchMoved(Touch* touch, Event* unused_Event);
	void onTouchEnded(Touch* touch, Event* unused_Event);
	//�����¼��Ļص�����
	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* unused_event);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* unused_event);
	//�ĸ������ƶ��ļ�ʱ������
	void moveUpUpdate(float dt);
	void moveDownUpdate(float dt);
	void moveLeftUpdate(float dt);
	void moveRightUpdate(float dt);

};

#endif