#include "C2_Scene_01.h"
#include "C2_Scene_03.h"
#include "GameScene\SRScene.h"

USING_NS_CC;
#define SPEED 3
using namespace cocostudio::timeline;

int C2_Scene_01::_from = 0;

Scene* C2_Scene_01::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = C2_Scene_01::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

C2_Scene_01::C2_Scene_01() {
	for (int i = 0; i < 2; i++) {
		_toSpot[i] = false;
	}
	_isWalking = false;
}
// on "init" you need to initialize your instance

bool C2_Scene_01::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("C2Scene.csb");
    addChild(rootNode);

	//background setting
	_bg = Sprite::create("GameScene/C2Scene/C2_Z3.png");
	_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_bg);

	//character setting

	if (_from == 0) { // from SR
		_boy = new CPlayer(false, *this, Point(1000, 315), true);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(1, true);
	}
	else // from corridor
	{
		_boy = new CPlayer(true, *this,Point(120,60),true);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(0, true);
	}
	



	//spots
	_spot[0] = (cocos2d::Sprite*)rootNode->getChildByName("spot_0");
	Size size = _spot[0]->getContentSize();
	Point pos = _spot[0]->getPosition();
	_spotRect[0] = Rect(pos.x-size.width*12/2, pos.y-size.height*3.5/2, size.width*12, size.height*3.5f);
//		_spot[i - 1]->setVisible(false);
//	this->addChild(_spot[0]);

	_spot[1] = (cocos2d::Sprite*)rootNode->getChildByName("spot_1");
	size = _spot[1]->getContentSize();
	pos = _spot[1]->getPosition();
	_spotRect[1] = Rect(pos.x - size.width*2.5 / 2, pos.y - size.height*2.5 / 2, size.width*2.5f, size.height*2.5f);
//	this->addChild(_spot[1]);
	
		
	//door
	_doorArea= (cocos2d::Sprite*)rootNode->getChildByName("doorRect");
	size = _doorArea->getContentSize();
	pos = _doorArea->getPosition();
	_doorRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	//touch
	_listener1 = EventListenerTouchOneByOne::create();	
	_listener1->onTouchBegan = CC_CALLBACK_2(C2_Scene_01::onTouchBegan, this);
	_listener1->onTouchMoved = CC_CALLBACK_2(C2_Scene_01::onTouchMoved, this);
	_listener1->onTouchEnded = CC_CALLBACK_2(C2_Scene_01::onTouchEnded, this);

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);
	this->schedule(CC_SCHEDULE_SELECTOR(C2_Scene_01::doStep));
    return true;
}

void C2_Scene_01::doStep(float dt) {
	if (_toSpot[0]) {
		if (!_boy->GetReachSpot(0)) {
			if (ToSpot0(dt)) {
				_isWalking = false;
				_toSpot[0] = false;
				this->unschedule(schedule_selector(C2_Scene_01::doStep));
				C2_Scene_03::_from = 1;
				Director::getInstance()->replaceScene(C2_Scene_03::createScene());
			}
		}
		else
		{
			this->unschedule(schedule_selector(C2_Scene_01::doStep));
			C2_Scene_03::_from = 1;
			Director::getInstance()->replaceScene(C2_Scene_03::createScene());
		}
	}
	
	if (_toSpot[1]) {
		if (!_boy->GetReachSpot(1)) {
			if (ToSpot1(dt)) {
				_toSpot[1] = false;
				_isWalking = false;
				this->unschedule(schedule_selector(C2_Scene_01::doStep));
				Director::getInstance()->replaceScene(SRScene::createScene());
			}
		}
		else
		{
			this->unschedule(schedule_selector(C2_Scene_01::doStep));
			Director::getInstance()->replaceScene(SRScene::createScene());

		}
	}
	
}

bool C2_Scene_01::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	//testing
//	if (!_boy->GetIsTalking()) {
		if (!_isWalking) {
			if (_spotRect[0].containsPoint(touchLoc)) {
				_toSpot[1] = false;
				_toSpot[0] = true;
				log("clicked0");
			}
			if (_doorRect.containsPoint(touchLoc)) {
				_toSpot[0] = false;
				_toSpot[1] = true;
				log("door");
			}
		}
//	}
//	else { 
//		_boy->SetIsTalking(false);
//		_boy->StopTalking();
//	}

	return true;
}

void C2_Scene_01::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}

void C2_Scene_01::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	
}


bool C2_Scene_01::ToSpot0(float dt) {
	Vec2 pos = _boy->getPosition();
	if (_spotRect[0].containsPoint(pos)) {
		_boy->SetReachSpot(1, false);
		_boy->SetReachSpot(0, true);
		_boy->Stop(false);
		return true;
	}
	else { 
		_isWalking = true;
		_boy->Mirror(false);
		_boy->go(false);
		_boy->setPosition(Vec2(pos.x - 176.0f*SPEED*dt, pos.y - 51.0f*SPEED*dt));
		return false;
	}
}

bool C2_Scene_01::ToSpot1(float dt) {
	Vec2 pos = _boy->getPosition();
	if (_spotRect[1].containsPoint(pos)) {
		_boy->SetReachSpot(0, false);
		_boy->SetReachSpot(1, true);
		_boy->Mirror(false);
		_boy->Stop(true);
		return true;
	}
	else {
		_isWalking = true;
		_boy->Mirror(true);
		_boy->go(true);
		_boy->setPosition(Vec2(pos.x + 176.0f*SPEED*dt, pos.y + 51.0f*SPEED*dt));
		return false;
	}
}


