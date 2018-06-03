#include "ChatRoom.h"

bool ChatRoom::init()
{
	if (!Scene::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	BGLayer = LayerColor::create(Color4B::ORANGE);
	this->addChild(BGLayer,kZBGLayer,kZBGLayer);

	
	LayerColor* InputBoxBGLayer = LayerColor::create(Color4B::BLUE,4 * BGLayer->getContentSize().width / 5, 1 * BGLayer->getContentSize().height / 7);
	InputBoxBGLayer->setAnchorPoint(Vec2::ZERO);
	InputBoxBGLayer->setPosition(40, 30);
	BGLayer->addChild(InputBoxBGLayer, kZInputBoxBGLayer, kZInputBoxBGLayer);
	

	createTextField();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* eve) {
		if (code == EventKeyboard::KeyCode::KEY_ENTER) {
			return true;
		}
		else {
			return false;
		}
	};
	keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* eve) {
		if(code == EventKeyboard::KeyCode::KEY_ENTER)
			sendMsg();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	return true;
}

bool ChatRoom::onTouchBegan(Touch * touch, Event * ev)
{

	//�����ж��Ƿ�����˿ؼ�    
	bool isClicked = textEdit->boundingBox().containsPoint(touch->getLocation());

	//��������˿ؼ�    
	if (isClicked)
	{
		//���������    
		textEdit->attachWithIME();
	}
	else
	{
		textEdit->detachWithIME();
	}

	//��ʾ���ܴ�����Ϣ    
	return true;
}

void ChatRoom::sendMsg()
{
	textEdit->setString("");
}

void ChatRoom::createTextField()
{
	//�ı��༭������
	textEdit = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "fonts\arial.ttf", 36);
	textEdit->setAnchorPoint(Vec2::ZERO);
	textEdit->setPosition(Vec2(40, 30));
	textEdit->setColorSpaceHolder(Color3B::WHITE);
	textEdit->setMaxLineWidth(5);

	BGLayer->addChild(textEdit, kZEditBox, kZEditBox);
}
