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

//地图中元素以及地图本身
#include "Hero.h"
#include "Enemy.h"
#include "Teleport.h"
#include "NPC.h"
#include "GameMap.h"
//单例对象以及动画音频存档控制引擎 
#include "Singleton.h"
#include "AnimationControl.h"
#include "AudioControl.h"
#include "SaveControl.h"
//聊天室
#include "ChatRoom.h"
//全局变量
#include "Global.h"
#endif