#ifndef _HROCKER_
#define _HROCKER_


#include"cocos2d.h"
USING_NS_CC;

typedef enum {
	tag_rocker,tag_rockerBG
}tagForHRocker;

class HRocker : public Layer
{
public:
	EventListenerTouchOneByOne * listener;
	//创建摇杆
	static HRocker* createHRocker(const char* rockerImageName, const char* rockerBGImageName, Point position);
	//启动摇杆
	void startRocker(bool _isStopOther);
	//停止摇杆
	void stopRocker();
private:
	void rockerInit(const char* rockerImageName, const char* rockerBGImageName, Point position);
	//得到半径为r的圆周上一个角度对应的x，y坐标
	Point getAnglePosition(float r, float angle);
	//是否可操作摇杆
	bool isCanMove;
	//得到摇杆与用户触屏点的角度
	float getRad(Point pos1, Point pos2);
	//摇杆背景坐标
	Point rockerBGPosition;
	//摇杆背景的半径
	float rockerBGR;
	//触屏事件
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	CREATE_FUNC(HRocker);

};


#endif