#include "ChatRoom.h"


bool ChatRoom::init()
{
	if (!Scene::init()) {
		return false;
	}
	boot_client();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����Layer
	BGLayer = LayerColor::create(Color4B::ORANGE);
	this->addChild(BGLayer,kZBGLayer,kZBGLayer);
	//����򱳾�
	LayerColor* InputBoxBGLayer = LayerColor::create(Color4B::BLUE,4 * BGLayer->getContentSize().width / 5, 1 * BGLayer->getContentSize().height / 9);
	InputBoxBGLayer->setAnchorPoint(Vec2::ZERO);
	InputBoxBGLayer->setPosition(40, 30);
	BGLayer->addChild(InputBoxBGLayer, kZInputBoxBGLayer, kZInputBoxBGLayer);
	//�����������
	createTextField();
	//�����ڵ���Ϣ�����
	Barrier1 = LayerColor::create(Color4B::BLACK, BGLayer->getContentSize().width, BGLayer->getContentSize().height / 9);
	Barrier1->setAnchorPoint(Vec2::ZERO);
	Barrier1->setPosition(0, 8 * BGLayer->getContentSize().height / 9);
	BGLayer->addChild(Barrier1, kZBarrier1, kZBarrier1);
	Barrier2 = LayerColor::create(Color4B::BLACK, BGLayer->getContentSize().width,2 * BGLayer->getContentSize().height / 9);
	Barrier2->setAnchorPoint(Vec2::ZERO);
	Barrier2->setPosition(0, 0);
	BGLayer->addChild(Barrier2, kZBarrier2, kZBarrier2);
	//�������м�����
	//���������� ������������ ����Խ������� ����������ط� �򲻿�������
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);
	//��Ӽ��̼����� ʵ���ı����� ʹ���˦˺���
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

ChatRoom::ChatRoom()
{
	Global::instance()->chatRoom = this;
}

ChatRoom::~ChatRoom()
{
	Global::instance()->chatRoom = NULL;
}

void ChatRoom::boot_client()
{
	boost::asio::io_service io_service;

	tcp::resolver resolver(io_service);
	tcp::resolver::query query("127.0.0.1", "1000"); // ip port:����  
	tcp::resolver::iterator iterator = resolver.resolve(query);

	chat_client c(io_service, iterator); // ��ʼ��������  
	client = &c;
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service)); // �߳�  

	char line[chat_message::max_body_length + 1];
	while (std::cin.getline(line, chat_message::max_body_length + 1))
	{
		chat_message msg;
		msg.body_length(strlen(line));
		memcpy(msg.body(), line, msg.body_length());// line to msg  
		msg.encode_header();
		c.write(msg);
	}

	 
}

bool ChatRoom::onTouchBegan(Touch * touch, Event * ev)
{

	//�����ж��Ƿ�����˿ؼ�    
	bool isClicked = textEdit->boundingBox().containsPoint(touch->getLocation());

	//��������˿ؼ�    
	if (isClicked)
	{
		textEdit->setPlaceHolder("");
		//���������    
		textEdit->attachWithIME();
	}
	else
	{
		textEdit->detachWithIME();
		textEdit->setPlaceHolder("Please input:");
	}

	//��ʾ���ܴ�����Ϣ    
	return true;
}



void ChatRoom::sendMsg()
{
	stringstream sstr;
	sstr.clear();
	sstr << textEdit->getString();
	char line[chat_message::max_body_length + 1];
	sstr >> line;
	chat_message msg;
	msg.body_length(strlen(line));
	memcpy(msg.body(), line, msg.body_length());// line to msg  
	msg.encode_header();
	client->write(msg);
	textEdit->setString("");
}
//���������
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
//������Ϣ�ı���Label
Label * ChatRoom::createMSGLabel(std::string str)
{
	TTFConfig ttfconfig("fonts/arial.ttf", 18);
	Label* label = Label::createWithTTF(ttfconfig, str, TextHAlignment::LEFT, 4 * BGLayer->getContentSize().width / 5);
	label->setAnchorPoint(Vec2::ZERO);
	return label;
}
//���������������Ϣ
void ChatRoom::addLabel(Label * label)
{
	if (LabelList.empty()) { //�����Ϣ��������Ϣ��ֱ��push_back()
		LabelList.push_back(label);
		label->setPosition(30, 8 * BGLayer->getContentSize().height / 9 - label->getContentSize().height);
		BGLayer->addChild(label, kZMSGBox);
		return;
	}
	Label* back = LabelList.back();
	LabelList.push_back(label);
	float positionYAboveLabel = back->getPositionY();
	label->setPosition(30, positionYAboveLabel - label->getContentSize().height);
	if (label->getPositionY() < Barrier2->getPositionY() + Barrier2->getContentSize().height) {
		rollUpWhenNewMsg(); //������Ϣ����������Ϣ�ڿɼ���Χ֮��
		createScrollLayerIfitHasNotBeenCreated();
	}
	BGLayer->addChild(label, kZMSGBox);
	

}

void ChatRoom::rollUpWhenNewMsg() //������Ϣ����ʱ�� ����Ҫ���Ϸ����򷭹���Ӧ����
{
	if (LabelList.back()->getPositionY() < Barrier2->getPositionY() + Barrier2->getContentSize().height) {
		float dy = Barrier2->getContentSize().height + Barrier2->getPositionY() - LabelList.back()->getPositionY();
		for (auto label : LabelList) {
			label->setPositionY(label->getPositionY() + dy);
		}
	}
	else {
		return;
	}
}

void ChatRoom::createScrollLayerIfitHasNotBeenCreated()
{		
	chatRoomLength = Barrier1->getPositionY() - (Barrier2->getPositionY() + Barrier2->getContentSize().height); //�������ܳ�
	MessageLength = 0;//��Ϣ���������ܳ�
	for (auto label : LabelList) {
		MessageLength += label->getContentSize().height;
	}
	buttonLength = 25; //���ϼ����°�ť�Ŀ��볤
	ScrollerLength = chatRoomLength / MessageLength * (chatRoomLength - 2 * buttonLength); //�������ĳ��� 
	if (!ScrollLayerHasBeenCreated) {
		ScrollLayerHasBeenCreated = true;
		//��ӵ�һ����ť
		buttonLayer1 = LayerColor::create(Color4B::RED, buttonLength, buttonLength);
		buttonLayer1->setAnchorPoint(Vec2(0, 0));
		buttonLayer1->setPosition(ADDED_WIDTH - buttonLength, Barrier1->getPositionY() - buttonLength);
		BGLayer->addChild(buttonLayer1, kZScrollLayer);
		//��ӵڶ�����ť
		buttonLayer2 = LayerColor::create(Color4B::RED, buttonLength, buttonLength);
		buttonLayer2->setAnchorPoint(Vec2(0, 0));
		buttonLayer2->setPosition(ADDED_WIDTH - buttonLength, Barrier2->getPositionY() + Barrier2->getContentSize().height);
		BGLayer->addChild(buttonLayer2, kZScrollLayer);
		//Ϊ��ť����¼�
		EventListenerTouchOneByOne* TouchListener = EventListenerTouchOneByOne::create();
		TouchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onMouseDown, this);
		TouchListener->onTouchEnded = CC_CALLBACK_2(ChatRoom::onMouseUp, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);
	}
	
}

bool ChatRoom::onMouseDown(Touch* touch, Event* ev)
{
	chatRoomLength = Barrier1->getPositionY() - (Barrier2->getPositionY() + Barrier2->getContentSize().height); //�������ܳ�
	MessageLength = 0;//��Ϣ���������ܳ�
	for (auto label : LabelList) {
		MessageLength += label->getContentSize().height;
	}
	ScrollerLength = chatRoomLength / MessageLength * (chatRoomLength - 2 * buttonLength); //�������ĳ���
	if (buttonLayer1->getBoundingBox().containsPoint(touch->getLocation())) { //������ϰ�ť
		schedule(schedule_selector(ChatRoom::moveUp, this),0.1f);
	}
	else if (buttonLayer2->getBoundingBox().containsPoint(touch->getLocation())) { //������°�ť
		schedule(schedule_selector(ChatRoom::moveDown, this),0.1f);
	}
	/*else if (ScrollLayer->getBoundingBox().containsPoint(touch->getLocation())) {              I give up
		isTouchingScroller = true;
		mouseStartLocationY = touch->getLocation().y;
		scrollerStartLocationY = ScrollLayer->getPositionY();
	}*/
	else {
		return false;
	}

	return true;
}



void ChatRoom::onMouseUp(Touch * touch, Event * ev)
{
	if (buttonLayer1->getBoundingBox().containsPoint(touch->getStartLocation())) { //������ϰ�ť
		unschedule(schedule_selector(ChatRoom::moveUp, this));
	}
	else if (buttonLayer2->getBoundingBox().containsPoint(touch->getStartLocation())) { //������°�ť
		unschedule(schedule_selector(ChatRoom::moveDown, this));
	}
}

void ChatRoom::moveUp(float dt)
{
	if (LabelList.front()->getPositionY() >= Barrier1->getPositionY()) {
		for (auto label : LabelList) {
			label->setPositionY(label->getPositionY() - label->getContentSize().height);
		}
	}
}

void ChatRoom::moveDown(float dt)
{
	if (LabelList.back()->getPositionY() < Barrier2->getPositionY() + Barrier2->getContentSize().height) {
		for (auto label : LabelList) {
			label->setPositionY(label->getPositionY() + label->getContentSize().height);
		}
	}
}
