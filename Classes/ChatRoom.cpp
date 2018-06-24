#include "ChatRoom.h"


boost::asio::io_service io_service;


bool ChatRoom::init()
{
	if (!Scene::init()) {
		return false;
	}
	if (hasName()) {
		extrainit();
	}
	this->InputName();

	
	return true;
}

void ChatRoom::extrainit()
{
	boot_client(); //��ʼ���ͻ���
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����Layer
	BGLayer = LayerColor::create(Color4B::BLACK);
	this->addChild(BGLayer, kZBGLayer, kZBGLayer);
	//�˳���ť
	auto exit = MenuItemImage::create("Close/CloseNormal.png", "Close/CloseSelected.png", [&](Ref* ref) {Director::getInstance()->popScene(); });
	auto exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(exit->getContentSize().width / 2, MAP_SIZE);
	BGLayer->addChild(exitButton, 10);
	//����򱳾�
	LayerColor* InputBoxBGLayer = LayerColor::create(Color4B::BLACK, 4 * BGLayer->getContentSize().width / 5, 1 * BGLayer->getContentSize().height / 9);
	InputBoxBGLayer->setAnchorPoint(Vec2::ZERO);
	InputBoxBGLayer->setPosition(40, 30);
	BGLayer->addChild(InputBoxBGLayer, kZInputBoxBGLayer, kZInputBoxBGLayer);
	//�����������
	createTextField();
	//�����ڵ���Ϣ�����
	Barrier1 = LayerColor::create(Color4B::WHITE, BGLayer->getContentSize().width, BGLayer->getContentSize().height / 9);
	Barrier1->setAnchorPoint(Vec2::ZERO);
	Barrier1->setPosition(0, 8 * BGLayer->getContentSize().height / 9);
	BGLayer->addChild(Barrier1, kZBarrier1, kZBarrier1);
	Barrier2 = LayerColor::create(Color4B::WHITE, BGLayer->getContentSize().width, 2 * BGLayer->getContentSize().height / 9);
	Barrier2->setAnchorPoint(Vec2::ZERO);
	Barrier2->setPosition(0, 0);
	BGLayer->addChild(Barrier2, kZBarrier2, kZBarrier2);
	//�������м�����
	//���������� ������������ ����Խ������� ����������ط� �򲻿�������
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
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
		if (code == EventKeyboard::KeyCode::KEY_ENTER)
			sendMsg();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

}

ChatRoom::ChatRoom()
{
	Global::instance()->chatRoom = this;
}

ChatRoom::~ChatRoom()
{
	if (client != nullptr) {
		Global::instance()->chatRoom = NULL;
		client->close();
		t.join();
	}
	
}

void ChatRoom::boot_client()
{
	resolver = new tcp::resolver(io_service);
	query = new tcp::resolver::query("127.0.0.1", "1000");
	tcp::resolver::iterator iterator = (*resolver).resolve(*query);
	client = new chat_client(io_service, iterator); // ��ʼ��������  
	t = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)); // �߳� 

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
	std::string str = Global::instance()->userName + " : " + textEdit->getString();
	//this->addLabel(createMSGLabel(str));
	textEdit->setString("");
	char line[chat_message::max_body_length + 1] = { '\0' };
	memcpy(line, str.c_str(), str.size());
	chat_message msg;
	msg.body_length(strlen(line));
	memcpy(msg.body(), line, msg.body_length());// line to msg  
	msg.encode_header();
	client->write(msg);
}

void ChatRoom::receiveMsg(const std::string & str)
{
	this->addLabel(this->createMSGLabel(str));
}

//���������
void ChatRoom::createTextField()
{
	//�ı��༭������
	textEdit = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "fonts\arial.ttf", 36);
	textEdit->setAnchorPoint(Vec2::ZERO);
	textEdit->setPosition(Vec2(40, 30));
	textEdit->setColorSpaceHolder(Color3B::WHITE);
	

	BGLayer->addChild(textEdit, kZEditBox, kZEditBox);
}
//������Ϣ�ı���Label
Label * ChatRoom::createMSGLabel(const std::string & str )
{
	TTFConfig ttfconfig("fonts/arial.ttf", 18);
	Label* label = Label::createWithTTF(ttfconfig, str, TextHAlignment::LEFT, 4 * BGLayer->getContentSize().width / 5);
	label->setAnchorPoint(Vec2::ZERO);
	return label;
}//���������������Ϣ

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

void ChatRoom::InputName() {
	InputNameLayer = LayerColor::create(Color4B::WHITE);
	this->addChild(InputNameLayer);
	TTFConfig config("fonts/arial.ttf", 28);
	auto label = Label::createWithTTF(config, "Input your name");
	label->setColor(Color3B::BLACK);
	label->setPosition( ADDED_WIDTH / 2, 2.8 * ADDED_HEIGHT / 4);
	InputNameLayer->addChild(label, 1);
	Layer* TextBGLayer = LayerColor::create(Color4B::BLACK,ADDED_WIDTH / 2 , ADDED_HEIGHT / 9);
	TextBGLayer->setPosition(ADDED_WIDTH / 4, ADDED_HEIGHT / 2);
	InputNameLayer->addChild(TextBGLayer);
	//�ı��༭������
	textEdit = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "fonts\arial.ttf", 36);
	textEdit->setAnchorPoint(Vec2::ZERO);
	textEdit->setPosition(ADDED_WIDTH / 4, ADDED_HEIGHT / 2);
	textEdit->setColorSpaceHolder(Color3B::WHITE);
	InputNameLayer->addChild(textEdit);
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, InputNameLayer);
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
		if (code == EventKeyboard::KeyCode::KEY_ENTER) {
			std::string str = textEdit->getString();
			if (str != "") {
				Global::instance()->userName = str;
				eve->getCurrentTarget()->removeFromParent();
				extrainit();
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, InputNameLayer);
}

bool ChatRoom::hasName()
{
	if (Global::instance()->userName == "") {
		return false;
	}
	return true;
}

bool chat_client::init()
{
	if (!Node::init()) {
		return false;
	}
	return true;
}

void chat_client::handle_read_body(const boost::system::error_code & error)
{
	cout << __FUNCTION__ << endl;
	if (!error)
	{
		std::string str = std::string(read_msg_.body(), read_msg_.body_length());
		log(str.c_str());
		std::function<void()> func = std::bind(&ChatRoom::receiveMsg, Global::instance()->chatRoom, str);
		Director::getInstance()->getScheduler()->performFunctionInCocosThread(func);
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), chat_message::header_length),
			boost::bind(&chat_client::handle_read_header, this, // 4  
				boost::asio::placeholders::error));
	}
	else
	{
		do_close();
	}
}

