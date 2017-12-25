#include "CFlyEffect.h"

USING_NS_CC;

CFlyEffect::CFlyEffect() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/bagItem.plist");
	_pic = Sprite::createWithSpriteFrameName("B_akey.png");
	//_pic->setScale(0.1f);
	_pic->setPosition(Point(0.0f, 0.0f));
	//_pic = nullptr;
	this->addChild(_pic);

	_endPos = Point((visibleSize.width- origin.x)/2,0);
}

CFlyEffect::~CFlyEffect() {

	
}


void CFlyEffect::setFly(Point pos) {
	
	
	// set parameter
	_pic->setPosition(pos);
	dx = _endPos.x - pos.x;
	dy = _endPos.y - pos.y;
	
	vx = dx/1.0f;
	float t = dx / vx;
	vy = (dy + 0.5*GRAVITY*t*t) / t;
	//vy = 10.0f;
}

bool CFlyEffect::doStep(float dt) {
	Point curPos = _pic->getPosition();
	vy += GRAVITY;
	_pic->setPosition(curPos.x + vx*dt, curPos.y + vy*dt);
	if (curPos.y < 0) return false;
	else return true;
}

void CFlyEffect::setPic(const std::string&picImage) {
	_pic->setSpriteFrame(picImage);
}
