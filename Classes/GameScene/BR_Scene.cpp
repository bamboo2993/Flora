#include "BR_Scene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "GameScene\C2_Scene_03.h"

using namespace cocostudio::timeline;
USING_NS_CC;

#define SPEED 3
#define BAG_OPEN_HEIGHT 150.0f
#define BAG_CLOSE_HEIGHT 250.0f

#define CURRENT_SCENE   "BR_Scene.cpp"

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
	for (int i = 0; i < 6; i++) {
		_toSpot[i] = false;
	}
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
	/*_boy = new CPlayer(true, *this, Point(1530.0f, 41.15f), false);
	_boy->setAnimation("Animation/boyanim.plist");
	_boy->SetReachSpot(6, true);
*/
	_boy = new CPlayer(false, *this, Point(1250, 370), 0);
	_boy->setAnimation("Animation/boyanim.plist");
	_boy->SetReachSpot(2, true);


	//spots
	char tmp[30];
	for (int i = 0; i < 5; i++) {
		sprintf(tmp, "spot_%d", i);
		_spot[i] = (cocos2d::Sprite*)rootNode->getChildByName(tmp);
		Size size = _spot[i]->getContentSize();
		Point pos = _spot[i]->getPosition();
		_spotRect[i] = Rect(pos.x-size.width*2.5/2, pos.y-size.height*2.5/2, size.width*2.5f, size.height*2.5f);
	}
	Size size = _spot[3]->getContentSize();
	Point pos = _spot[3]->getPosition();
	_spotRect[3] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width*2.5f, size.height*1.5f);


	_spot[5] = (cocos2d::Sprite*)rootNode->getChildByName("spot_5");
	size = _spot[5]->getContentSize();
	pos = _spot[5]->getPosition();
	_spotRect[5] = Rect(pos.x - size.width*22 / 2, pos.y - size.height*3.8 / 2, size.width*22, size.height*3.8f);


	//talkAreas
	for (int i = 0; i < 5; i++) {
		sprintf(tmp, "talkArea_%d", i);
		_talkArea[i] = (cocos2d::Sprite*)rootNode->getChildByName(tmp);
		Size size = _talkArea[i]->getContentSize();
		Point pos = _talkArea[i]->getPosition();
		_talkAreaRect[i] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
	}

	//bag
	_newspaper = CLightbox::create();
	_newspaper->init(rootNode, "bagTrigger", "GameScene/BR_Scene/BR_P.png");
	_newspaper->setVisible(false);
	this->addChild(_newspaper, 25);

	//set bag =================================================================


	CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);


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
	if (_boy->getPosition().x < 550)
		log("%f,%f", _boy->getPosition().x, _boy->getPosition().y);
	if (_toSpot[0]) {
	//	if (!_boy->GetReachSpot(0)) {
			if (ToSpot0(dt)) {
				_isWalking = false;
				_toSpot[0] = false;
				//talk		
			}
	//	}
		else
		{
		//talk
		}
	}
	if (_toSpot[1]) {
		if (!_boy->GetReachSpot(1)) {
			if (ToSpot1(dt)) {
				_isWalking = false;
				_toSpot[0] = false;
				//talk		
			}
		}
		else
		{
			//talk
		}
	}
	if (_toSpot[2]) {
		if (!_boy->GetReachSpot(2)) {
			if (ToSpot2(dt)) {
				_isWalking = false;
				_toSpot[2] = false;
				//talk
				_newspaper->doStep(dt);
			}
		}
		else
		{
			//talk
		}
	}
	if (_toSpot[4]) {
		if (!_boy->GetReachSpot(4)) {
			if (ToSpot4(dt)) {
				_isWalking = false;
				_toSpot[0] = false;
				//talk		
			}
		}
		else
		{
			//talk
		}
	}
	if (_toSpot[5]) {
		if (!_boy->GetReachSpot(5)) {
			if (ToSpot5(dt)) {
				_isWalking = false;
				_toSpot[5] = false;
				this->unschedule(schedule_selector(BR_Scene::doStep));
				C2_Scene_03::_from = 5;
				Director::getInstance()->replaceScene(C2_Scene_03::createScene());
			}
		}
		else
		{
			this->unschedule(schedule_selector(BR_Scene::doStep));
			C2_Scene_03::_from = 5;
			Director::getInstance()->replaceScene(C2_Scene_03::createScene());
		}
	}

}

bool BR_Scene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	_touchLoc = pTouch->getLocation();
	//testing
	if (!_newspaper->GetOpen()) {
		if (!_isWalking) {
			ClearToSpot();
			if (_talkAreaRect[0].containsPoint(_touchLoc)) {
				_toSpot[2] = true;
				log("2");
			}
			else if (_talkAreaRect[2].containsPoint(_touchLoc) || _talkAreaRect[1].containsPoint(_touchLoc)) {
				_toSpot[0] = true;
				log("0");
			}
			else if (_talkAreaRect[4].containsPoint(_touchLoc)) {
				_toSpot[4] = true;
				log("4");
			}
			else if (_talkAreaRect[3].containsPoint(_touchLoc)) {
				_toSpot[1] = true;
				log("1");
			}
			else if (_spotRect[5].containsPoint(_touchLoc)) {
				_toSpot[5] = true;
				log("5");
			}
			else if (_newspaper->TouchBegan(_touchLoc)) {
				_toSpot[2] = true;
			}
		}	
	}
	else{
		_newspaper->TouchBegan(_touchLoc);
	}



	//swipe gesture
	_startX = _touchLoc.x;
	_startY = _touchLoc.y;

	if (!_boy->GetIsTalking()) {
		if (_ibagState) { //when bag is open
			//use items in bag===========================================
			CBag::getInstance()->touchesBegan(_touchLoc);

		}
	}
	return true;
}

void BR_Scene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	_touchLoc = pTouch->getLocation();
	if (!_boy->GetIsTalking()) {
		//use items in bag===========================================
		if (_ibagState) { //when bag is open
			CBag::getInstance()->touchesMoved(_touchLoc);
		}
	}
	log("moved");
}

void BR_Scene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();

	_touchLoc = pTouch->getLocation();


	//swipe gesture
	float offsetX = _touchLoc.x - _startX;
	float offsetY = _touchLoc.y - _startY;

	if (!_boy->GetIsTalking()) {
		if (_ibagState != 2) {

			// [WALK + PICK OBJECT]===================
			if (offsetX == 0 && offsetY == 0 && !CBag::getInstance()->LightboxState()) { // when screen tapped
				if (_touchLoc.y > 227) {

					////player walk =====================================================

					//放大鏡沒開 --------------
					if (!_bopenNode) {
						//沒按重置-------------
						if (!_resetRect.containsPoint(_touchLoc)) {

							if (!_isWalking) {
								ClearToSpot();
								if (_talkAreaRect[0].containsPoint(_touchLoc)) {
									_toSpot[2] = true;
									log("2");
								}
								else if (_talkAreaRect[2].containsPoint(_touchLoc) || _talkAreaRect[1].containsPoint(_touchLoc)) {
									_toSpot[0] = true;
									log("0");
								}
								else if (_talkAreaRect[4].containsPoint(_touchLoc)) {
									_toSpot[4] = true;
									log("4");
								}
								else if (_talkAreaRect[3].containsPoint(_touchLoc)) {
									_toSpot[1] = true;
									log("1");
								}
								else if (_spotRect[5].containsPoint(_touchLoc)) {
									_toSpot[5] = true;
									log("5");
								}
								else if (_newspaper->TouchBegan(_touchLoc)) {
									_toSpot[2] = true;
								}
							}

						}
						else {
							// reset button=========================
							//reset();
						}



					}




					//znode[0]開---------------------
					if (_bopenNode) {
						_pTrigger[1].touchesBegan(_touchLoc);
						_pTrigger[2].touchesBegan(_touchLoc);
						_pTrigger[3].touchesBegan(_touchLoc);
						_pTrigger[4].touchesBegan(_touchLoc);
						_pTrigger[5].touchesBegan(_touchLoc);

						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[0] = !_bopenNode[0];
							_zNode[0]->setVisible(false);
							log("close detect");
						}

					}
					

					if (!_isWalking) {
						//0
						if (!_bopenNode[0] && _detectRect[0].containsPoint(_touchLoc)) {
							_btouchNode[0] = true;

							_bWalk = 1;
							_player->setPreviousPosition();

							if (_touchLoc.x > _player->_rpos.x) {
								_player->_isFacingRight = 1;
								_player->Mirror();
							}
							else {
								_player->_isFacingRight = 0;
								_player->Mirror();
							}

							//-------------------------------
							_TargetLoc = _touchLoc;

							log("touched detect");
						}
						else if (!_bopenNode[0] && !_detectRect[0].containsPoint(_touchLoc)) {
							_btouchNode[0] = false;
						}
						
					}




				}
				else {
					if (!_ibagState) {

						//放大鏡沒開 --------------
						if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {
							//沒按重置-------------

							//====================================

							if (_outRect.containsPoint(_touchLoc)) {
								_touchOut = true;
								_bWalk = 1;
								_player->setPreviousPosition();
							}
							else {
								_openOut = false;
								_touchOut = false;
							}
						}

					}
				}

			}


		}
		//=====================================================================
		// open/close/swipe bag-------
		if (!CBag::getInstance()->itemdrag() && !CBag::getInstance()->LightboxState()) {
			if (!_ibagState && _startY < BAG_OPEN_HEIGHT) { // when touched y< set height

															// bag oppened set bag and item position----------------------
				if (fabs(offsetX) < fabs(offsetY) && offsetY > 0) {
					CBag::getInstance()->setPosition(172, 115);
					_ibagState = 1;
					log("bag open state:1");

				}
			}

			else if (_ibagState == 2) {

				if (fabs(offsetX) < fabs(offsetY)) { // close bag
					if (offsetY < 0) { //down
						CBag::getInstance()->ToStateOne();
						_ibagState = 1;
						log("bag open state:1");

					}
				}
			}


			else if (_ibagState == 1 && _startY <= BAG_CLOSE_HEIGHT) {
				//if (_bbagOn && _startY <= BAG_CLOSE_HEIGHT) {

				// bag oppened set bag and item position----------------------
				if (fabs(offsetX) < fabs(offsetY) && offsetY > 0) {
					CBag::getInstance()->ToStateTwo();
					_ibagState = 2;
					log("bag open state:2");

				}

				else if (fabs(offsetX) < fabs(offsetY)) { // close bag
					if (offsetY < 0) { //down
						CBag::getInstance()->setPosition(172, -115);
						_ibagState = 0;
						log("bag close");


					}
				}



			}
		}



		//use items in bag===========================================
		if (_ibagState) { //when bag is open
			int i;
			i = CBag::getInstance()->touchesEnded(_touchLoc, _ibagState, CURRENT_SCENE);

			//to detect item used and its effect-------
			if (i >= 0) {
				// mix mix

				// add sound





			}
		}
	}
	log("end");
}

bool BR_Scene::ToSpot4(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[4].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(4, true);
		_boy->Mirror(false);
		_boy->Stop(false);
		return true;
	}
	else {
		if (_boy->GetReachSpot(1)) {
			GoSpot3(dt, 1);
		}
		else if (_boy->GetReachSpot(5)) {
			GoSpot3(dt, 5);
		}
		else if (_boy->GetReachSpot(0)) {
			ToSpot2(dt);
		}
		else
		{
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x - 240.0f*SPEED*dt, pos.y - 100.0f*SPEED*dt));	
		}
		return false;
	}
}
bool BR_Scene::ToSpot1(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[1].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(1, true);
		_boy->Mirror(false);
		_boy->Stop(true);
		return true;
	}
	else {
		if (_boy->GetReachSpot(2)) {
			GoSpot3(dt, 2);
		}
		else if (_boy->GetReachSpot(4)) {
			GoSpot3(dt, 4);
		}
		else if (_boy->GetReachSpot(0)) {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x - 240.0f*SPEED*dt, pos.y - 100.0f*SPEED*dt));
		}
		else
		{
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x - 260.0f*SPEED*dt, pos.y + 100.0f*SPEED*dt));
		}
		return false;
	}
}
bool BR_Scene::ToSpot2(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[2].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(2, true);
		_boy->Mirror(true);
		_boy->Stop(true);
		return true;
	}
	else {
		if (_boy->GetReachSpot(1)) {
			GoSpot3(dt, 1);
		}
		else if (_boy->GetReachSpot(5)) {
			GoSpot3(dt, 5);
		}
		else if (_boy->GetReachSpot(0)) {
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x + 260.0f*SPEED*dt, pos.y - 100.0f*SPEED*dt));
			return false;
		}
		else
		{
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x + 240.0f*SPEED*dt, pos.y + 100.0f*SPEED*dt));
		}
		return false;
	}
}

bool BR_Scene::ToSpot0(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[0].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(0, true);
		_boy->Mirror(true);
		_boy->Stop(true);
		return true;
	}
	else {
		if (_boy->GetReachSpot(4)) {
			ToSpot2(dt);
		}
		else if (_boy->GetReachSpot(2)) {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x - 260.0f*SPEED*dt, pos.y + 100.0f*SPEED*dt));
			return false;
		}
		else if (_boy->GetReachSpot(5)|| _boy->GetReachSpot(3)) {
			ToSpot1(dt);
		}
		else
		{
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x + 240.0f*SPEED*dt, pos.y + 100.0f*SPEED*dt));
		}
		return false;
	}
}
bool BR_Scene::ToSpot5(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[5].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(5, true);
		_boy->Mirror(true);
		_boy->Stop(false);
		return true;
	}
	else {
		if (_boy->GetReachSpot(4)) {
			GoSpot3(dt, 4);
		}
		else if (_boy->GetReachSpot(2)) {
			GoSpot3(dt, 2);
		}
		else if (_boy->GetReachSpot(0)) {
			ToSpot1(dt);
		}
		else
		{
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x + 260.0f*SPEED*dt, pos.y - 100.0f*SPEED*dt));
		}
		return false;
	}
}

bool BR_Scene::GoSpot3(float dt, int n) {
	Point pos = _boy->getPosition();
	if (_spotRect[3].containsPoint(pos)) {
		_boy->SetReachSpot(-1, false);
		_boy->SetReachSpot(3, true);
		_boy->Stop();
		return true;
	}
	else {
		if (n==2) {
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x - 240 * SPEED*dt, pos.y - 100.0f*SPEED*dt));
			return false;
		}
		else if (n==4) {
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x + 240.0f*SPEED*dt, pos.y + 100.0f*SPEED*dt));
			return false;
		}
		else if (n==1)
		{
			_isWalking = true;
			_boy->Mirror(true);
			_boy->go(false);
			_boy->setPosition(Vec2(pos.x + 260.0f*SPEED*dt, pos.y - 100.0f*SPEED*dt));
			return false;
		}
		else // from 5
		{
			_isWalking = true;
			_boy->Mirror(false);
			_boy->go(true);
			_boy->setPosition(Vec2(pos.x - 260.0f*SPEED*dt, pos.y + 100.0f*SPEED*dt));
			return false;
		}
	}
}

void BR_Scene::ClearToSpot() {
	for (int i = 0; i < 6; i++) {
		_toSpot[i] = false;
	}
}