#ifndef __MAGIC_TOWER__
#define __MAGIC_TOWER__

#define TIPBARHEIGHT 25
#define MAP_SIZE 416
#define STATUSBARWIDTH 192
#define ADDED_WIDTH (MAP_SIZE+STATUSBARWIDTH)
#define ADDED_HEIGHT (MAP_SIZE+TIPBARHEIGHT)
#define TILE_WIDTH 32
#define TILE_COUNT 13
#define NORMAL_TIPBAR "                 <Space> Help                        <S> Save                       <L> Load"


#include "cocos2d.h"
//#include "chat_client.h"
#include "GameConstants.h"
#include "SimpleAudioEngine.h"
//Layer
#include "ControlLayer.h"
#include "GameLayer.h"
#include "SettingLayer.h"
//Scene
#include "StartScene.h"
#include "GameScene.h"

//��ͼ��Ԫ���Լ���ͼ����
#include "Hero.h"
#include "Enemy.h"
#include "Teleport.h"
#include "NPC.h"
#include "GameMap.h"
//���������Լ�������Ƶ�浵�������� 
#include "Singleton.h"
#include "AnimationControl.h"
#include "AudioControl.h"
#include "SaveControl.h"
//������
#include "ChatRoom.h"
//ȫ�ֱ���
#include "Global.h"
#endif