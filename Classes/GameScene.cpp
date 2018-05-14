#include "GameScene.h"


USING_NS_CC;


Scene* GameScene::createScene()
{
    return GameScene::create();
}


bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	Global::instance()->gameScene = this;

	
	GameLayer* gamelayer = GameLayer::createGameLayer(0);
	this->addChild(gamelayer,-1);
	gamelayer->setPosition(192 , TIPBARHEIGHT);

	LayerColor* statusLayer = LayerColor::create(Color4B::YELLOW, STATUSBARWIDTH, MAP_SIZE);
	this->addChild(statusLayer, -1);
	statusLayer->setPosition(0, TIPBARHEIGHT);

	LayerColor* downTipBar = LayerColor::create(Color4B::WHITE, STATUSBARWIDTH + MAP_SIZE, TIPBARHEIGHT);
	this->addChild(downTipBar, -1);
	downTipBar->setPosition(0, 0);

	TTFConfig ttfconfig1("fonts/arial.ttf", 12);
	auto tipBarLabel = Label::createWithTTF(ttfconfig1, NORMAL_TIPBAR); //初始化TipBar
	this->addChild(tipBarLabel, kZTipBar, kZTipBar);
	tipBarLabel->setAnchorPoint(Vec2::ZERO);
	tipBarLabel->setPosition(5, 5);
	tipBarLabel->setColor(Color3B::BLACK);

	ControlLayer* controllayer = ControlLayer::create();
	this->addChild(controllayer, 2);
	controllayer->setPosition(Vec2::ZERO);


	//创建左侧显示人物信息的状态条
	//显示血量
	TTFConfig ttfconfig("fonts/arial.ttf", 12);
	std::string temp = std::to_string(Global::instance()->hero->HP);
	auto currentHP = Label::createWithTTF(ttfconfig, "Hero HP : " + temp);
	currentHP->setAnchorPoint(Vec2::ZERO);
	currentHP->setColor(Color3B::BLACK);
	currentHP->setPosition(192 / 3, 32 * 11);
	this->addChild(currentHP, kZHP, kZHP);
	//显示攻击力
	temp = std::to_string(Global::instance()->hero->ATK);
	auto currentATK = Label::createWithTTF(ttfconfig, "Hero ATK : " + temp);
	currentATK->setAnchorPoint(Vec2::ZERO);
	currentATK->setColor(Color3B::BLACK);
	currentATK->setPosition(192 / 3, 32 * 10);
	this->addChild(currentATK, kZATK, kZATK);
	//显示防御力
	temp = std::to_string(Global::instance()->hero->DEF);
	auto currentDEF = Label::createWithTTF(ttfconfig, "Hero DEF : " + temp);
	currentDEF->setAnchorPoint(Vec2::ZERO);
	currentDEF->setColor(Color3B::BLACK);
	currentDEF->setPosition(192 / 3, 32 * 9);
	this->addChild(currentDEF, kZDEF, kZDEF);
	//显示黄钥匙数量
	temp = std::to_string(Global::instance()->hero->YellowKeys);
	auto currentYellowKeys = Label::createWithTTF(ttfconfig, "Hero YellowKeys : " + temp);
	currentYellowKeys->setAnchorPoint(Vec2::ZERO);
	currentYellowKeys->setColor(Color3B::BLACK);
	currentYellowKeys->setPosition(192 / 3, 32 * 8);
	this->addChild(currentYellowKeys, kZYellowKeys, kZYellowKeys);
	//显示蓝钥匙数量
	temp = std::to_string(Global::instance()->hero->BlueKeys);
	auto currentBlueKeys = Label::createWithTTF(ttfconfig, "Hero BlueKeys : " + temp);
	currentBlueKeys->setAnchorPoint(Vec2::ZERO);
	currentBlueKeys->setColor(Color3B::BLACK);
	currentBlueKeys->setPosition(192 / 3, 32 * 7);
	this->addChild(currentBlueKeys, kZBlueKeys, kZBlueKeys);
	//显示红钥匙数量
	temp = std::to_string(Global::instance()->hero->RedKeys);
	auto currentRedKeys = Label::createWithTTF(ttfconfig, "Hero RedKeys : " + temp);
	currentRedKeys->setAnchorPoint(Vec2::ZERO);
	currentRedKeys->setColor(Color3B::BLACK);
	currentRedKeys->setPosition(192 / 3, 32 * 6);
	this->addChild(currentRedKeys, kZRedKeys, kZRedKeys);

	


}

//刷新左侧状态条 形参为kZoder的枚举常量 表示人物变化的信息类别
void GameScene::refreshStatus(kZorder order)
{
	Label* newLabel;
	GameScene* tempGameScene = Global::instance()->gameScene;

	if (order == kZHP) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZHP)));
		this->removeChildByTag(kZHP);
		this->addChild(newLabel, kZHP, kZHP);
	}
	else if (order == kZATK) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZATK)));
		this->removeChildByTag(kZATK);
		this->addChild(newLabel, kZATK, kZATK);
	}
	else if (order == kZDEF) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZDEF)));
		this->removeChildByTag(kZDEF);
		this->addChild(newLabel, kZDEF, kZDEF);
	}
	else if (order == kZYellowKeys) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZYellowKeys)));
		this->removeChildByTag(kZYellowKeys);
		this->addChild(newLabel, kZYellowKeys, kZYellowKeys);
	}
	else if (order == kZBlueKeys) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZBlueKeys)));
		this->removeChildByTag(kZBlueKeys);
		this->addChild(newLabel, kZBlueKeys, kZBlueKeys);
	}
	else if (order == kZRedKeys) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZRedKeys)));
		this->removeChildByTag(kZRedKeys);
		this->addChild(newLabel, kZRedKeys, kZRedKeys);
	}

	
}

Label* GameScene::createNewLabelForStatus(Label* oldLabel) {
	TTFConfig ttfconfig("fonts/arial.ttf", 12);
	std::string newString;
	switch (oldLabel->getZOrder()) {
	case kZHP:newString = std::string("Hero HP : " + std::to_string(Global::instance()->hero->HP)); break;
	case kZATK:newString = std::string("Hero ATK : " + std::to_string(Global::instance()->hero->ATK)); break;
	case kZDEF:newString = std::string("Hero DEF : " + std::to_string(Global::instance()->hero->DEF)); break;
	case kZYellowKeys:newString = std::string("Hero YellowKeys : " + std::to_string(Global::instance()->hero->YellowKeys)); break;
	case kZBlueKeys:newString = std::string("Hero BlueKeys : " + std::to_string(Global::instance()->hero->BlueKeys)); break;
	case kZRedKeys:newString = std::string("Hero RedKeys : " + std::to_string(Global::instance()->hero->RedKeys)); break;
	default:;
	}
	Label* newLabel = Label::createWithTTF(ttfconfig, newString);
	newLabel->setAnchorPoint(Vec2::ZERO);
	newLabel->setColor(oldLabel->getColor());
	newLabel->setPosition(oldLabel->getPosition());
	return newLabel;

}

void GameScene::showTipBarText(std::string str)
{
	Label* label = (Label*)(this->getChildByTag(kZTipBar));
	TTFConfig ttfconfig("fonts/arial.ttf", 8);
	Label* newLabel = Label::createWithTTF(ttfconfig, str);
	newLabel->setPosition(label->getPosition());
	newLabel->setColor(label->getColor());
	this->removeChildByTag(kZTipBar);
	this->addChild(newLabel, kZTipBar, kZTipBar);
}

void GameScene::showSaveLayer()
{
	auto Saver = SaveControl::instance();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	SaverLayer = LayerColor::create(Color4B::ORANGE,3/4*visibleSize.width,3/4*visibleSize.height);
	SaverLayer->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(SaverLayer,kZSaveLayer,kZSaveLayer);
	for (int i = 1; i <= 5; i++) { //五个存档位置
		auto subLayer = LayerColor::create(Color4B::BLUE, visibleSize.width * 4 / 5, visibleSize.height * 4 / 7 / 5);
		SaverLayer->addChild(subLayer,kZSaveLayer + 1, i);
		subLayer->setPosition(SaverLayer->getContentSize().width / 2, (2 * (i - 1) + 1)*SaverLayer->getContentSize().height / 10);
		if (Saver->checkIfTheSaveExisted(i)) {
			//存档存在 则显示英雄HP ATK DEF coins currentlevel
		}
		else {
			TTFConfig ttfconfig("fonts/arial.ttf", 12);
			Label* label = Label::createWithTTF(ttfconfig, "No Save Existed");
			subLayer->addChild(label, kZSaveLayer + 2);
			subLayer->setPosition(label->getContentSize().width / 2, label->getContentSize().height / 2);
		}
	}

	auto MouseListener = EventListenerMouse::create();
	MouseListener->onMouseMove = CC_CALLBACK_1(GameScene::SaveLayerOnMouseMove,this);
	

}

void GameScene::SaveLayerOnMouseMove(Event * event)
{
	auto MouseEvent = (EventMouse*)event;
	float x = MouseEvent->getCursorX();
	float y = MouseEvent->getCursorY();
	//for(int i)
}


