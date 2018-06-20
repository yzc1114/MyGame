#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include"MagicTower.h"
USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);
	//刷新某个状态栏
	void refreshStatus(kZorder kzorder);
	//刷新全部状态栏
	void refreshAllStatus();
	//为状态栏创建一个新Label 用旧的Label作为参数
	Label* createNewLabelForStatus(Label* oldLabel);
	//显示下方提示
	void showTipBarText(std::string str);
	//显示存档界面
	void showSaveLayer();
	//创建存档上显示的存档信息字符串
	std::string createTheInfoShowedOnSaveLayer(int order);
	//鼠标经过存档变色效果
	void SaveLayerOnMouseMove(Event* event);
	void SaveLayerOnMouseDown(Event* event);
	void setIfShowingSaveLayer(bool b) { isShowingSaveLayer = b; };
<<<<<<< HEAD
	//
=======

	void GameVictory();

	void continueCALLBACK();

>>>>>>> NoWeb'sJ
private:
	//状态栏所用的字符格式
	TTFConfig ttfconfigStatusBar;
	//记录是否在展示存档界面 初始为false
	bool isShowingSaveLayer = false;

	Layer* victoryLayer;
};

#endif 
