#include "HelpLayer.h"

HelpLayer::HelpLayer()
{
}


HelpLayer::~HelpLayer()
{
}

bool HelpLayer::init()
{
	if (!Layer::init()) {
		return nullptr;
	}

	Global::instance()->controlLayer->isHelping = true;

	Layer* BGLayer = LayerColor::create(Color4B::BLACK);
	this->addChild(BGLayer, 1);

	auto theListener = EventListenerTouchOneByOne::create();
	theListener->setSwallowTouches(true);
	theListener->onTouchBegan = [&](Touch* touch, Event* event) {return true;};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(theListener, BGLayer);

	auto exit = MenuItemImage::create("Close/CloseNormal.png", "Close/CloseSelected.png", [&](Ref* ref) {
		this->removeFromParent(); 
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
		Global::instance()->controlLayer->isHelping = false;});
	
	auto exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(20, 416);
	this->addChild(exitButton, 2);

	auto* LastPage = MenuItemImage::create("LastPage.png", "LastPage.png", CC_CALLBACK_1(HelpLayer::LastPageCallBack, this));
	auto* NextPage = MenuItemImage::create("NextPage.png", "NextPage.png", CC_CALLBACK_1(HelpLayer::NextPageCallBack, this));
	auto* pageChange = Menu::create(LastPage, NextPage, NULL);
	pageChange->alignItemsVerticallyWithPadding(10);
	pageChange->setPosition(560, 220);
	this->addChild(pageChange, 2);

	HelpLayer::addIllustration();
	return true;

}

void HelpLayer::addIllustration()
{
	//µÚÒ»Ò³Í¼¼ø
	
	
	Sprite* Sprite_Slime = Sprite::create("enemy.png", Rect(0, 8 * 32, 32, 32));
	Sprite* Sprite_Bat = Sprite::create("enemy.png", Rect(0, 5 * 32, 32, 32));
	Sprite* Sprite_Skeleton = Sprite::create("enemy.png", Rect(0, 0 * 32, 32, 32));
	Sprite* Sprite_SwordSkeleton = Sprite::create("enemy.png", Rect(0, 1 * 32, 32, 32));
	Sprite* Sprite_EliteSkeleton = Sprite::create("enemy.png", Rect(0, 2 * 32, 32, 32));
	Sprite* Sprite_Ghost = Sprite::create("enemy.png", Rect(0, 12 * 32, 32, 32));

	auto* Label_Slime = LabelTTF::create("Attack:12\nDefense:6\nHP:10", "Arial", 16);
	auto* Label_Bat = LabelTTF::create("Attack:16\nDefense:8\nHP:16", "Arial", 16);
	auto* Label_Skeleton = LabelTTF::create("Attack:20\nDefense:10\nHP:25", "Arial", 16);
	auto* Label_SwordSkeleton = LabelTTF::create("Attack:26\nDefense:14\nHP:45", "Arial", 16);
	auto* Label_EliteSkeleton = LabelTTF::create("Attack:36\nDefense:24\nHP:60", "Arial", 16);
	auto* Label_Ghost = LabelTTF::create("Attack:48\nDefense:28\nHP:55", "Arial", 16);

	Sprite_Slime->setPosition(16, VERTICAL_INTERVAL+16);
	Sprite_Bat->setPosition(BLOCK_SIZE+16, VERTICAL_INTERVAL +16);
	Sprite_Skeleton->setPosition(2* BLOCK_SIZE +16, VERTICAL_INTERVAL +16);
	Sprite_SwordSkeleton->setPosition(16, BLOCK_SIZE + VERTICAL_INTERVAL +16);
	Sprite_EliteSkeleton->setPosition(BLOCK_SIZE +16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Sprite_Ghost->setPosition(2* BLOCK_SIZE +16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Label_Slime->setPosition(BLOCK_SIZE -LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE /2);
	Label_Bat->setPosition(2* BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_Skeleton->setPosition(3* BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_SwordSkeleton->setPosition(BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);
	Label_EliteSkeleton->setPosition(2* BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);
	Label_Ghost->setPosition(3* BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);

	Page1->addChild(Sprite_Slime);
	Page1->addChild(Sprite_Bat);
	Page1->addChild(Sprite_Skeleton);
	Page1->addChild(Sprite_SwordSkeleton);
	Page1->addChild(Sprite_EliteSkeleton);
	Page1->addChild(Sprite_Ghost);
	Page1->addChild(Label_Slime);
	Page1->addChild(Label_Bat);
	Page1->addChild(Label_Skeleton);
	Page1->addChild(Label_SwordSkeleton);
	Page1->addChild(Label_EliteSkeleton);
	Page1->addChild(Label_Ghost);

	this->addChild(Page1, helpKz[0]);


	//µÚ¶şÒ³Í¼¼ø
	
	
	Sprite* Sprite_ArmorSkeleton = Sprite::create("enemy.png", Rect(0, 3 * 32, 32, 32));
	Sprite* Sprite_BloodGhost = Sprite::create("enemy.png", Rect(0, 13 * 32, 32, 32));
	Sprite* Sprite_Orc = Sprite::create("enemy.png", Rect(0, 16 * 32, 32, 32));
	Sprite* Sprite_LittleWitch = Sprite::create("enemy.png", Rect(0, 14 * 32, 32, 32));
	Sprite* Sprite_GreatWitch = Sprite::create("enemy.png", Rect(0, 15 * 32, 32, 32));
	Sprite* Sprite_OrcWarrior = Sprite::create("enemy.png", Rect(0, 17 * 32, 32, 32));

	auto* Label_ArmorSkeleton = LabelTTF::create("Attack:44\nDefense:38\nHP:85", "Arial", 16);
	auto* Label_BloodGhost = LabelTTF::create("Attack:58\nDefense:30\nHP:70", "Arial", 16);
	auto* Label_Orc = LabelTTF::create("Attack:54\nDefense:50\nHP:90", "Arial", 16);
	auto* Label_LittleWitch = LabelTTF::create("Attack:70\nDefense:40\nHP:70", "Arial", 16);
	auto* Label_GreatWitch = LabelTTF::create("Attack:78\nDefense:46\nHP:85", "Arial", 16);
	auto* Label_OrcWarrior = LabelTTF::create("Attack:62\nDefense:58\nHP:110", "Arial", 16);
	
	Sprite_ArmorSkeleton->setPosition(16, VERTICAL_INTERVAL + 16);
	Sprite_BloodGhost->setPosition(BLOCK_SIZE + 16, VERTICAL_INTERVAL + 16);
	Sprite_Orc->setPosition(2 * BLOCK_SIZE + 16, VERTICAL_INTERVAL + 16);
	Sprite_LittleWitch->setPosition(16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Sprite_GreatWitch->setPosition(BLOCK_SIZE + 16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Sprite_OrcWarrior->setPosition(2 * BLOCK_SIZE + 16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Label_ArmorSkeleton->setPosition(BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_BloodGhost->setPosition(2 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_Orc->setPosition(3 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_LittleWitch->setPosition(BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);
	Label_GreatWitch->setPosition(2 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);
	Label_OrcWarrior->setPosition(3 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);

	Page2->addChild(Sprite_ArmorSkeleton);
	Page2->addChild(Sprite_BloodGhost);
	Page2->addChild(Sprite_Orc);
	Page2->addChild(Sprite_LittleWitch);
	Page2->addChild(Sprite_GreatWitch);
	Page2->addChild(Sprite_OrcWarrior);
	Page2->addChild(Label_ArmorSkeleton);
	Page2->addChild(Label_BloodGhost);
	Page2->addChild(Label_Orc);
	Page2->addChild(Label_LittleWitch);
	Page2->addChild(Label_GreatWitch);
	Page2->addChild(Label_OrcWarrior);

	this->addChild(Page2, helpKz[1]);


	//µÚÈıÒ³Í¼¼ø
	

	Sprite* Sprite_PortraitStone = Sprite::create("enemy.png", Rect(0, 18 * 32, 32, 32));
	Sprite* Sprite_MucusMan = Sprite::create("enemy.png", Rect(0, 19 * 32, 32, 32));
	Sprite* Sprite_BlueKnight = Sprite::create("enemy.png", Rect(0, 47 * 32, 32, 32));
	Sprite* Sprite_PaleGhost = Sprite::create("enemy.png", Rect(0, 48 * 32, 32, 32));
	Sprite* Sprite_BlackPaladin = Sprite::create("enemy.png", Rect(0, 24 * 32, 32, 32));
	Sprite* Sprite_Rogue = Sprite::create("enemy.png", Rect(0, 23 * 32, 32, 32));

	auto* Label_PortraitStone = LabelTTF::create("Attack:150\nDefense:55\nHP:40", "Arial", 16);
	auto* Label_MucusMan = LabelTTF::create("Attack:65\nDefense:65\nHP:160", "Arial", 16);
	auto* Label_BlueKnight = LabelTTF::create("Attack:75\nDefense:70\nHP:150", "Arial", 16);
	auto* Label_PaleGhost = LabelTTF::create("Attack:115\nDefense:50\nHP:110", "Arial", 16);
	auto* Label_BlackPaladin = LabelTTF::create("Attack:80\nDefense:80\nHP:200", "Arial", 16);
	auto* Label_Rogue = LabelTTF::create("Attack:120\nDefense:60\nHP:150", "Arial", 16);

	Sprite_PortraitStone->setPosition(16, VERTICAL_INTERVAL + 16);
	Sprite_MucusMan->setPosition(BLOCK_SIZE + 16, VERTICAL_INTERVAL + 16);
	Sprite_BlueKnight->setPosition(2 * BLOCK_SIZE + 16, VERTICAL_INTERVAL + 16);
	Sprite_PaleGhost->setPosition(16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Sprite_BlackPaladin->setPosition(BLOCK_SIZE + 16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Sprite_Rogue->setPosition(2 * BLOCK_SIZE + 16, BLOCK_SIZE + VERTICAL_INTERVAL + 16);
	Label_PortraitStone->setPosition(BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_MucusMan->setPosition(2 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_BlueKnight->setPosition(3 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + BLOCK_SIZE / 2);
	Label_PaleGhost->setPosition(BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);
	Label_BlackPaladin->setPosition(2 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);
	Label_Rogue->setPosition(3 * BLOCK_SIZE - LABEL_SIZE, VERTICAL_INTERVAL + 1.5*BLOCK_SIZE);

	Page3->addChild(Sprite_PortraitStone);
	Page3->addChild(Sprite_MucusMan);
	Page3->addChild(Sprite_BlueKnight);
	Page3->addChild(Sprite_PaleGhost);
	Page3->addChild(Sprite_BlackPaladin);
	Page3->addChild(Sprite_Rogue);
	Page3->addChild(Label_PortraitStone);
	Page3->addChild(Label_MucusMan);
	Page3->addChild(Label_BlueKnight);
	Page3->addChild(Label_PaleGhost);
	Page3->addChild(Label_BlackPaladin);
	Page3->addChild(Label_Rogue);

	this->addChild(Page3, helpKz[2]);



}


void HelpLayer::LastPageCallBack(Ref* psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
	if (page == 1) {
		Page1->setZOrder(0);
		Page3->setZOrder(10);
		page = 3;
	}
	else if (page == 2) {
		Page2->setZOrder(0);
		Page1->setZOrder(10);
		page = 1;
	}
	else {
		Page3->setZOrder(0);
		Page2->setZOrder(10);
		page = 2;
	}


}

void HelpLayer::NextPageCallBack(Ref* psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
	if (page == 1) {
		Page1->setZOrder(0);
		Page2->setZOrder(10);
		page = 2;
	}
	else if (page == 2) {
		Page2->setZOrder(0);
		Page3->setZOrder(10);
		page = 3;
	}
	else {
		Page3->setZOrder(0);
		Page1->setZOrder(10);
		page = 1;
	}

}

