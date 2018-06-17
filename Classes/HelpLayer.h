#ifndef _HELP_LAYER_
#define _HELP_LAYER_
#include "MagicTower.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui\CocosGUI.h"
USING_NS_CC;

USING_NS_CC_EXT;
using namespace ui;

#define BLOCK_SIZE (Director::getInstance()->getVisibleSize().width*180.0/608.0)
#define VERTICAL_INTERVAL (Director::getInstance()->getVisibleSize().height/2-BLOCK_SIZE)
#define LABEL_SIZE ((BLOCK_SIZE-32)/2)




class HelpLayer : public Layer
{
public:
	HelpLayer();
	~HelpLayer();
	virtual bool init();
	CREATE_FUNC(HelpLayer);
	
	void addIllustration();
	void LastPageCallBack(Ref* psender);
	void NextPageCallBack(Ref* psender);

	

private:
	int helpKz[3] = { 10,0,0 };
	int page = 1;

	Layer* Page1 = Layer::create();
	Layer* Page2 = Layer::create();
	Layer* Page3 = Layer::create();
	


};

#endif // !_HELP_LAYER_
