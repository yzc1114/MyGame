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
	boot_client(); //初始化客户端
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//背景Layer
	BGLayer = LayerColor::create(Color4B::BLACK);
	this->addChild(BGLayer, kZBGLayer, kZBGLayer);
	//退出按钮
	auto exit = MenuItemImage::create("Close/CloseNormal.png", "Close/CloseSelected.png", [&](Ref* ref) {Director::getInstance()->popScene(); });
	auto exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(exit->getContentSize().width / 2, MAP_SIZE);
	BGLayer->addChild(exitButton, 10);
	//输入框背景
	LayerColor* InputBoxBGLayer = LayerColor::create(Color4B::BLACK, 4 * BGLayer->getContentSize().width / 5, 1 * BGLayer->getContentSize().height / 9);
	InputBoxBGLayer->setAnchorPoint(Vec2::ZERO);
	InputBoxBGLayer->setPosition(40, 30);
	BGLayer->addChild(InputBoxBGLayer, kZInputBoxBGLayer, kZInputBoxBGLayer);
	//创建输入框函数
	createTextField();
	//创建遮挡信息框的条
	Barrier1 = LayerColor::create(Color4B::WHITE, BGLayer->getContentSize().width, BGLayer->getContentSize().height / 9);
	Barrier1->setAnchorPoint(Vec2::ZERO);
	Barrier1->setPosition(0, 8 * BGLayer->getContentSize().height / 9);
	BGLayer->addChild(Barrier1, kZBarrier1, kZBarrier1);
	Barrier2 = LayerColor::create(Color4B::WHITE, BGLayer->getContentSize().width, 2 * BGLayer->getContentSize().height / 9);
	Barrier2->setAnchorPoint(Vec2::ZERO);
	Barrier2->setPosition(0, 0);
	BGLayer->addChild(Barrier2, kZBarrier2, kZBarrier2);
	//加入所有监听器
	//触摸监听器 若点击了输入框 则可以进行输入 若点击其他地方 则不可以输入
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//添加键盘监听器 实现文本输入 使用了λ函数
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
	client = new chat_client(io_service, iterator); // 初始化、连接  
	t = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)); // 线程 

}

bool ChatRoom::onTouchBegan(Touch * touch, Event * ev)
{

	//用于判断是否点中了控件    
	bool isClicked = textEdit->boundingBox().containsPoint(touch->getLocation());

	//如果点中了控件    
	if (isClicked)
	{
		textEdit->setPlaceHolder("");
		//弹出软键盘    
		textEdit->attachWithIME();
	}
	else
	{
		textEdit->detachWithIME();
		textEdit->setPlaceHolder("Please input:");
	}

	//表示接受触摸消息    
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

//创建输入框
void ChatRoom::createTextField()
{
	//文本编辑框设置
	textEdit = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "fonts\arial.ttf", 36);
	textEdit->setAnchorPoint(Vec2::ZERO);
	textEdit->setPosition(Vec2(40, 30));
	textEdit->setColorSpaceHolder(Color3B::WHITE);
	

	BGLayer->addChild(textEdit, kZEditBox, kZEditBox);
}
//创建消息文本的Label
Label * ChatRoom::createMSGLabel(const std::string & str )
{
	TTFConfig ttfconfig("fonts/arial.ttf", 18);
	Label* label = Label::createWithTTF(ttfconfig, str, TextHAlignment::LEFT, 4 * BGLayer->getContentSize().width / 5);
	label->setAnchorPoint(Vec2::ZERO);
	return label;
}//向聊天室添加新消息

void ChatRoom::addLabel(Label * label)
{
	if (LabelList.empty()) { //如果消息室米有消息就直接push_back()
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
		rollUpWhenNewMsg(); //有新消息来并且新消息在可见范围之外
		createScrollLayerIfitHasNotBeenCreated();
	}
	BGLayer->addChild(label, kZMSGBox);
	

}

void ChatRoom::rollUpWhenNewMsg() //有新消息来的时候 若需要向上翻滚则翻滚相应距离
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
	chatRoomLength = Barrier1->getPositionY() - (Barrier2->getPositionY() + Barrier2->getContentSize().height); //聊天室总长
	MessageLength = 0;//消息加起来的总长
	for (auto label : LabelList) {
		MessageLength += label->getContentSize().height;
	}
	buttonLength = 25; //向上及向下按钮的宽与长
	ScrollerLength = chatRoomLength / MessageLength * (chatRoomLength - 2 * buttonLength); //滚动条的长度 
	if (!ScrollLayerHasBeenCreated) {
		ScrollLayerHasBeenCreated = true;
		//添加第一个按钮
		buttonLayer1 = LayerColor::create(Color4B::RED, buttonLength, buttonLength);
		buttonLayer1->setAnchorPoint(Vec2(0, 0));
		buttonLayer1->setPosition(ADDED_WIDTH - buttonLength, Barrier1->getPositionY() - buttonLength);
		BGLayer->addChild(buttonLayer1, kZScrollLayer);
		//添加第二个按钮
		buttonLayer2 = LayerColor::create(Color4B::RED, buttonLength, buttonLength);
		buttonLayer2->setAnchorPoint(Vec2(0, 0));
		buttonLayer2->setPosition(ADDED_WIDTH - buttonLength, Barrier2->getPositionY() + Barrier2->getContentSize().height);
		BGLayer->addChild(buttonLayer2, kZScrollLayer);
		//为按钮添加事件
		EventListenerTouchOneByOne* TouchListener = EventListenerTouchOneByOne::create();
		TouchListener->onTouchBegan = CC_CALLBACK_2(ChatRoom::onMouseDown, this);
		TouchListener->onTouchEnded = CC_CALLBACK_2(ChatRoom::onMouseUp, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);
	}
	
}

bool ChatRoom::onMouseDown(Touch* touch, Event* ev)
{
	chatRoomLength = Barrier1->getPositionY() - (Barrier2->getPositionY() + Barrier2->getContentSize().height); //聊天室总长
	MessageLength = 0;//消息加起来的总长
	for (auto label : LabelList) {
		MessageLength += label->getContentSize().height;
	}
	ScrollerLength = chatRoomLength / MessageLength * (chatRoomLength - 2 * buttonLength); //滚动条的长度
	if (buttonLayer1->getBoundingBox().containsPoint(touch->getLocation())) { //点击向上按钮
		schedule(schedule_selector(ChatRoom::moveUp, this),0.1f);
	}
	else if (buttonLayer2->getBoundingBox().containsPoint(touch->getLocation())) { //点击向下按钮
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
	if (buttonLayer1->getBoundingBox().containsPoint(touch->getStartLocation())) { //点击向上按钮
		unschedule(schedule_selector(ChatRoom::moveUp, this));
	}
	else if (buttonLayer2->getBoundingBox().containsPoint(touch->getStartLocation())) { //点击向下按钮
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
	//文本编辑框设置
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

