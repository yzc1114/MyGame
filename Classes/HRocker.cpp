#include"cocos2d.h"
#include"HRocker.h"

HRocker * HRocker::createHRocker(const char * rockerImageName, const char * rockerBGImageName, Point position)
{
	HRocker* layer = HRocker::create();
	if (layer) {
		layer->rockerInit(rockerImageName, rockerBGImageName, position);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

void HRocker::startRocker(bool _isStopOther)
{
	//精灵设置可见，设置监听
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	rocker->setVisible(true);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rockerBG->setVisible(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HRocker::stopRocker()
{
	//精灵设置不可见，取消监听
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	rocker->setVisible(false);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rockerBG->setVisible(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
}

void HRocker::rockerInit(const char * rockerImageName, const char * rockerBGImageName, Point position)
{
	//添加背景图
	Sprite* spRockerBG = Sprite::create(rockerBGImageName);
	spRockerBG->setVisible(true);
	spRockerBG->setPosition(position);
	spRockerBG->setScale(0.2f);
	addChild(spRockerBG, 0, tag_rockerBG);
	//添加按钮图
	Sprite* spRocker = Sprite::create(rockerImageName);
	spRocker->setVisible(true);
	spRocker->setPosition(position);
	spRocker->setOpacity(180);
	spRocker->setScale(0.2f);
	addChild(spRocker, 1, tag_rocker);
	//摇杆背景图坐标
	rockerBGPosition = position;
	//摇杆背景图半径
	rockerBGR = spRockerBG->getContentSize().width*0.5*0.2;
	
	listener = EventListenerTouchOneByOne::create();
	//绑定监听事件
	listener->onTouchBegan = CC_CALLBACK_2(HRocker::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HRocker::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HRocker::onTouchEnded, this);
	listener->setSwallowTouches(true);

}

Point HRocker::getAnglePosition(float r, float angle)
{

	return Vec2(r*cos(angle),r*sin(angle));
}

float HRocker::getRad(Point pos1, Point pos2)
{
	//得到两点的坐标x，y坐标值
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;
	//求出两边长度
	float x = px2 - px1;
	float y = py2 - py1;
	//利用勾股定理求两点间直线距离
	float xie = sqrt(x*x + y * y);
	float cos = x / xie;
	float rad = acos(cos);
	//当触屏y坐标<摇杆的y坐标时，取反
	if (py1 > py2) {
		rad = -rad;
	}
	return rad;
}

bool HRocker::onTouchBegan(Touch * touch, Event * event)
{
	Sprite* sp = (Sprite*)getChildByTag(tag_rocker);
	//得到触屏点坐标
	Point point = touch->getLocation();
	//判断是否单击到sp这个精灵：boundingbox（）精灵大小之内的所有坐标
	if (sp->boundingBox().containsPoint(point)) {
		isCanMove = true;
	}
	return true;
	return false;
}

void HRocker::onTouchMoved(Touch * touch, Event * event)
{
	if (!isCanMove) {
		return;
	}
	Sprite* sp = (Sprite*)getChildByTag(tag_rocker);
	Point point = touch->getLocation();
	//判断两个圆心的距离是否大于摇杆背景的半径
	if (sqrt(pow(point.x - rockerBGPosition.x, 2) + pow(point.y - rockerBGPosition.y, 2)) >= rockerBGR) {
		//得到触点与摇杆背景圆心形成的角度
		float angle = getRad(rockerBGPosition, point);
		//确保小圆运动在背景图内
		sp->setPosition(getAnglePosition(rockerBGR, angle) + rockerBGPosition);
	}
	else {
		sp->setPosition(point);
	}
}

void HRocker::onTouchEnded(Touch * touch, Event * event)
{
	if (!isCanMove) {
		return;
	}
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08f, rockerBG->getPosition()));
	isCanMove = false;

}



