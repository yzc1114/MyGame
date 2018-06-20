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
	//ˢ��ĳ��״̬��
	void refreshStatus(kZorder kzorder);
	//ˢ��ȫ��״̬��
	void refreshAllStatus();
	//Ϊ״̬������һ����Label �þɵ�Label��Ϊ����
	Label* createNewLabelForStatus(Label* oldLabel);
	//��ʾ�·���ʾ
	void showTipBarText(std::string str);
	//��ʾ�浵����
	void showSaveLayer();
	//�����浵����ʾ�Ĵ浵��Ϣ�ַ���
	std::string createTheInfoShowedOnSaveLayer(int order);
	//��꾭���浵��ɫЧ��
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
	//״̬�����õ��ַ���ʽ
	TTFConfig ttfconfigStatusBar;
	//��¼�Ƿ���չʾ�浵���� ��ʼΪfalse
	bool isShowingSaveLayer = false;

	Layer* victoryLayer;
};

#endif 
