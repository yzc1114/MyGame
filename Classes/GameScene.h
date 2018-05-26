#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include"cocos2d.h"
#include"GameLayer.h"
#include"ControlLayer.h"
#include"GameConstants.h"
USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

	void refreshStatus(kZorder kzorder);

	Label* createNewLabelForStatus(Label* oldLabel);

	void showTipBarText(std::string str);

	void showSaveLayer();

	void SaveLayerOnMouseMove(Event* event);

	void SaveLayerOnMouseDown(Event* event);

};

#endif 
