#ifndef _CHATROOM_
#define _CHATROOM_

#include "MagicTower.h"
USING_NS_CC;
class ChatRoom : public Scene
{
public:
	
	virtual bool init();
	CREATE_FUNC(ChatRoom);
	enum {
		kZBGLayer,
		kZMSGBox,
		kZBarrier1,
		kZBarrier2,
		kZInputBoxBGLayer,
		kZEditBox,
		
	};
	virtual bool onTouchBegan(Touch* touch, Event* ev);
	void sendMsg();
	void createTextField();
	Label* createMSGLabel(std::string str);
	void addLabel(Label* label);
	void rollUpWhenNewMsg();
private:
	TextFieldTTF* textEdit;
	Layer* BGLayer;
	Layer* Barrier1;
	Layer* Barrier2;
	std::queue<Label*> LabelQueue;
};

#endif
