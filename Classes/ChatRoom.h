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
		kZInputBoxBGLayer,
		kZEditBox,
		kZTextBoxBGLayer
	};
	virtual bool onTouchBegan(Touch* touch, Event* ev);
	void sendMsg();
	void createTextField();
private:
	TextFieldTTF* textEdit;
	Layer* BGLayer;
};

#endif
