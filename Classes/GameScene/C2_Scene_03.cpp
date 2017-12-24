#include "C2_Scene_03.h"
#include "C2_Scene_02.h"
#include "C2_Scene_01.h"
#include "GameScene\GRScene.h"

USING_NS_CC;
#define SPEED 2
using namespace cocostudio::timeline;

int C2_Scene_03::_from = 0;

Scene* C2_Scene_03::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = C2_Scene_03::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

C2_Scene_03::C2_Scene_03() {
	for (int i = 0; i < 6; i++) {
		_toSpot[i] = false;
	}
	_isWalking = false;
}
// on "init" you need to initialize your instance
bool C2_Scene_03::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("C2Scene03.csb");
    addChild(rootNode);

	//background setting
	_bg = Sprite::create("GameScene/C2Scene/C2_Z1.png");
	_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_bg);

	_bgfront = (cocos2d::Sprite*)rootNode->getChildByName("bg_front");
	_bgfront->setPosition(1460, 827);
	this->addChild(_bgfront, 2);
	if (_from == 1){ // from C2_1
		_boy = new CPlayer(false, *this, Point(925, 490), false);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(5, true);
	}
	else if (_from == 2) // from C2_2
	{
		_boy = new CPlayer(true, *this, Point(1900, 35), false);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(3, true);
	}
	else if(_from == 0) // from C1
	{
		_boy = new CPlayer(false, *this, Point(50, 330), true);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(0, true);
	}
	else if (_from == 5) // from boyRoom
	{
		_boy = new CPlayer(false, *this, Point(690, 409), true);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(4, true);
	}
	else // from GR
	{
		_boy = new CPlayer(false, *this, Point(1170, 143.5), false);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(2, true);
	}


	//spots
	char tmp[30];
	for (int i = 0; i < 5; i++) {
		sprintf(tmp, "spot_%d", i);
		_spot[i] = (cocos2d::Sprite*)rootNode->getChildByName(tmp);
		Size size = _spot[i]->getContentSize();
		Point pos = _spot[i]->getPosition();
		_spotRect[i] = Rect(pos.x - size.width*1.5f / 2, pos.y - size.height*1.5f / 2, size.width*1.5f, size.height*1.5f);
	}

	_spot[5] = (cocos2d::Sprite*)rootNode->getChildByName("spot_5");
	Size size = _spot[5]->getContentSize();
	Point pos = _spot[5]->getPosition();
	_spotRect[5] = Rect(pos.x-size.width*6.7f/2, pos.y-size.height*3.0f/2, size.width*6.7f, size.height*3.0f);

	//door
	_doorArea[0]= (cocos2d::Sprite*)rootNode->getChildByName("doorArea_1");
	size = _doorArea[0]->getContentSize();
	pos = _doorArea[0]->getPosition();
	_doorRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_doorArea[1] = (cocos2d::Sprite*)rootNode->getChildByName("doorArea_2");
	size = _doorArea[1]->getContentSize();
	pos = _doorArea[1]->getPosition();
	_doorRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//touch
	_listener1 = EventListenerTouchOneByOne::create();	
	_listener1->onTouchBegan = CC_CALLBACK_2(C2_Scene_03::onTouchBegan, this);
	_listener1->onTouchMoved = CC_CALLBACK_2(C2_Scene_03::onTouchMoved, this);
	_listener1->onTouchEnded = CC_CALLBACK_2(C2_Scene_03::onTouchEnded, this);

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);
	this->schedule(CC_SCHEDULE_SELECTOR(C2_Scene_03::doStep));
    return true;
}

void C2_Scene_03::doStep(float dt) {
	
	// Z-Order
	if (_boy->getPosition().y > 290) {	_boy->setZOrder(1);	}
	else	{	_boy->setZOrder(3);	}

	// Walk
	if (_toSpot[0]) {
		if (!_boy->GetReachSpot(0)) {
			if (ToSpot0(dt)) {
				_isWalking = false;
				_toSpot[0] = false;
			}
		}	
	}
	
	if (_toSpot[2]) {
	//	if (!_boy->GetReachSpot(2)) {
			if (ToSpot2(dt)) {
				_toSpot[2] = false;
				_isWalking = false;
				this->unschedule(schedule_selector(C2_Scene_03::doStep));
				Director::getInstance()->replaceScene(GRScene::createScene());
			}
	//	}
	}
	if (_toSpot[3]) {
		if (ToSpot3(dt)) {
			_toSpot[3] = false;
			_isWalking = false;
			this->unschedule(schedule_selector(C2_Scene_03::doStep));
			C2_Scene_02::_from = 1;
			Director::getInstance()->replaceScene(C2_Scene_02::createScene());	
		}
	
	}
	if (_toSpot[4]) {
		if (ToSpot4(dt)) {
			_toSpot[4] = false;
			_isWalking = false;
		}
	}
	if (_toSpot[5]) {
		if (ToSpot5(dt)) {
			_toSpot[5] = false;
			_isWalking = false;
			this->unschedule(schedule_selector(C2_Scene_03::doStep));
			C2_Scene_01::_from = 3;
			Director::getInstance()->replaceScene(C2_Scene_01::createScene());
		}
	}
}

bool C2_Scene_03::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	//testing
	if (!_isWalking) {
		ClearToSpot();
		if (_spotRect[0].containsPoint(touchLoc)) {
			_toSpot[0] = true;
		}
		else if (_spotRect[3].containsPoint(touchLoc)) {
			_toSpot[3] = true;
		}
		else if (_spotRect[5].containsPoint(touchLoc)) {
			_toSpot[5] = true;
		}
		else if (_doorRect[0].containsPoint(touchLoc)) {
			_toSpot[2] = true;
		}
		else if (_doorRect[1].containsPoint(touchLoc)) {
			_toSpot[4] = true;
		}
	}

	return false;
}

void C2_Scene_03::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}

void C2_Scene_03::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}


bool C2_Scene_03::ToSpot0(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[0].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(0, true);
		_boy->Stop(true);
		_boy->Mirror(false);
		return true;
	}
	else {
		if (_boy->GetReachSpot(4)|| _boy->GetReachSpot(5)) { // ToSpot1
			ToSpot1_45(dt);
		}
		else {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x - 370.0f*SPEED*dt, pos.y + 59.0f*SPEED*dt));
			return false;
		}
		return false;
	}
}

bool C2_Scene_03::ToSpot2(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[2].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(2, true);
		_boy->Mirror(true);
		_boy->Stop(true);
		return true;
	}
	else {
		if (_boy->GetReachSpot(4) || _boy->GetReachSpot(5)) { // ToSpot1
			ToSpot1_45(dt);
		}
		else if (_boy->GetReachSpot(3)) {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x - 370.0f*SPEED*dt, pos.y + 59.0f*SPEED*dt));
			return false;
		}
		else {
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x + 370.0f*SPEED*dt, pos.y - 59.0f*SPEED*dt));
			return false;
		}
		return false;
	}
}


bool C2_Scene_03::ToSpot1_45(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[1].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(1, true);
		_boy->Stop();
		return true;
	}
	else // from 4 5
	{
		_isWalking = true;
		_boy->Mirror(false);
		_boy->go(false);
		_boy->setPosition(Vec2(pos.x - 360 * SPEED*dt, pos.y - 150 * SPEED*dt));
		return false;
	}
	
}

bool C2_Scene_03::ToSpot1_023(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[1].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(1, true);
		_boy->Stop();
		return true;
	}
	else // from 0 2 3
	{
		if(_boy->GetReachSpot(0)){
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x + 370.0f*SPEED*dt, pos.y - 59.0f*SPEED*dt));
			return false;
		}
		else {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x - 370.0f*SPEED*dt, pos.y + 59.0f*SPEED*dt));
			return false;
		}
	}
}

bool C2_Scene_03::ToSpot3(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[3].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(3, true);
		_boy->Mirror(true);
		_boy->Stop(false);
		return true;
	}
	else {
		if (_boy->GetReachSpot(4) || _boy->GetReachSpot(5)) { // ToSpot1
			ToSpot1_45(dt);
		}
		else {
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x + 370.0f*SPEED*dt, pos.y - 59.0f*SPEED*dt));
			return false;
		}
		return false;
	}
}

bool C2_Scene_03::ToSpot4(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[4].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(4, true);
		_boy->Mirror(false);
		_boy->Stop(true);
		return true;
	}
	else {
		if (_boy->GetReachSpot(5)) {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x - 360.0f*SPEED*dt, pos.y - 150.0f*SPEED*dt));
		}
		else if (_boy->GetReachSpot(0) || _boy->GetReachSpot(2) || _boy->GetReachSpot(3)) { // ToSpot1
			ToSpot1_023(dt);
		}
		else {
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x + 360.0f*SPEED*dt, pos.y + 150.0f*SPEED*dt));
		}
		return false;
	}
}

void C2_Scene_03::ClearToSpot() {
	for (int i = 0; i < 6; i++) {
		_toSpot[i] = false;
	}
}

bool C2_Scene_03::ToSpot5(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[5].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(5, true);
		_boy->Mirror(true);
		_boy->Stop(true);
		return true;
	}
	else {
		if (_boy->GetReachSpot(0) || _boy->GetReachSpot(2) || _boy->GetReachSpot(3)) { // ToSpot1
		ToSpot1_023(dt);
		}
		else {
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x + 360.0f*SPEED*dt, pos.y + 150.0f*SPEED*dt));
		}
		return false;
	}
}