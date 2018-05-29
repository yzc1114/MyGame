#include"Hero.h"

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

	ATK = DEF = 20;
	HP = 1000;
	YellowKeys = 5;
	BlueKeys = 1;
	RedKeys = 1;
	coins = 0;


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
	targetTileCoord = map->tileCoordForPosition(heroPosition);

	//�����߽�
	if (heroPosition.x < 0
		|| targetTileCoord.x > map->getMapSize().width - 1
		|| targetTileCoord.y < 0
		|| targetTileCoord.y > map->getMapSize().height - 1)
	{
		return kwall;
	}

	//��ȡǽ�ڲ��Ӧ�����ͼ��ID
	int targetTileGID = Global::instance()->gameMap->WallLayer->getTileGIDAt(targetTileCoord);

	//���ͼ��ID��Ϊ0����ʾ��ǽ
	if (targetTileGID)
	{
		return kwall;
	}

	//�����Ʒ���Ӧ�����ͼ��ID
	targetTileGID = Global::instance()->gameMap->ItemLayer->getTileGIDAt(targetTileCoord);

	//���ͼ��ID��Ϊ0����ʾ����Ʒ
	if (targetTileGID) {
		pickUpItem();
		return kitem;
	}

	//����Ų��Ӧ�����ͼ��ID
	targetTileGID = Global::instance()->gameMap->DoorLayer->getTileGIDAt(targetTileCoord);
	
	//���ͼ��ID��Ϊ0����ʾ����
	if (targetTileGID) {
		openDoor(targetTileGID);
		return kdoor;
	}


	//��ù�����Ӧ�����ͼ��ID
	targetTileGID = Global::instance()->gameMap->enemyLayer->getTileGIDAt(targetTileCoord);

	//���ͼ��ID��Ϊ0����ʾ�е���
	if (targetTileGID) {
		enemy = new Enemy(targetTileGID); //��GID��ʼ����Եĵ���
		fight();
		return kenemy;
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
		//�޷����� showTip()
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
		//��������
		auto scene = StartScene::createStartScene();
		Director::getInstance()->replaceScene(scene);
	}


}

void Hero::MusicUpdate(float dt)
{
	if(isHeroFighting) CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/FightingBGS.mp3", false,3.0f);

}

void Hero::actWithNPC() {
	if (isTalking) { return; }
	
	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
	isTalking = true;
	int index = targetTileCoord.x + targetTileCoord.y * Global::instance()->gameMap->getMapSize().width;
	NPC *npc = Global::instance()->gameMap->npcDict.at(index);
	
	auto* pcontactMenu = MenuItemImage::create("ContactWindow.png", "ContactWindow.png", [&](Ref* ref) {
		Global::instance()->gameLayer->removeChildByTag(kZnpc);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
		isTalking = false;
																													});
	auto* contactLabel = MenuItemLabel::create(LabelTTF::create(npc->contactMessage, "Arial", 12), [&](Ref* ref) {
		Global::instance()->gameLayer->removeChildByTag(kZnpc);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ContactBGS.mp3");
		isTalking = false;
																													});
	auto* contactMenu = Menu::create(pcontactMenu, contactLabel, NULL);
	contactMenu->setPosition(32 * 10, 32 * 10);
	contactMenu->setScale(1.5f);
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
		//dosomething
		return;
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
		this->YellowKeys++;
		tempGameScene->refreshStatus(kZYellowKeys);
	}
	if (gid == 292) {
		//��Կ��
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		this->BlueKeys++;
		tempGameScene->refreshStatus(kZBlueKeys);
	}
	if (gid == 293) {
		//��Կ��
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/KeyBGS.mp3");
		this->RedKeys++;
		tempGameScene->refreshStatus(kZRedKeys);
	}
	if (gid == 275) {
		//����������2
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		this->ATK += 2;
		tempGameScene->refreshStatus(kZATK);
	}
	if (gid == 276) {
		//����������2
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		this->DEF += 2;
		tempGameScene->refreshStatus(kZDEF);
	}
	if (gid == 279) {
		//��Ѫƿ Ѫ����100
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		this->HP += 100;
		tempGameScene->refreshStatus(kZHP);
	}
	if (gid == 280) {
		//��Ѫƿ Ѫ����500
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ItemBGS.mp3");
		this->HP += 500;
		tempGameScene->refreshStatus(kZHP);
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

/*
void Hero::contactMenuCALLBACK(Ref* psender) {

	Global::instance()->gameLayer->removeChildByTag(kZnpc);

}
*/