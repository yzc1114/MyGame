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


	auto LayerColorBG = LayerColor::create(Color4B::BLACK);
	this->addChild(LayerColorBG, 0);

	TTFConfig ttfconfig("fonts/arial.ttf", 72);
	auto caption = Label::createWithTTF(ttfconfig, "The Tower");
	caption->setPosition(origin.x + visibleSize.width / 2, origin.y + 3 * visibleSize.height / 4);
	this->addChild(caption, 1);

	auto buttonNewGame = MenuItemFont::create("Start", CC_CALLBACK_1(StartScene::buttonStartCALLBACK, this));
	auto buttonExit = MenuItemFont::create("Exit", CC_CALLBACK_1(StartScene::buttonExitCALLBACK, this));
	auto menu = Menu::create(buttonNewGame, buttonExit ,NULL);
	menu->alignItemsVerticallyWithPadding(30);
	menu->setPosition(origin.x + visibleSize.width / 2, origin.y + 2.5*visibleSize.height / 7);
	this->addChild(menu, 2);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/StartBGM.mp3");
	

}


void StartScene::buttonStartCALLBACK(Ref* psender) {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/STARTBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);

}
void StartScene::buttonExitCALLBACK(Ref* psender) {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/STARTBGS.mp3");
	Director::getInstance()->end();
}


