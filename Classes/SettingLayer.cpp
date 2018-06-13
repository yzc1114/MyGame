#include "SettingLayer.h"



SettingLayer::SettingLayer()
{
}


SettingLayer::~SettingLayer()
{
}

bool SettingLayer::init()
{
	if (!Layer::init()) {
		return nullptr;
	}
	
	Layer* BGLayer = LayerColor::create(Color4B::BLACK);
	this->addChild(BGLayer, 0);

	auto TouchBlocker = EventListenerTouchOneByOne::create();
	TouchBlocker->onTouchBegan = [&](Touch* touch, Event* eve) {return false; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchBlocker, BGLayer);

	auto exit = MenuItemImage::create("Close/CloseNormal.png", "Close/CloseSelected.png", [&](Ref* ref) {this->removeFromParent(); });
	auto exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(20, 416);
	BGLayer->addChild(exitButton, 1);

	//通过create()方法创建slider     
	auto _newSlider = Slider::create();

	//设置slider背景图片  
	_newSlider->loadBarTexture("Slider/Slider_BG.png");

	//设置slider进度图片  
	_newSlider->loadProgressBarTexture("Slider/Slider_ON.png");

	//设置slider滑动按钮图片   
	_newSlider->loadSlidBallTextures("Slider/Slider.png", "Slider/Slider.png", "Slider/Slider.png");

	//设置slider的进度（范围：0~100）  
	_newSlider->setPercent(100);

	//获得slider的进度  
	int percent = _newSlider->getPercent();

	//设置slider的位置        
	_newSlider->setPosition(Vec2(ADDED_WIDTH, ADDED_HEIGHT) / 2);

	_newSlider->addEventListener(CC_CALLBACK_2(SettingLayer::sliderCallback, this));

	BGLayer->addChild(_newSlider, 2);
}

void SettingLayer::sliderCallback(cocos2d::Ref * ref, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider * slider = dynamic_cast<Slider*>(ref);
		int per = slider->getPercent();
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(per / 100.0);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(per / 100.0);
	}
}
