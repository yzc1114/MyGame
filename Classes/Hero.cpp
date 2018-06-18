#include"Hero.h"
#include<list>
#include<fstream>
bool Hero::init()
{
	if (!Node::init())
	{
		return false;
	}
	//�þ���֡��������
	heroSprite = Sprite::createWithSpriteFrame(AnimationControl::instance()->getAnimation(kdown)->getFrames().at(0)->getSpriteFrame());
	//����ê��
	heroSprite->setAnchorPoint(Point::ZERO);

	this->addChild(heroSprite);
	//��ʼ��Ӣ������

	ATK = DEF = 10;
	HP = 1;
	YellowKeys = 0;
	BlueKeys = 0;
	RedKeys = 0;
	coins = 0;
	HavingAxes = false;


	isHeroMoving = isHeroFighting = isDoorOpening = isTalking = false;
	return true;
}

Hero::Hero()
{
	//����Global heroֵ
	Global::instance()->hero = this;
}

Hero::~Hero()
{
}

//��ָ�������ƶ�һ��
void Hero::move(HeroDirection direction)
{
	//���Ѿ����� �򷵻�
	if (isHeroMoving || isHeroFighting || isDoorOpening|| isTalking)
	{
		return;
	}

	//�ƶ��ľ���
	Point moveByPosition;

	//���ݷ�������ƶ��ľ���
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

	//����Ŀ�����꣬�õ�ǰ��ʿ��������ƶ�����
	targetPosition = this->getPosition() + moveByPosition;

	//����checkCollision�����ײ���ͣ������ǽ�ڡ�����ţ���ֻ��Ҫ������ʿ�ĳ���
	CollisionType collisionType = checkCollision(targetPosition);

	if (collisionType == kwall
		|| collisionType == kenemy
		|| collisionType == knpc
		|| collisionType == kdoor
		)
	{
		setFaceDirection((HeroDirection)direction);
		return;
	}
	//���Ǵ����� �򲻶�
	if (collisionType == kteleport) {
		return;
	}

	//heroSprite���������߶���
	heroSprite->runAction(AnimationControl::instance()->createAnimate(direction));

	//������Ǽ�����Ʒ�Ļ� ���ײ���ʾ����Ϊ��ͨ״̬
	if (collisionType != kitem) {
		Global::instance()->gameScene->showTipBarText(NORMAL_TIPBAR);
	}
	

	//�������λ�ƣ�����ʱ����onMoveDone���� �ѷ�����Ϣ���ݸ�onMoveDone����
	Action *action = Sequence::create(
		MoveBy::create(0.20f, moveByPosition),
		CallFuncN::create(CC_CALLBACK_1(Hero::onMoveDone, this, (int)direction)),
		NULL);

	this->runAction(action);

	isHeroMoving = true;
}

CollisionType Hero::checkCollision(Point heroPosition)
{
	auto map = Global::instance()->gameMap;
	TMXLayer* tempPtr;
	int targetTileGID = 0;
	targetTileCoord = map->tileCoordForPosition(heroPosition);

	//�����߽�
	if (heroPosition.x < 0
		|| targetTileCoord.x > map->getMapSize().width - 1
		|| targetTileCoord.y < 0
		|| targetTileCoord.y > map->getMapSize().height - 1)
	{
		return kwall;
	}

	if (tempPtr = Global::instance()->gameMap->WallLayer) {
		//��ȡǽ�ڲ��Ӧ�����ͼ��ID
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//���ͼ��ID��Ϊ0����ʾ��ǽ
		if (targetTileGID)
		{
			return kwall;
		}
	}
	
	if (tempPtr = Global::instance()->gameMap->ItemLayer) {
	//�����Ʒ���Ӧ�����ͼ��ID
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//���ͼ��ID��Ϊ0����ʾ����Ʒ
		if (targetTileGID) {
			pickUpItem();
			return kitem;
		}
	}
	

	//����Ų��Ӧ�����ͼ��ID
	if (tempPtr = Global::instance()->gameMap->DoorLayer) {
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//���ͼ��ID��Ϊ0����ʾ����
		if (targetTileGID) {
			openDoor(targetTileGID);
			return kdoor;
		}
	}
		
	
	
	if (tempPtr = Global::instance()->gameMap->enemyLayer) {
		//��ù�����Ӧ�����ͼ��ID
		targetTileGID = tempPtr->getTileGIDAt(targetTileCoord);
		//���ͼ��ID��Ϊ0����ʾ�е���
		if (targetTileGID) {
			enemy = new Enemy(targetTileGID); //��GID��ʼ����Եĵ���
			fight();
			return kenemy;
		}
	}

	



	int index = targetTileCoord.x + targetTileCoord.y * Global::instance()->gameMap->getMapSize().width;

	//��npc�ֵ��в�ѯ
	NPC *npc = Global::instance()->gameMap->npcDict.at(index);
	if (npc != NULL)
	{
		actWithNPC();
		return knpc;
	}

	//��Teleport�ֵ��в�ѯ
	Teleport *teleport = Global::instance()->gameMap->teleportDict.at(index);
	if (teleport != NULL)
	{
		doTeleport(teleport);
		return kteleport;
	}


	return knull;
}


void Hero::setFaceDirection(HeroDirection direction) {
	//�����沿����
	heroSprite->setTextureRect(Rect(0, 32 * direction, 32, 32));
}

void Hero::fight()
{
	//�����ظ�ս��
	if (isHeroFighting) {
		return;
	}

	int targetTileGID = Global::instance()->gameMap->WallLayer->getTileGIDAt(targetTileCoord);

	bool isEnemyCanBeHurt = ifTheEnemyCanBeHurt(enemy);

	if (!isEnemyCanBeHurt) {
		Global::instance()->gameScene->showTipBarText("The enemy is too powerful");
		return;
	}

	//����ս���þ���
	FightingSprite = Sprite::create("sword.png", Rect(0, 0, 192, 192)); 
	//��map�м���þ���
	Global::instance()->gameMap->addChild(FightingSprite,100,kZfighting);
	//ս������λ�ü��� ��Ӣ�ۺ͵����е㣨��Ϊ����������Ч��ƫ�����Խ��������ҵ������ש�飬��ʹ�����������ش����м䣩
	Point FightingPosition = (this->getPosition() + targetPosition) / 2;
	FightingPosition.x += 16;
	//ʹ��AnimationControl������Զ�ظ���ս������
	RepeatForever* FightingAction = RepeatForever::create(AnimationControl::instance()->createAnimate("Fighting"));
	//���þ���λ��
	FightingSprite->setPosition(FightingPosition);
	//���иö���
	FightingSprite->runAction(FightingAction);
	//��������ս��Ϊtrue
	isHeroFighting = true;
	//���ò���ս����Ч�Ķ�ʱ�������Ȳ���һ����Ч�����������ɱû����Ч��
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/FightingBGS.mp3");
	schedule(schedule_selector(Hero::MusicUpdate), 0.5f);
	//����ս���Ķ�ʱ��
	schedule(schedule_selector(Hero::FightingUpdate), 0.2f);
	
	

}

bool Hero::ifTheEnemyCanBeHurt(Enemy* enemy)
{
	//���޷�ʹ�������� ����false
	if (this->ATK - enemy->DEF <= 0) {
		return false;
	}
	else {
		return true;
	}

}

void Hero::onMoveDone(Node* pTarget, int data) {
	//�������Ժ� �����沿����Ϊ��Ӧ����
	int direction = data;
	setFaceDirection((HeroDirection)direction);
	//�������ƶ�����Ϊfalse
	isHeroMoving = false;

}

void Hero::doTeleport(Teleport *teleport)
{
	//��Global�б���ĸ��������Ϊ�µ�ͼ�ĸ����
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/TransportBGS.mp3");
	Global::instance()->heroSpawnTileCoord = teleport->heroTileCoord;
	Global::instance()->gameLayer->switchMap(teleport->targetMap);
}

void Hero::FightingUpdate(float dt)
{
	int HeroATKminusEnemyDEF; //ÿ��update���˼��ٵ�Ѫ��
	int EnemyATKminusHeroDEF; //ÿ��updateӢ�ۼ��ٵ�Ѫ��


	//����HeroATKminusEnemyDEFΪӢ�۹�����ȥ���˷���
	if (this->ATK - enemy->DEF < 0) {
		HeroATKminusEnemyDEF = 0;
	}
	else {
		HeroATKminusEnemyDEF = this->ATK - enemy->DEF;
	}

	//����EnemyATKminusHeroDEFΪ���˹�����ȥӢ�۷���
	if (enemy->ATK - this->DEF < 0) {
		EnemyATKminusHeroDEF = 0;
	}
	else {
		EnemyATKminusHeroDEF = enemy->ATK - this->DEF;
	}

	enemy->HP -= HeroATKminusEnemyDEF; //�������ܵ�����
	//����������
	if (enemy->HP <= 0) { 
		//�ڵ�ͼ��ɾ������
		Global::instance()->gameMap->enemyLayer->removeTileAt(targetTileCoord);
		//ɾ��ս���þ���
		Global::instance()->gameMap->removeChildByTag(kZfighting); 
		//ȡ����ʱ��
		unschedule(schedule_selector(Hero::FightingUpdate)); 
		unschedule(schedule_selector(Hero::MusicUpdate));
		//����isHeroFightingΪfalse
		isHeroFighting = false;
	}

	this->HP -= EnemyATKminusHeroDEF; //Ӣ���ܵ�����
	Global::instance()->gameScene->refreshStatus(kZHP);

	//��Ӣ������
	if (this->HP <= 0) {
		log("gg");
		//ɾ��ս���þ���
		Global::instance()->gameMap->removeChildByTag(kZfighting);
		//ȡ����ʱ��
		unschedule(schedule_selector(Hero::FightingUpdate));
		unschedule(schedule_selector(Hero::MusicUpdate));
		//����isHeroFightingΪfalse
		isHeroFighting = false;
		//����Global
		Global::instance()->resetGlobal();
		//��������
		auto scene = StartScene::createStartScene();
		Director::getInstance()->replaceScene(scene);
	}

}

void Hero::MusicUpdate(float dt)
{
	if(isHeroFighting) CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/FightingBGS.mp3", false,3.0f);

}

//A*�㷨ʵ���Զ�Ѱ·
void Hero::moveToSomePointAutomatically(Vec2 TileCoord)
{
	if (!ifReachable(TileCoord))return;

	TMXLayer* wallLayer = Global::instance()->gameMap->WallLayer;
	
	Vec2 startPoint = GameMap::tileCoordForPosition(this->getPosition()); //some fucking crazy offsets; shit
	Vec2 endPoint = TileCoord;
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
		node(Vec2 p,Vec2 endP) : point(p),endp(endP) { F = H = INT_MAX; G = 0; parentNode = nullptr; }//ֻ�����������
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
	openList.push_back(node(startPoint,endPoint));
	std::list<node>::iterator currentNode = openList.begin();

	//�������
	std::ofstream out("d:\\out.txt");
	

	while (true) {
		//����F��С��node
		currentNode = openList.begin();
		if (openList.empty())return;
		for (auto iter = openList.begin(); iter != openList.end();iter++) {
			if (*iter < *currentNode) {
				currentNode = iter; //����СF��node��Ϊ��ǰnode
			}
		}
		//�������
		std::string str;
		str = " x : " + std::to_string(currentNode->getVec2().x) + " y : " + std::to_string(currentNode->getVec2().y) + "\n";
		log(str.c_str());

		//�������������Ͻǰ׿�

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
			//����p�Ƿ���CloseList��
			for (auto v : closeList) {
				if (v.getVec2() == p) {
					pIsInCloseList = true;
				}
			}//�������
			//���ǿɵִ�Ĳ��������� close list �� 
			if (ifReachable(p) && !pIsInCloseList) {
				//����p�Ƿ���openList��
				bool pIsInOpenList = false;
				std::list<node>::iterator iter; //�����洢�ҵ���node
				for (iter = openList.begin(); iter != openList.end() ; iter++){
					if (iter->getVec2() == p) {
						pIsInOpenList = true;
						break;
					}
				}//�������

				if (!pIsInOpenList) { //�������㲻��openlist���� �������� open list �����Ұѵ�ǰ��������Ϊ���ĸ��ף���¼�÷���� F ��G �� H ֵ��
					openList.push_back(node(p, currentNode->getThisNodePtr()));
				}
				else {
					//�������·��(�����ɵ�ǰ���񵽴�������) �Ƿ���ã��� G ֵ���ο�����С�� G ֵ��ʾ���Ǹ��õ�·��������������������ĸ�������Ϊ��ǰ���񣬲����¼������� G �� F ֵ��
					if (currentNode->getG() + 1 < iter->getG()) { //������·��(�����ɵ�ǰ���񵽴�������)����
						iter->setParent(currentNode->getThisNodePtr());
						iter->setG(currentNode->getG() + 1);
						iter->setF(iter->getH() + iter->getG());
					}
				}


			}
		}


		//����յ��Ƿ񱻼���
		bool added = false;
		for (auto iter : openList) {
			if (iter.getVec2() == endPoint) {
				added = true; //��������
			}
		}
		if (added)break;

	}
	log("ok");

	std::list<node>::iterator iter;
	for (iter = openList.begin(); iter != openList.end();iter++){
		if (iter->getVec2() == endPoint) {
			break;
		}
	}
	node* nodeP = iter->getThisNodePtr();
	while (nodeP->getParent() != nullptr) {
		Vec2 p = nodeP->getVec2();
		double x = p.x;
		double y = p.y;
		log("%lf %lf\n",x,y);
		nodeP = nodeP->getParent();
	}

}

bool Hero::ifReachable(Vec2 tilecoord)
{
	if (Global::instance()->gameMap == Global::instance()->GameMaps[1]) {
		
	}
	if (auto tempPtr = Global::instance()->gameMap->WallLayer) {
		//��ȡǽ�ڲ��Ӧ�����ͼ��ID
		int targetTileGID = tempPtr->getTileGIDAt(tilecoord);
		//���ͼ��ID��Ϊ0����ʾ��ǽ
		if (targetTileGID)
		{
			return false;
		}
		else {
			return true;
		}
	}
}

void Hero::actWithNPC() {
	if (isTalking) { return; }              //�������BB����ô���ڴ˵ز�Ҫ�߶�
	
	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");        //����BB��Ч
	isTalking = true;																			//��ʾ����BB
	
	//����Ŀ��Ψһ���겢��ȡ��NPC����
	int index = targetTileCoord.x + targetTileCoord.y * Global::instance()->gameMap->getMapSize().width;    
	NPC *npc = Global::instance()->gameMap->npcDict.at(index);
	
	//�����˵������ص���Lambda����
	auto* pcontactMenu = MenuItemImage::create("ContactWindow.png", "ContactWindow.png", [&](Ref* ref) {
		Global::instance()->gameLayer->removeChildByTag(kZnpc);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
		isTalking = false;
																													});
	
	//������ǩ��������Χ���˵��ڲ������ص���Lambda����
	auto contactLabel = LabelTTF::create(npc->contactMessage, "Arial", 13);
	contactLabel->setDimensions(Size(170, 190));
	auto* pcontactLabel = MenuItemLabel::create(contactLabel, [&](Ref* ref) {
		Global::instance()->gameLayer->removeChildByTag(kZnpc);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
		isTalking = false;
																													});
	
	//�����˵�����ӽڵ�
	auto* contactMenu = Menu::create(pcontactMenu, pcontactLabel, NULL);
	contactMenu->setPosition(32 * 14, 32 * 12);
	contactMenu->setScale(1.8f);
	Global::instance()->gameLayer->addChild(contactMenu, 250, kZnpc);


}

void Hero::openDoor(int gid) {
	//������ڿ���
	if (isDoorOpening) {
		return;
	}
	//��������ŵ�GID��
	targetDoorGID = gid;
	//259 ����
	//260 ����
	//261 ����
	//262 boss��
	if (gid == 259) { //����
		if (YellowKeys > 0) {
			YellowKeys--;
			Global::instance()->gameScene->refreshStatus(kZYellowKeys);
		}
		else {
			return;
		}
	}
	if (gid == 260) { //����
		if (BlueKeys > 0) {
			BlueKeys--;
			Global::instance()->gameScene->refreshStatus(kZBlueKeys);
		}
		else {
			return;
		}
	}
	if (gid == 261) { //����
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
	//�������ڿ���ΪTRUE
	isDoorOpening = true;

	//�����ŵĶ���
	schedule(schedule_selector(Hero::DoorOpeningUpdate), 0.1f);

	//���ſ�����Ч
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/OpenBGS.mp3");
}

void Hero::pickUpItem() {

	int gid = Global::instance()->gameMap->ItemLayer->getTileGIDAt(targetTileCoord);

	GameScene* tempGameScene = Global::instance()->gameScene;

	if (gid == 291) {
		//��Կ��
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a yellow key");
		this->YellowKeys++;
		tempGameScene->refreshStatus(kZYellowKeys);
	}
	if (gid == 292) {
		//��Կ��
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a blue key");
		this->BlueKeys++;
		tempGameScene->refreshStatus(kZBlueKeys);
	}
	if (gid == 293) {
		//��Կ��
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a red key");
		this->RedKeys++;
		tempGameScene->refreshStatus(kZRedKeys);
	}
	if (gid == 275) {
		//����������2
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up an Attack Stone , Attack plus 2");
		this->ATK += 2;
		tempGameScene->refreshStatus(kZATK);
	}
	if (gid == 276) {
		//����������2
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a Defence Stone , Defence plus 2");
		this->DEF += 2;
		tempGameScene->refreshStatus(kZDEF);
	}
	if (gid == 279) {
		//��Ѫƿ Ѫ����100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a small liquid medicine , HP plus 100");
		this->HP += 100;
		tempGameScene->refreshStatus(kZHP);
	}
	if (gid == 280) {
		//��Ѫƿ Ѫ����300
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a big liquid medicine , HP plus 300");
		this->HP += 300;
		tempGameScene->refreshStatus(kZHP);
	}
	if (gid == 308) {
		//���� ��������
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/PickAxesBGS.mp3");
		tempGameScene->showTipBarText("You have picked up a Pickaxe");
		this->HavingAxes = true;
	}
	if (gid == 326) {
		//�޾�֮�� ������100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/WeaponBGS.mp3");
		tempGameScene->showTipBarText("You have picked up the Infinity Blade");
		this->ATK += 100;
		tempGameScene->refreshStatus(kZATK);
	}
	if (gid == 332) {
		//����֮�� ������100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/WeaponBGS.mp3");
		tempGameScene->showTipBarText("You have picked up the Dolan shield");
		this->DEF += 100;
		tempGameScene->refreshStatus(kZDEF);
	}
	if (gid == 311) {
		//��¥�� ��������һ�㣨�����ص�17�㣩
		Global::instance()->gameMap->ItemLayer->removeTileAt(targetTileCoord);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/TransportBGS.mp3");
		Global::instance()->heroSpawnTileCoord = Point(6, 11);
		Global::instance()->gameLayer->switchMap(17);
		
		return;
	}

	

	//ֱ��ɾ��ͼ��
	Global::instance()->gameMap->ItemLayer->removeTileAt(targetTileCoord);

}

void Hero::DoorOpeningUpdate(float dt) {
	//��һ�����ӵ�GID
	int NextGid = Global::instance()->gameMap->DoorLayer->getTileGIDAt(targetTileCoord) + 4;
	//�����ֵ����12 ��������ŵ��ĸ�ͼƬ�Ѽ��ع�
	if (NextGid - targetDoorGID > 12) {
		//ɾ����ש��
		Global::instance()->gameMap->DoorLayer->removeTileAt(targetTileCoord);
		//ȡ����ʱ��
		unschedule(schedule_selector(Hero::DoorOpeningUpdate));
		//�������ڿ���Ϊfalse
		isDoorOpening = false;
	}
	else {
		//��С��12 �����ש��ͼƬ
		Global::instance()->gameMap->DoorLayer->setTileGID(NextGid,targetTileCoord);
	}
}

