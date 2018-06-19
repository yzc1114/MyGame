#ifndef _SETTING_LAYER_
#define _SETTING_LAYER_
#include "MagicTower.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui\CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


class SettingLayer : public Layer
{
public:
	SettingLayer();
	~SettingLayer();
	virtual bool init();
	CREATE_FUNC(SettingLayer);
	void sliderCallback(cocos2d::Ref * ref, Slider::EventType type);
};

#endif // !_SETTING_LAYER_