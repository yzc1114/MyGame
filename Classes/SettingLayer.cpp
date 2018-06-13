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

	//ͨ��create()��������slider     
	auto _newSlider = Slider::create();

	//����slider����ͼƬ  
	_newSlider->loadBarTexture("Slider/Slider_BG.png");

	//����slider����ͼƬ  
	_newSlider->loadProgressBarTexture("Slider/Slider_ON.png");

	//����slider������ťͼƬ   
	_newSlider->loadSlidBallTextures("Slider/Slider.png", "Slider/Slider.png", "Slider/Slider.png");

	//����slider�Ľ��ȣ���Χ��0~100��  
	_newSlider->setPercent(100);

	//���slider�Ľ���  
	int percent = _newSlider->getPercent();

	//����slider��λ��        
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
