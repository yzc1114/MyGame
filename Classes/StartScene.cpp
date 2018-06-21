#include "StartScene.h"

Scene * StartScene::createStartScene()
{
	StartScene* scene = new StartScene;
	if (scene && scene->init()) {
		scene->autorelease();
		scene->initStartScene();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;

}

bool StartScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	return true;
}

void StartScene::initStartScene()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//这部分没啥技术含量就不加注释了
	auto* StartSceneBG = Sprite::create("StartBG.png");
	StartSceneBG->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	addChild(StartSceneBG, 0);

	TTFConfig ttfconfig("fonts/HGrep.ttf", 108);
	auto caption = Label::createWithTTF(ttfconfig, "Magic Tower");
	caption->setColor(Color3B::RED);
	caption->enableGlow(Color4B::RED);
	caption->enableShadow(Color4B::RED, Size(4, 4));
	caption->setPosition(origin.x + visibleSize.width / 2, origin.y + 3 * visibleSize.height / 4);
	this->addChild(caption, 1);

	TTFConfig MenuConfig("fonts/Redocn.ttf", 34);
	auto StartLabel = Label::createWithTTF(MenuConfig,"Start");
	StartLabel->setColor(ccc3(204,0,0));
	auto buttonNewGame = MenuItemLabel::create(StartLabel, CC_CALLBACK_1(StartScene::buttonStartCALLBACK, this));
	auto SettingLabel = Label::createWithTTF(MenuConfig, "Setting");
	SettingLabel->setColor(ccc3(204, 0, 0));
	auto buttonSetting = MenuItemLabel::create(SettingLabel, [&](Ref* psender) {this->addChild(SettingLayer::create(), 100); });
	auto ExitLabel = Label::createWithTTF(MenuConfig, "Exit");
	ExitLabel->setColor(ccc3(204, 0, 0));
	auto buttonExit = MenuItemLabel::create(ExitLabel, CC_CALLBACK_1(StartScene::buttonExitCALLBACK, this));


	auto menu = Menu::create(buttonNewGame, buttonSetting, buttonExit ,NULL);
	
	menu->alignItemsVerticallyWithPadding(30);
	menu->setPosition(origin.x + visibleSize.width / 2, origin.y + 2.5*visibleSize.height / 7);
	this->addChild(menu, 2);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/StartBGM.mp3");
	

}


void StartScene::buttonStartCALLBACK(Ref* psender) {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/STARTBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	Global::instance()->heroSpawnTileCoord = Vec2(1, 11);
	auto scene = GameScene::createScene();
	while (scene == nullptr) {
		scene = GameScene::createScene();
	}
	Director::getInstance()->replaceScene(scene);

}
void StartScene::buttonExitCALLBACK(Ref* psender) {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/STARTBGS.mp3");
	Director::getInstance()->end();
}


