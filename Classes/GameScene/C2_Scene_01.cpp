#include "BR_Scene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
#define SPEEDRIGHT 26.0f
#define SPEEDLEFT 0.6f
using namespace cocostudio::timeline;

Scene* BR_Scene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BR_Scene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

BR_Scene::BR_Scene() {
	for (int i = 0; i < 5; i++) {
		_toSpot[i] = false;
		_reachSpot[i] = false;
	}
	_reachSpot[4] = true;
	_reachMainpath = false;
	_isWalking = false;
}
// on "init" you need to initialize your instance
bool BR_Scene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("BR_Scene.csb");
    addChild(rootNode);

	//background setting
	_bg_back = Sprite::create("GameScene/BR_Scene/BR_S1.png");
	_bg_back->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_bg_back);

	_bg_front = Sprite::create("GameScene/BR_Scene/BR_S2.png");
	_bg_front->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_bg_front,20);

	//character setting
	_boyRoot = CSLoader::createNode("GameScene/BR_Scene/boyWalkAni.csb");
	_boyRoot->setPosition(650, 120);
	this->addChild(_boyRoot);

	_boyBody = (cocos2d::Sprite*)_boyRoot->getChildByName("body");
	_boyBody->setVisible(true);

	_walkAni[0] = (cocos2d::Sprite*)_boyRoot->getChildByName("leftBottom");
	_walkAni[0]->setVisible(false);

	_walkAni[1] = (cocos2d::Sprite*)_boyRoot->getChildByName("leftTop");
	_walkAni[1]->setVisible(false);

	_walkAni[2] = (cocos2d::Sprite*)_boyRoot->getChildByName("rightBottom");
	_walkAni[2]->setVisible(false);

	_walkAni[3] = (cocos2d::Sprite*)_boyRoot->getChildByName("rightTop");
	_walkAni[3]->setVisible(false);
	
	//spots
	char tmp[30];
	for (int i = 1; i < 6; i++) {
		sprintf(tmp, "spot_%d", i);
		_spot[i-1] = (cocos2d::Sprite*)rootNode->getChildByName(tmp);
		Size size = _spot[i - 1]->getContentSize();
		Point pos = _spot[i - 1]->getPosition();
		_spotRect[i-1] = Rect(pos.x-size.width/2, pos.y-size.height/2, size.width*1.5f, size.height*1.5f);
//		_spot[i - 1]->setVisible(false);
		this->addChild(_spot[i - 1]);
	}

	//talkAreas
	for (int i = 1; i < 7; i++) {
		sprintf(tmp, "talkArea_%d", i);
		_talkArea[i - 1] = (cocos2d::Sprite*)rootNode->getChildByName(tmp);
		Size size = _talkArea[i - 1]->getContentSize();
		Point pos = _talkArea[i - 1]->getPosition();
		_talkAreaRect[i - 1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
		//_talkArea[i - 1]->setVisible(false);
		//this->addChild(_talkArea[i-1]);
	}

	//bag
	_bagArea = (cocos2d::Sprite*)rootNode->getChildByName("bagTrigger");
	Size size = _bagArea->getContentSize();
	Point pos = _bagArea->getPosition();
	_bagRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	//touch
	_listener1 = EventListenerTouchOneByOne::create();	
	_listener1->onTouchBegan = CC_CALLBACK_2(BR_Scene::onTouchBegan, this);
	_listener1->onTouchMoved = CC_CALLBACK_2(BR_Scene::onTouchMoved, this);
	_listener1->onTouchEnded = CC_CALLBACK_2(BR_Scene::onTouchEnded, this);

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);
	this->schedule(CC_SCHEDULE_SELECTOR(BR_Scene::doStep));
    return true;
}

void BR_Scene::doStep(float dt) {
	Point pos = _boyRoot->getPosition();
	if (_toSpot[0]) {
		if (!_reachSpot[0]) {
			if (!_reachMainpath) {
				ToMainPath(dt);
			}
			else {
				if (!_reachSpot[1]) {
					ToSpot1(dt);
				}
				else {
					if (ToSpot0(dt)) {
						_isWalking = false;
						_toSpot[0] = false;
					}
				}
			}
		}
		else {
			_isWalking = false;
			_toSpot[0] = false;
		}
	}
	if (_toSpot[1]) {
		if (!_reachSpot[1]) {
			if (!_reachMainpath) {
				ToMainPath(dt);
			}
			else {
				if (ToSpot1(dt)) {
					_toSpot[1] = false;
					_isWalking = false;
				}
			}
		}
		else {
			_isWalking = false;
			_reachSpot[1] = false;
		}
	}
	if (_toSpot[2]) {
		if (!_reachSpot[2]) {
			if (!_reachMainpath) {
				ToMainPath(dt);
			}
			else {
				if (!_reachSpot[3]) {
					ToSpot3(dt);
				}
				else {
					if (ToSpot2(dt)) {
						_isWalking = false;
						_toSpot[2] = false;
					}
				}
			}
		}
		else {
			_isWalking = false;
			_toSpot[2] = false;
		}
	}
	if (_toSpot[3]) {
		if (!_reachSpot[3]) {
			if (!_reachMainpath) {
				ToMainPath(dt);
			}
			else {
				if (ToSpot3(dt)) {
					_isWalking = false;
					_toSpot[3] = false;
				}
			}
		}
		else {
			_isWalking = false;
			_reachSpot[3] = false;
		}
	}
	if (_toSpot[4]) {
		if (!_reachSpot[4]) {
			if (!_reachMainpath) {
				ToMainPath(dt);
			}
			else {
				if (!_reachSpot[3]) {
					ToSpot3(dt);
				}
				else {
					if (ToSpot4(dt)) {
						_isWalking = false;
						_toSpot[4] = false;
					}
				}
			}
		}
		else {
			_isWalking = false;
			_toSpot[4] = false;
		}
	}
}

bool BR_Scene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	//testing
	if (!_isWalking) {
		if (_talkAreaRect[0].containsPoint(touchLoc) || _talkAreaRect[1].containsPoint(touchLoc)) {
			_toSpot[2] = true;
		}
		if (_talkAreaRect[2].containsPoint(touchLoc) || _talkAreaRect[3].containsPoint(touchLoc)) {
			_toSpot[0] = true;
		}
		if (_talkAreaRect[4].containsPoint(touchLoc)) {
			_toSpot[1] = true;
		}
		if (_talkAreaRect[5].containsPoint(touchLoc)) {
			_toSpot[4] = true;
		}
	}
	//log("%1.0f,1.0%f", _boyRoot->getPositionX(), _boyRoot->getPositionY());
	//log("%1.0f,1.0%f", touchLoc.x, touchLoc.y);
	//toSpot[2] = true;
	return false;
}

void BR_Scene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}

void BR_Scene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}

bool BR_Scene::ToMainPath(float dt) {
	Point pos = _boyRoot->getPosition();
	float m = -185.0f / 506.0f;
	float value = pos.x*m - 480 * m + 445;
	if (!_reachSpot[1] && !_reachSpot[3]) {
		if (pos.y > value) { // right
			if (_spotRect[1].containsPoint(pos)) {
				ClearReachSpot();
				_reachSpot[1] = true;
				_reachMainpath = true;
				return true;
			} 
			else if (_spotRect[3].containsPoint(pos)) {
				ClearReachSpot();
				_reachSpot[3] = true;
				_reachMainpath = true;
				return true;
			}
			else {
				_isWalking = true;
				_boyRoot->setPosition(Vec2(pos.x - 12.0f*SPEEDRIGHT*dt, pos.y -5.0f*SPEEDRIGHT*dt));
				return false;
			}
		}
		else //left
		{
			if (_spotRect[3].containsPoint(pos)) {
				ClearReachSpot();
				_reachSpot[3] = true;
				_reachMainpath = true;
				return true;
			}
			else {
				_isWalking = true;
				_boyRoot->setPosition(Vec2(pos.x + 12.0f*SPEEDRIGHT*dt, pos.y + 5.0f*SPEEDRIGHT*dt));
				return false;
			}
		}
	}
	else {
		_reachMainpath = true;
		return true;
	}
	return true;
}

bool BR_Scene::ToSpot0(float dt) {
	Point pos = _boyRoot->getPosition();
	if (_spotRect[0].containsPoint(pos)) {
		ClearReachSpot();
		_reachSpot[0] = true;
		_reachMainpath = false;
		return true;
	}
	else { 
		_isWalking = true;
		_boyRoot->setPosition(Vec2(pos.x + 12.0f*SPEEDRIGHT*dt, pos.y + 5.0f*SPEEDRIGHT*dt));
		return false;
	}
}

bool BR_Scene::ToSpot1(float dt) {
	Point pos = _boyRoot->getPosition();
	if (_spotRect[1].containsPoint(pos)) {
		ClearReachSpot();
		_reachSpot[1] = true;
		_reachMainpath = true;
		return true;
	}
	else {
		_isWalking = true;
		_boyRoot->setPosition(Vec2(pos.x - 506.0f*SPEEDLEFT*dt, pos.y + 185.0f*SPEEDLEFT*dt));
		return false;
	}
}

bool BR_Scene::ToSpot2(float dt) {
	Point pos = _boyRoot->getPosition();
	if (_spotRect[2].containsPoint(pos)) {
		ClearReachSpot();
		_reachSpot[2] = true;
		_reachMainpath = false;
		return true;
	}
	else {
		_isWalking = true;
		_boyRoot->setPosition(Vec2(pos.x + 12.0f*SPEEDRIGHT*dt, pos.y + 5.0f*SPEEDRIGHT*dt));
		return false;
	}
}

bool BR_Scene::ToSpot4(float dt) {
	Point pos = _boyRoot->getPosition();
	if (_spotRect[4].containsPoint(pos)) {
		ClearReachSpot();
		_reachSpot[4] = true;
		_reachMainpath = false;
		return true;
	}
	else {
		_isWalking = true;
		_boyRoot->setPosition(Vec2(pos.x - 12.0f*SPEEDRIGHT*dt, pos.y - 5.0f*SPEEDRIGHT*dt));
		return false;
	}
}

bool BR_Scene::ToSpot3(float dt) {	 // from 1 to 3
	Point pos = _boyRoot->getPosition();
	if (_spotRect[3].containsPoint(pos)) {
		ClearReachSpot();
		_reachSpot[3] = true;
		_reachMainpath = true;
		return true;
	}
	else {
		_isWalking = true;
		_boyRoot->setPosition(Vec2(pos.x + 506.0f*SPEEDLEFT*dt, pos.y - 185.0f*SPEEDLEFT*dt));
		return false;
	}
}

void BR_Scene::ClearReachSpot() {
	for (int i = 0; i < 5; i++) {
		_reachSpot[i] = false;
	}
}

