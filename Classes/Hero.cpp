#include"Hero.h"
bool Hero::init()
{
	if (!Node::init())
	{
		return false;
	}
	//用精灵帧创建精灵
	heroSprite = Sprite::createWithSpriteFrame(AnimationControl::instance()->getAnimation(kdown)->getFrames().at(0)->getSpriteFrame());
	//设置锚点
	heroSprite->setAnchorPoint(Point::ZERO);

	this->addChild(heroSprite);
	//初始化英雄属性

	ATK = DEF = 10;
	HP = 1;
	YellowKeys = 0;
	BlueKeys = 0;
	RedKeys = 0;
	coins = 0;
	HavingAxes = false;


	isHeroMoving = isHeroFighting = isDoorOpening = isTalking = isAutomaticallyMoving = false;
	return true;
}

Hero::Hero()
{
	//设置Global hero值
	Global::instance()->hero = this;
}

Hero::~Hero()
{
}

//向指定方向移动一格
void Hero::move(HeroDirection direction)
{
	//若已经在走 则返回
	if (isHeroMoving || isHeroFighting || isDoorOpening|| isTalking || isAutomaticallyMoving)
	{
		return;
	}

	//移动的距离
	Point moveByPosition;

	//根据方向计算移动的距离
	switch (direction)
	{
	case kdown:
		moveByPosition = Point(0, -32);
		break;
	case kleft:
		moveByPosition = Point(-32, 0);
		break;
	case kright:
		moveByPosition = Point(32, 0);
		break;
	case kup:
		moveByPosition = Point(0, 32);
		break;
	}

	//计算目标坐标，用当前勇士坐标加上移动距离
	targetPosition = this->getPosition() + moveByPosition;

	//调用checkCollision检测碰撞类型，如果是墙壁、怪物、门，则只需要设置勇士的朝向
	CollisionType collisionType = checkCollision(targetPosition, false);

	if (collisionType == kwall
		|| collisionType == kenemy
		|| collisionType == knpc
		|| collisionType == kdoor
		)
	{
		setFaceDirection((HeroDirection)direction);
		return;
	}
	//若是传送门 则不动
	if (collisionType == kteleport) {
		return;
	}

	//heroSprite仅播放行走动画
	heroSprite->runAction(AnimationControl::instance()->createAnimate(direction));

	//如果不是捡起物品的话 将底部提示条设为普通状态
	if (collisionType != kitem) {
		Global::instance()->gameScene->showTipBarText(NORMAL_TIPBAR);
	}
	

	//主体进行位移，结束时调用onMoveDone方法 把方向信息传递给onMoveDone方法
	Action *action = Sequence::create(
		MoveBy::create(0.20f, moveByPosition),
		CallFuncN::create(CC_CALLBACK_1(Hero::onMoveDone, this, (int)direction)),
		NULL);

	this->runAction(action);

	isHeroMoving = true;

}

CollisionType Hero::checkCollision(Point heroPosition,bool isAutoMoving)
{
	auto map = Global::instance()->gameMap;
	TMXLayer* tempPtr;
	int targetTileGID = 0;
	targetTileCoord = map->tileCoordForPosition(heroPosition);

	//超出边界
	if (heroPosition.x < 0
		|| targetTileCoord.x > map->getMapSize().width - 1
		|| targetTileCoord.y < 0
		|| targetTileCoord.y > map->getMapSize().height - 1)
	{
		return kwall;
	}

	if (tempPtr = Global::instance()->gameMap->WallLayer) {
		//获取墙壁层对应坐标的图块ID
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//如果图块ID不为0，表示有墙
		if (targetTileGID)
		{
			return kwall;
		}
	}
	
	if (tempPtr = Global::instance()->gameMap->ItemLayer) {
	//获得物品层对应坐标的图块ID
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//如果图块ID不为0，表示有物品
		if (targetTileGID) {
			if (!isCheckingAutomaticallyMovePath) {
				pickUpItem();
			}
			return kitem;
		}
	}
	

	//获得门层对应坐标的图块ID
	if (tempPtr = Global::instance()->gameMap->DoorLayer) {
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//如果图块ID不为0，表示有门
		if (targetTileGID) {
			if(!isAutoMoving)
				openDoor(targetTileGID); //如果在自动行走 则不开门
			return kdoor;
		}
	}
		
	
	
	if (tempPtr = Global::instance()->gameMap->enemyLayer) {
		//获得怪物层对应坐标的图块ID
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//如果图块ID不为0，表示有敌人
		if (targetTileGID) {
			if (!isAutoMoving) {
				enemy = new Enemy(targetTileGID); //用GID初始化面对的敌人
				fight();
			}
			return kenemy;
		}
	}

	



	int index = targetTileCoord.x + targetTileCoord.y * Global::instance()->gameMap->getMapSize().width;

	//从npc字典中查询
	NPC *npc = Global::instance()->gameMap->npcDict.at(index);
	if (npc != NULL)
	{
		if(!isAutoMoving)
			actWithNPC();//如果正在自动移动 则不互动
		return knpc;
	}

	//从Teleport字典中查询
	Teleport *teleport = Global::instance()->gameMap->teleportDict.at(index);
	if (teleport != NULL)
	{
		if(!isAutoMoving)
			doTeleport(teleport);//如果正在自动移动 则不做传送
		return kteleport;
	}


	return knull;
}

void Hero::setFaceDirection(HeroDirection direction) {
	//设置面部朝向
	heroSprite->setTextureRect(Rect(0, 32 * direction, 32, 32));
}

void Hero::fight()
{
	//避免重复战斗
	if (isHeroFighting) {
		return;
	}

	bool isEnemyCanBeHurt = ifTheEnemyCanBeHurt(enemy);

	if (!isEnemyCanBeHurt) {
		Global::instance()->gameScene->showTipBarText("The enemy is too powerful");
		return;
	}

	//创建战斗用精灵
	FightingSprite = Sprite::create("sword.png", Rect(0, 0, 192, 192)); 
	//在map中加入该精灵
	Global::instance()->gameMap->addChild(FightingSprite,100,kZfighting);
	//战斗精灵位置计算 在英雄和敌人中点（因为动画里粒子效果偏左，所以将动画往右调整半个砖块，以使动画更严整地处于中间）
	Point FightingPosition = (this->getPosition() + targetPosition) / 2;
	FightingPosition.x += 16;
	//使用AnimationControl创建永远重复的战斗动作
	RepeatForever* FightingAction = RepeatForever::create(AnimationControl::instance()->createAnimate("Fighting"));
	//设置精灵位置
	FightingSprite->setPosition(FightingPosition);
	//运行该动作
	FightingSprite->runAction(FightingAction);
	//设置正在战斗为true
	isHeroFighting = true;
	//设置播放战斗音效的定时器（事先播放一次音效，避免见面秒杀没有音效）
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/FightingBGS.mp3");
	schedule(schedule_selector(Hero::MusicUpdate), 0.5f);
	//设置战斗的定时器
	schedule(schedule_selector(Hero::FightingUpdate), 0.2f);
	
	

}

bool Hero::ifTheEnemyCanBeHurt(Enemy* enemy)
{
	//若无法使敌人受伤 返回false
	if (this->ATK - enemy->DEF <= 0) {
		return false;
	}
	else {
		return true;
	}

}

void Hero::onMoveDone(Node* pTarget, int data) {
	//行走完以后 设置面部朝向为相应方向
	int direction = data;
	setFaceDirection((HeroDirection)direction);
	//将正在移动设置为false
	if (isHeroMoving) {
		isHeroMoving = false;
	}
	else if (isAutomaticallyMoving) {
		isAutomaticallyMoving = false;
		steps.pop();
		if (GameMap::tileCoordForPosition(this->getPosition()) == EndPointOfAutoMoving) {
			unschedule(schedule_selector(Hero::AutoMovingUpdate, this));
		}
	}

}

void Hero::doTeleport(Teleport *teleport)
{
	//将Global中保存的复活点设置为新地图的复活点
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/TransportBGS.mp3");
	Global::instance()->heroSpawnTileCoord = teleport->heroTileCoord;
	Global::instance()->gameLayer->switchMap(teleport->targetMap);
}

void Hero::FightingUpdate(float dt)
{
	int HeroATKminusEnemyDEF; //每次update敌人减少的血量
	int EnemyATKminusHeroDEF; //每次update英雄减少的血量


	//设置HeroATKminusEnemyDEF为英雄攻击减去敌人防御
	if (this->ATK - enemy->DEF < 0) {
		HeroATKminusEnemyDEF = 0;
	}
	else {
		HeroATKminusEnemyDEF = this->ATK - enemy->DEF;
	}

	//设置EnemyATKminusHeroDEF为敌人攻击减去英雄防御
	if (enemy->ATK - this->DEF < 0) {
		EnemyATKminusHeroDEF = 0;
	}
	else {
		EnemyATKminusHeroDEF = enemy->ATK - this->DEF;
	}

	enemy->HP -= HeroATKminusEnemyDEF; //敌人先受到攻击
	//若敌人死亡
	if (enemy->HP <= 0) { 
		//在地图中删除敌人
		Global::instance()->gameMap->enemyLayer->removeTileAt(targetTileCoord);
		//删除战斗用精灵
		Global::instance()->gameMap->removeChildByTag(kZfighting); 
		//取消定时器
		unschedule(schedule_selector(Hero::FightingUpdate)); 
		unschedule(schedule_selector(Hero::MusicUpdate));
		//设置isHeroFighting为false
		isHeroFighting = false;

		if (enemy->startGID == 211) 
		{
			Global::instance()->gameScene->GameVictory();
		}

	}

	this->HP -= EnemyATKminusHeroDEF; //英雄受到攻击
	Global::instance()->gameScene->refreshStatus(kZHP);

	//若英雄死亡
	if (this->HP <= 0) {
		log("gg");
		//删除战斗用精灵
		Global::instance()->gameMap->removeChildByTag(kZfighting);
		//取消定时器
		unschedule(schedule_selector(Hero::FightingUpdate));
		unschedule(schedule_selector(Hero::MusicUpdate));
		//设置isHeroFighting为false
		isHeroFighting = false;
		//将当前的地图从他的父节点删除 以便清理所有的内存
		Global::instance()->gameMap->removeFromParent();
		//重置Global
		Global::instance()->resetGlobal();
		//更换场景
		auto scene = StartScene::createStartScene();
		Director::getInstance()->replaceScene(scene);
	}

}

void Hero::MusicUpdate(float dt)
{
	if(isHeroFighting) CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/FightingBGS.mp3", false,3.0f);

}

//A*算法实现自动寻路
void Hero::moveToSomePointAutomatically(Vec2 TileCoord)
{
	//若已经在做事情 则返回
	if (isHeroMoving || isHeroFighting || isDoorOpening || isTalking || isAutomaticallyMoving)
	{
		return;
	}
	if (!ifReachable(TileCoord))return;//若目标不可达 则返回

	TMXLayer* wallLayer = Global::instance()->gameMap->WallLayer;
	
	Vec2 startPoint = GameMap::tileCoordForPosition(this->getPosition()); //some fucking crazy offsets; shit
	this->EndPointOfAutoMoving = TileCoord;
	struct node {
	private:
		Vec2 endp;
		Vec2 point;
		node* parentNode;
		int F;
		int G;
		int H;
	public:
		node() {}
		node(Vec2 p,Vec2 endP) : point(p),endp(endP) { F = H = INT_MAX; G = 0; parentNode = nullptr; }//只用来生成起点
		node(Vec2 p, node* parent) : point(p), parentNode(parent) { endp = parent->getEndP();  G = parent->getG() + 1; H = std::abs(endp.x - point.x) + std::abs(endp.y - point.y); F = G + H; }
		int getF() { return F; }
		int getG() { return G; }
		int getH() { return H; }
		node* getThisNodePtr() { return this; }
		void setF(int f) { F = f; }
		void setG(int g) { G = g; }
		void setH(int h) { H = h; }
		void setParent(node* p) { parentNode = p; };
		Vec2 getVec2() { return point; }
		Vec2 getEndP() { return endp; }
		node* getParent() { return parentNode; }
		bool operator<(node other) { return F < other.getF(); }
		bool operator==(node other) {
			return point == other.getVec2()
				&& parentNode == other.getParent()
				&& F == other.getF()
				&& G == other.getG()
				&& H == other.getH();
		}
	};
	std::list<node> openList;
	std::list<node> closeList;
	openList.push_back(node(startPoint, EndPointOfAutoMoving));
	std::list<node>::iterator currentNode = openList.begin();
	
	isCheckingAutomaticallyMovePath = true;
	while (true) {
		
		//查找F最小的node
		currentNode = openList.begin();
		if (openList.empty())return;
		for (auto iter = openList.begin(); iter != openList.end();iter++) {
			if (*iter < *currentNode) {
				currentNode = iter; //用最小F的node作为当前node
			}
		}
		//输出调试
		std::string str;
		str = " x : " + std::to_string(currentNode->getVec2().x) + " y : " + std::to_string(currentNode->getVec2().y) + "\n";
		log(str.c_str());

		closeList.push_back(*currentNode);
		openList.erase(currentNode);
		currentNode = closeList.end();
		currentNode--;
		
		Vec2 Left = currentNode->getVec2() - Vec2(1, 0);
		
		Vec2 Up = currentNode->getVec2() - Vec2(0, 1);
		
		Vec2 Right = currentNode->getVec2() + Vec2(1, 0);
		
		Vec2 Down = currentNode->getVec2() + Vec2(0, 1);
		
		std::vector<Vec2> points = { Left,Up,Right,Down };
		
		for (int i = 0; i < 4; i++) {
			Vec2 p = points.at(i);
			bool pIsInCloseList = false;
			//查找p是否在CloseList内
			for (auto v : closeList) {
				if (v.getVec2() == p) {
					pIsInCloseList = true;
				}
			}//查找完毕
			//它是可抵达的并且它不在 close list 中 
			if (ifReachable(p) && !pIsInCloseList) {
				//查找p是否在openList内
				bool pIsInOpenList = false;
				std::list<node>::iterator iter; //用来存储找到的node
				for (iter = openList.begin(); iter != openList.end() ; iter++){
					if (iter->getVec2() == p) {
						pIsInOpenList = true;
						break;
					}
				}//查找完毕

				if (!pIsInOpenList) { //如果这个点不在openlist里面 把它加入 open list ，并且把当前方格设置为它的父亲，记录该方格的 F ，G 和 H 值。
					openList.push_back(node(p, currentNode->getThisNodePtr()));
				}
				else {
					//检查这条路径(即经由当前方格到达它那里) 是否更好，用 G 值作参考。更小的 G 值表示这是更好的路径。如果是这样，把它的父亲设置为当前方格，并重新计算它的 G 和 F 值。
					if (currentNode->getG() + 1 < iter->getG()) { //若这条路径(即经由当前方格到达它那里)更好
						iter->setParent(currentNode->getThisNodePtr());
						iter->setG(currentNode->getG() + 1);
						iter->setF(iter->getH() + iter->getG());
					}
				}


			}
		}


		//检测终点是否被加入
		bool added = false;
		for (auto iter : openList) {
			if (iter.getVec2() == EndPointOfAutoMoving) {
				added = true; //被加入了
			}
		}
		if (added)break;

	}
	log("ok");
	isCheckingAutomaticallyMovePath = false;

	std::list<node>::iterator iter;
	for (iter = openList.begin(); iter != openList.end();iter++){
		if (iter->getVec2() == EndPointOfAutoMoving) {
			break;
		}
	}
	node* nodeP = iter->getThisNodePtr();

	//记录下所有的步数 用stack记录 使用时pop出来即可
	while (!steps.empty()) {
		steps.pop();
	}
	while (nodeP->getParent() != nullptr) {
		Vec2 p = nodeP->getVec2();
		steps.push(p);
		double x = p.x;
		double y = p.y;
		log("%lf %lf\n",x,y);
		nodeP = nodeP->getParent();
	}

	schedule(schedule_selector(Hero::AutoMovingUpdate, this));

}

void Hero::AutoMovingUpdate(float dt)
{
	if (!steps.empty()) {
		//如果还在自动行进中 则代表没有到达目标 继续循环
		if (isAutomaticallyMoving) {
			return;
		}
		else if (!isAutomaticallyMoving) { //如果没有自动行进中 则代表已经到达目标位置 将目前的point从steps中pop出来 并打破无限循环 进行下一个位置的移动
			Vec2 point = steps.top();
			//移动至point 逻辑 ：如果碰到任何东西 立刻停止前进 使用循环确保英雄能够移动到下一个位置
			AutoMovingTest type = moveToPointAroundHeroWhenAutomaticallyMoving(point);
			//如果检测到撞到东西 或者出错 则立刻停止更新
			if (type == hitSomethingBad || type == badThingHappened)unschedule(schedule_selector(Hero::AutoMovingUpdate, this));;
		}
	}
	else {
		log("call AutoMovingUpdate even when steps is empty");
	}
	
}

bool Hero::ifReachable(Vec2 tilecoord)
{
	isCheckingAutomaticallyMovePath = true;
	CollisionType type = checkCollision(GameMap::positionForTileCoord(tilecoord), true);
	isCheckingAutomaticallyMovePath = false;
	if (type != knull) {
		if (type == kitem) {
			return true;
		}
		else {
			return false;
		}
		
	}
	else {
		return true;
	}

}

AutoMovingTest Hero::moveToPointAroundHeroWhenAutomaticallyMoving(Vec2 point)
{
	//若已经在走 则返回
	if (isAutomaticallyMoving)
	{
		return heroIsAutoMoving;
	}
	//英雄当前坐标
	Vec2 heroPosition = GameMap::tileCoordForPosition(this->getPosition()); //some fucking crazy offsets; shit
	//移动的方向单位向量 末位置减去初位置向量 注意：使用Tile坐标
	Vec2 IdentityDirection = point - heroPosition;
	//移动的向量 注意：使用cocos像素坐标
	Vec2 moveByPosition;
	//英雄移动方向
	HeroDirection direction;
	if (IdentityDirection == Vec2(1, 0)) {
		direction = kright;
	}
	else if (IdentityDirection == Vec2(-1, 0)) {
		direction = kleft;
	}
	else if (IdentityDirection == Vec2(0, 1)) {
		direction = kdown;
	}
	else if (IdentityDirection == Vec2(0, -1)) {
		direction = kup;
	}
	else {
		log("HeroAutomatically moving went some wrong");
		return badThingHappened;
	}
	
	//根据方向计算移动的距离
	switch (direction)
	{
	case kdown:
		moveByPosition = Point(0, -32);
		break;
	case kleft:
		moveByPosition = Point(-32, 0);
		break;
	case kright:
		moveByPosition = Point(32, 0);
		break;
	case kup:
		moveByPosition = Point(0, 32);
		break;
	}

	//计算目标坐标，用当前勇士坐标加上移动距离 为像素坐标
	NextPoint = this->getPosition() + moveByPosition;

	//调用checkCollision检测碰撞类型，如果是墙壁、怪物、门，则只需要设置勇士的朝向
	CollisionType collisionType = checkCollision(NextPoint,true);

	if (collisionType != knull) //如果撞到任何东西 立刻返回hitSomethingBad
	{
		if(collisionType != kitem)
			return hitSomethingBad;
	}

	//heroSprite仅播放行走动画
	heroSprite->runAction(AnimationControl::instance()->createAnimate(direction));

	//主体进行位移，结束时调用onMoveDone方法 把方向信息传递给onMoveDone方法
	Action *action = Sequence::create(
		MoveBy::create(0.20f, moveByPosition),
		CallFuncN::create(CC_CALLBACK_1(Hero::onMoveDone, this, (int)direction)),
		NULL);

	this->runAction(action);

	isAutomaticallyMoving = true;

	return moveCommondExecuted;
}

void Hero::actWithNPC() {
	if (isTalking) { return; }              //如果正在BB，那么就在此地不要走动
	
	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");        //播放BB音效
	isTalking = true;																			//表示正在BB
	
	//计算目标唯一坐标并读取该NPC数据
	int index = targetTileCoord.x + targetTileCoord.y * Global::instance()->gameMap->getMapSize().width;    
	NPC *npc = Global::instance()->gameMap->npcDict.at(index);
	
	//创建菜单外观与回调的Lambda函数
	auto* pcontactMenu = MenuItemImage::create("ContactWindow.png", "ContactWindow.png", [&](Ref* ref) {
		Global::instance()->gameLayer->removeChildByTag(kZnpc);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
		isTalking = false;
																													});
	
	//创建标签，调整范围至菜单内并创建回调的Lambda函数
	auto contactLabel = LabelTTF::create(npc->contactMessage, "Arial", 13);
	contactLabel->setDimensions(Size(170, 190));
	auto* pcontactLabel = MenuItemLabel::create(contactLabel, [&](Ref* ref) {
		Global::instance()->gameLayer->removeChildByTag(kZnpc);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
		isTalking = false;
																													});
	
	//建立菜单并添加节点
	auto* contactMenu = Menu::create(pcontactMenu, pcontactLabel, NULL);
	contactMenu->setPosition(32 * 14, 32 * 12);
	contactMenu->setScale(1.8f);
	Global::instance()->gameLayer->addChild(contactMenu, 250, kZnpc);


}

void Hero::openDoor(int gid) {
	//如果正在开门
	if (isDoorOpening) {
		return;
	}
	//保存这个门的GID；
	targetDoorGID = gid;
	//259 黄门
	//260 蓝门
	//261 红门
	//262 boss门
	if (gid == 259) { //黄门
		if (YellowKeys > 0) {
			YellowKeys--;
			Global::instance()->gameScene->refreshStatus(kZYellowKeys);
		}
		else {
			return;
		}
	}
	if (gid == 260) { //蓝门
		if (BlueKeys > 0) {
			BlueKeys--;
			Global::instance()->gameScene->refreshStatus(kZBlueKeys);
		}
		else {
			return;
		}
	}
	if (gid == 261) { //红门
		if (RedKeys > 0) {
			RedKeys--;
			Global::instance()->gameScene->refreshStatus(kZRedKeys);
		}
		else {
			return;
		}
	} 

	if (gid == 262) {
		if (HavingAxes) {
			HavingAxes = false;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/AxesDamageBGS.mp3");
			isDoorOpening = true;
			schedule(schedule_selector(Hero::DoorOpeningUpdate), 0.1f);
			return;
		}
		else {
			return;
		}
		
	}
	//设置正在开门为TRUE
	isDoorOpening = true;

	//更新门的动画
	schedule(schedule_selector(Hero::DoorOpeningUpdate), 0.1f);

	//播放开门音效
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/OpenBGS.mp3");
}

void Hero::pickUpItem() {

	int gid = Global::instance()->gameMap->ItemLayer->getTileGIDAt(targetTileCoord);

	GameScene* tempGameScene = Global::instance()->gameScene;

	if (gid == 291) {
		//黄钥匙
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a yellow key");
		this->YellowKeys++;
		tempGameScene->refreshStatus(kZYellowKeys);
	}
	if (gid == 292) {
		//蓝钥匙
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a blue key");
		this->BlueKeys++;
		tempGameScene->refreshStatus(kZBlueKeys);
	}
	if (gid == 293) {
		//红钥匙
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a red key");
		this->RedKeys++;
		tempGameScene->refreshStatus(kZRedKeys);
	}
	if (gid == 275) {
		//攻击力增加2
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up an Attack Stone , Attack plus 2");
		this->ATK += 2;
		tempGameScene->refreshStatus(kZATK);
	}
	if (gid == 276) {
		//防御力增加2
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a Defence Stone , Defence plus 2");
		this->DEF += 2;
		tempGameScene->refreshStatus(kZDEF);
	}
	if (gid == 279) {
		//红血瓶 血量加100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a small liquid medicine , HP plus 100");
		this->HP += 100;
		tempGameScene->refreshStatus(kZHP);
	}
	if (gid == 280) {
		//蓝血瓶 血量加300
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a big liquid medicine , HP plus 300");
		this->HP += 300;
		tempGameScene->refreshStatus(kZHP);
	}
	if (gid == 308) {
		//镐子 可以挖门
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/PickAxesBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a Pickaxe");
		this->HavingAxes = true;
	}
	if (gid == 326) {
		//无尽之刃 攻击加100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/WeaponBGS.mp3");
		tempGameScene->showTipBarText("You have picked up the Infinity Blade");
		this->ATK += 100;
		tempGameScene->refreshStatus(kZATK);
	}
	if (gid == 332) {
		//多兰之盾 防御加100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/WeaponBGS.mp3");
		tempGameScene->showTipBarText("You have picked up the Dolan shield");
		this->DEF += 100;
		tempGameScene->refreshStatus(kZDEF);
	}
	if (gid == 311) {
		//跳楼符 跳到下面一层（即隐藏的17层）
		Global::instance()->gameMap->ItemLayer->removeTileAt(targetTileCoord);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/TransportBGS.mp3");
		Global::instance()->heroSpawnTileCoord = Point(6, 11);
		Global::instance()->gameLayer->switchMap(17);
		
		return;
	}

	

	//直接删除图块
	Global::instance()->gameMap->ItemLayer->removeTileAt(targetTileCoord);

}

void Hero::DoorOpeningUpdate(float dt) {
	//下一个格子的GID
	int NextGid = Global::instance()->gameMap->DoorLayer->getTileGIDAt(targetTileCoord) + 4;
	//如果差值大于12 则表明开门的四个图片已加载过
	if (NextGid - targetDoorGID > 12) {
		//删除该砖块
		Global::instance()->gameMap->DoorLayer->removeTileAt(targetTileCoord);
		//取消定时器
		unschedule(schedule_selector(Hero::DoorOpeningUpdate));
		//设置正在开门为false
		isDoorOpening = false;
	}
	else {
		//若小于12 则更新砖块图片
		Global::instance()->gameMap->DoorLayer->setTileGID(NextGid,targetTileCoord);
	}
}

