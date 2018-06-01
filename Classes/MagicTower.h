#ifndef __MAGIC_TOWER__
#define __MAGIC_TOWER__

#define TIPBARHEIGHT 25
#define MAP_SIZE 416
#define STATUSBARWIDTH 192
#define NORMAL_TIPBAR "                 <Space> Help                        <S> Save                       <L> Load                      <R> Restart"


#include "cocos2d.h"
//#include "network\SocketIO.h"
//#include "NetworkControl.h"
#include "GameConstants.h"
#include "SimpleAudioEngine.h"
//Layer
#include "ControlLayer.h"
#include "GameLayer.h"
//Scene
#include "StartScene.h"
#include "GameScene.h"
#include "ChatRoom.h"
//地图中元素以及地图本身
#include "Hero.h"
#include "Enemy.h"
#include "Teleport.h"
#include "NPC.h"
#include "GameMap.h"
//单例对象以及动画音频存档控制引擎
#include "Singleton.h"
#include "AnimationControl.h"
#include "SaveControl.h"
#include "Global.h"
#include "AudioControl.h"

#endif