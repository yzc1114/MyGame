#include "ChatRoom.h"
ChatRoom::ChatRoom()
{
}
ChatRoom::~ChatRoom()
{
}

bool ChatRoom::init()
{
	if (!Scene::init()) {
		return false;
	}
	
	LayerColor* BGLayer = LayerColor::create(Color4B::BLACK);
	this->addChild(BGLayer);

	
}
