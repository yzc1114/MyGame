#include "ChatRoom.h"

bool ChatRoom::init()
{
	if (!Scene::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//背景Layer
	BGLayer = LayerColor::create(Color4B::ORANGE);
	this->addChild(BGLayer,kZBGLayer,kZBGLayer);
	//输入框背景
	LayerColor* InputBoxBGLayer = LayerColor::create(Color4B::BLUE,4 * BGLayer->getContentSize().width / 5, 1 * BGLayer->getContentSize().height / 9);
	InputBoxBGLayer->setAnchorPoint(Vec2::ZERO);
	InputBoxBGLayer->setPosition(40, 30);
	BGLayer->addChild(InputBoxBGLayer, kZInputBoxBGLayer, kZInputBoxBGLayer);
	//创建输入框函数
	createTextField();
	//创建遮挡信息框的条
	Barrier1 = LayerColor::create(Color4B::BLACK, BGLayer->getContentSize().width, BGLayer->getContentSize().height / 9);
	Barrier1->setAnchorPoint(Vec2::ZERO);
	Barrier1->setPosition(0, 8 * BGLayer->getContentSize().height / 9);
	BGLayer->addChild(Barrier1, kZBarrier1, kZBarrier1);
	Barrier2 = LayerColor::create(Color4B::BLACK, BGLayer->getContentSize().width,2 * BGLayer->getContentSize().height / 9);
	Barrier2->setAnchorPoint(Vec2::ZERO);
	Barrier2->setPosition(0, 0);
	BGLayer->addChild(Barrier2, kZBarrier2, kZBarrier2);
	//加入所有监听器
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
	//
	



	return true;
}

bool ChatRoom::onTouchBegan(Touch * touch, Event * ev)
{

	//用于判断是否点中了控件    
	bool isClicked = textEdit->boundingBox().containsPoint(touch->getLocation());

	//如果点中了控件    
	if (isClicked)
	{
		//弹出软键盘    
		textEdit->attachWithIME();
	}
	else
	{
		textEdit->detachWithIME();
	}

	//表示接受触摸消息    
	return true;
}

void ChatRoom::sendMsg()
{
	addLabel(createMSGLabel(textEdit->getString()));
	textEdit->setString("");
}

void ChatRoom::createTextField()
{
	//文本编辑框设置
	textEdit = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "fonts\arial.ttf", 36);
	textEdit->setAnchorPoint(Vec2::ZERO);
	textEdit->setPosition(Vec2(40, 30));
	textEdit->setColorSpaceHolder(Color3B::WHITE);
	textEdit->setMaxLineWidth(5);

	BGLayer->addChild(textEdit, kZEditBox, kZEditBox);
}

Label * ChatRoom::createMSGLabel(std::string str)
{
	TTFConfig ttfconfig("fonts/arial.ttf", 18);
	Label* label = Label::createWithTTF(ttfconfig, str, TextHAlignment::LEFT, 4 * BGLayer->getContentSize().width / 5);
	label->setAnchorPoint(Vec2::ZERO);
	return label;
}

void ChatRoom::addLabel(Label * label)
{
	if (LabelQueue.empty()) {
		LabelQueue.push(label);
		label->setPosition(30, 8 * BGLayer->getContentSize().height / 9 - label->getContentSize().height);
		BGLayer->addChild(label, kZMSGBox);
		return;
	}
	Label* back = LabelQueue.back();
	LabelQueue.push(label);
	float positionYAboveLabel = back->getPositionY();
	label->setPosition(30, positionYAboveLabel - label->getContentSize().height);
	if (label->getPositionY() < Barrier2->getPositionY()) {
		rollUpWhenNewMsg();
	}
	BGLayer->addChild(label, kZMSGBox);
	

}

void ChatRoom::rollUpWhenNewMsg()
{

}
