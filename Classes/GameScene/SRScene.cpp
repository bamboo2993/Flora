#include "SRScene.h"
#include "cocostudio/CocoStudio.h"

#include "C2_Scene_01.h"

#define ROOT_NODE   "SRScene.csb"
#define CURRENT_SCENE   "SRScene.cpp"

#define WALK_AREA_1 Rect(968.67f, 2.0f, 743.2f, 183.5f)
#define WALK_AREA_2 Rect(435.79f, 187.92f, 1149.1f, 276.0f)
#define WALK_AREA_3 Rect(611.56f, 464.56f, 763.7f, 73.7f)
#define WALK_AREA_4 Rect(796.02f, 536.79f, 393.2f, 64.3f)

#define LINE_Y 650

#define BAG_OPEN_HEIGHT 150.0f
#define BAG_CLOSE_HEIGHT 250.0f

#define BACKGROUND_FRONT "SR_S02"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;





Scene* SRScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SRScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

SRScene::SRScene() {
	_bWalk = false; //detect if player is walking

	//判斷 walk
	
	_bsolve[0] =false;
	_bsolve[1] = false;
	_clear = false;
	_ibagState = 0;

	_btogger[0] = false;
	_btogger[1] = false;

	_touchOut = false;
	_openOut = false;

	
	for (size_t i = 0; i < TALK_AREA; i++){
		_touchTalk[i] = false;
		_openTalk[i] = false;
	}




}
SRScene::~SRScene()
{
	xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);
	xmlBag::getInstance()->sortItems();

	//CBag::getInstance()->destroyInstance();
	xmlItem::getInstance()->destroyInstance();
	xmlTrigger::getInstance()->destroyInstance();
	xmlBag::getInstance()->destroyInstance();

//	this->removeAllChildren();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();

	if(_lock!=NULL)	delete _lock;
	if (_player != NULL)delete _player;
	if (_procedure != NULL)delete[] _procedure;
//	if (_pTrigger != NULL)delete _pTrigger;

}

// on "init" you need to initialize your instance
bool SRScene::init()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//// 利用程式直接產生序列幀動畫 
	//// STEP 1 : 讀入儲存多張圖片的 plist 檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SRScene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/bagItem.plist");
	// ------------------------------------------------------------------------------------------------- 


	////以 Sprite 作為背景 ========================================================================

	_rootNode = CSLoader::createNode(ROOT_NODE);
	addChild(_rootNode);

	Sprite *bkimage = (cocos2d::Sprite *) _rootNode->getChildByName(BACKGROUND_FRONT);
	this->addChild(bkimage, 100);



	//set zoom node====================
	_zNode[0] = (cocos2d::Node*)_rootNode->getChildByName("Node_SR_Z01");
	addChild(_zNode[0], 300);


	_zNode[1] = (cocos2d::Node*)_rootNode->getChildByName("Node_SR_Z02");
	addChild(_zNode[1], 300);

	_zNode[2] = (cocos2d::Node*)_rootNode->getChildByName("Node_SR_Z03");
	addChild(_zNode[2], 300);

	_zNode[3] = (cocos2d::Node*)_rootNode->getChildByName("Node_SR_Z04");
	addChild(_zNode[3], 300);
	

	// set player==============================================================

	_player = new CPlayer("aterStand01.png", "aterStand02.png", *this);
	_player->setAnimation("Animation/boyanim.plist");

	_player->setPosition(Point(1384.09f, 343.64f));
	_player->setRect();




	//set lightbox (experimental procedure) =================================================================
	_procedure[0] = CLightbox::create();
	_procedure[0]->init(_zNode[0], "SR_Z01_P01_trigger", "GameScene/SRScene/SR_Z01_P01.png");
	this->addChild(_procedure[0], 10000);

	_procedure[1] = CLightbox::create();
	_procedure[1]->init(_zNode[1], "SR_Z02_P01_trigger", "GameScene/SRScene/SR_Z02_P01.png");
	this->addChild(_procedure[1], 10000);

	_procedure[2] = CLightbox::create();

	_procedure[2]->setPage(1, "GameScene/SRScene/SR_Z04_P01.png");
	_procedure[2]->setPage(2, "GameScene/SRScene/SR_Z04_P02.png");
	_procedure[2]->setPage(3, "GameScene/SRScene/SR_Z04_P03.png");
	_procedure[2]->setPage(4, "GameScene/SRScene/SR_Z04_P04.png");
	_procedure[2]->init(_zNode[3], "SR_Z04_03", 4);
	this->addChild(_procedure[2], 10000);


	//_pTrigger = new CTrigger[2];

	//_pTsudokuRect = new Rect;
	//_pTkeyRect = new Rect;

	////set objects=========================================
	SetObject();


	//set lock ===================================
	_lock = new CLock();
	_lock->init(4, _zNode[2]->getChildByName("Node_lock_SR"), 4, false, 1);

	_lock->SetArea(_toggerRect[0]);
	_lock->SetPassword(4, 1233);
	_lock->SetExitArea(_closeRect);
	
	_xmlscene = new xmlScene("./res/xml/xmlfile_SRScene.xml");
	//_xmlscene->parseXML(_rootNode, CURRENT_SCENE, _pTrigger);
	//_xmlscene->parseNodeXML(_zNode[0], "zN0");
	//_xmlscene->parseNodeXML(_zNode[1], "zN1");
	//_xmlscene->parseNodeXML(_zNode[2], "zN2");


	//set bag =================================================================


	//CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);


	// 音效與音樂 --------------------------------------------------------------------------------

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/GR_bgm.mp3", true);
	//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //尚未實作

	////SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// 停止背景音樂

	//_pour = (cocostudio::ComAudio *)_rootNode->getChildByName("pour")->getComponent("pour");
	//	_powder = (cocostudio::ComAudio *) _rootNode->getChildByName("powder")->getComponent("powder");
	//	_mixing = (cocostudio::ComAudio *) _rootNode->getChildByName("mix")->getComponent("mix");
	//	_grinding = (cocostudio::ComAudio *) _rootNode->getChildByName("grind")->getComponent("grind");
	//	_debranch = (cocostudio::ComAudio *) _rootNode->getChildByName("debranch")->getComponent("debranch");

	//-------------------------------------------------------------------------------------------------

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(SRScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(SRScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(SRScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(SRScene::doStep));
	return true;
}





void SRScene::SetObject() {
	//set objects


	_detect[0] = (cocos2d::Sprite*)_rootNode->getChildByName("SR_Z01_trigger");
	Size size = _detect[0]->getContentSize();
	size.width = size.width * 3;
	Point pos = _detect[0]->getPosition();
	_detectRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_detect[1] = (cocos2d::Sprite*)_rootNode->getChildByName("SR_Z02_trigger");
	size = _detect[1]->getContentSize();
	size.width = size.width * 0.9;
	size.height = size.height * 0.6;
	pos = _detect[1]->getPosition();
	_detectRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_detect[2] = (cocos2d::Sprite*)_rootNode->getChildByName("SR_Z03_trigger");
	size = _detect[2]->getContentSize();
	size.width = size.width * 3;
	size.height = size.height * 1.64;
	pos = _detect[2]->getPosition();
	_detectRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_detect[3] = (cocos2d::Sprite*)_rootNode->getChildByName("SR_Z04_trigger");
	size = _detect[3]->getContentSize();
	size.width = size.width * 0.9;
	size.height = size.height * 1.4;
	pos = _detect[3]->getPosition();
	_detectRect[3] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);




	// reset button===============================================
	_reset = (cocos2d::Sprite*)_rootNode->getChildByName("reset");
	size = _reset->getContentSize();
	pos = _reset->getPosition();
	_resetRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);


	//set certificate togger area
	auto a = (cocos2d::Sprite*)_zNode[2]->getChildByName("SR_Z03_02");
	size = a->getContentSize();
	pos = a->getPosition();
	_toggerRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//set book togger area
	a = (cocos2d::Sprite*)_zNode[3]->getChildByName("SR_Z04_01");
	size = a->getContentSize();
	pos = a->getPosition();
	_toggerRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//set close target area [znode]
	a = (cocos2d::Sprite*)_zNode[0]->getChildByName("close");
	size = a->getContentSize();
	pos = a->getPosition();
	_closeRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//set talk area===============================
	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_cormer_0");
	size = a->getContentSize();
	pos = a->getPosition();
	size.width = size.width*2.53f;
	_talkRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_cormer_1");
	pos = a->getPosition();
	_talkRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_sofa_2");
	pos = a->getPosition();
	size = a->getContentSize();
	size.width = size.width*1.5f;
	size.height = size.height *1.3f;
	_talkRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_self_3");
	size = a->getContentSize();
	pos = a->getPosition();
	size.width = size.width*5.0f;
	size.height = size.height *2.0f;
	_talkRect[3] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	//set talk area===============================
	a = (cocos2d::Sprite*)_rootNode->getChildByName("exit");
	size = a->getContentSize();
	pos = a->getPosition();
	size.width = size.width*1.71f;
	size.height = size.height *0.453f;
	_outRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

}


void SRScene::doStep(float dt){

	//walk===================================
	// when item in scene is being touched
	// check which trigger is being touched
	if (_btouchNode[0]) {
		_player->Walk(Vec2(508.25f, 121.06f));
		_player->go(Vec2(508.25f, 121.06f));
		if (_player->Walk(Vec2(508.25f, 121.06f)) == false) {
			_bWalk = 0;
			//if (_bonObj) _bpickObj = true; //pick up obj
			//pick up obj

			_zNode[0]->setVisible(true);

			_bopenNode[0] = !_bopenNode[0];
			_btouchNode[0] = !_btouchNode[0];
			//log("show detect");
		}
	}

	else if (_btouchNode[1] || _btouchNode[3]) {
		_player->Walk(Vec2(1239.22f, 471.63f));
		_player->go(Vec2(1239.22f, 471.63f));
		if (_player->Walk(Vec2(1239.22f, 471.63f)) == false) {
			_bWalk = 0;
			
			if (_btouchNode[1]) {
				_zNode[1]->setVisible(true);

				_bopenNode[1] = !_bopenNode[1];
				_btouchNode[1] = !_btouchNode[1];
			}
			else {
				_zNode[3]->setVisible(true);

				_bopenNode[3] = !_bopenNode[3];
				_btouchNode[3] = !_btouchNode[3];
			}

			//pick up obj
		}
	}
	

	else if (_btouchNode[2]) {
		_player->Walk(Vec2(594.03f, 441.79f));
		_player->go(Vec2(594.03f, 441.79f));
		if (_player->Walk(Vec2(594.03f, 441.79f)) == false) {
			_bWalk = 0;
			//if (_bonObj) _bpickObj = true; //pick up obj

			_zNode[2]->setVisible(true);

			_bopenNode[2] = !_bopenNode[2];
			_btouchNode[2] = !_btouchNode[2];

			//pick up obj
		}
	}

	

	else if (_touchTalk[0] || _touchTalk[3]) {
		_player->Walk(Vec2(945.27f, 435.06f));
		_player->go(Vec2(945.27f, 435.06f));
		if (_player->Walk(Vec2(945.27f, 435.06f)) == false) {
			_bWalk = 0;

			//pick up obj
			if (_touchTalk[0]) {
				_touchTalk[0] = !_touchTalk[0];
				_openTalk[0] = !_openTalk[0];
			}
			else {
				_touchTalk[3] = !_touchTalk[3];
				_openTalk[3] = !_openTalk[3];
			}

			//log("show detect");
		}

	}
	
	else if (_touchTalk[1]) {
		_player->Walk(Vec2(1340.73f, 303.25f));
		_player->go(Vec2(1340.73f, 303.25f));
		if (_player->Walk(Vec2(1340.73f, 303.25f)) == false) {
			_bWalk = 0;

			//pick up obj
			_touchTalk[1] = !_touchTalk[1];
			_openTalk[1] = !_openTalk[1];

			//log("show detect");
		}

	}

	else if (_touchTalk[2]) {
		_player->Walk(Vec2(1480.58f, 118.30f));
		_player->go(Vec2(1480.58f, 118.30f));
		if (_player->Walk(Vec2(1480.58f, 118.30f)) == false) {
			_bWalk = 0;

			//pick up obj

			_touchTalk[2] = !_touchTalk[2];
			_openTalk[2] = !_openTalk[2];
			//log("show detect");
		}

	}



	else if (_touchOut) {
		_player->Walk(Vec2(1497.33f, -146.23f));
		_player->go(Vec2(1497.33f, -146.23f));
		if (_player->Walk(Vec2(1497.33f, -146.23f)) == false) {
			_bWalk = 0;

			//pick up obj
			_touchOut = !_touchOut;
			_openOut = !_openOut;

			//log("show detect");
		}

	}

	else {
		_player->Stop();
	}

	//// pick up obj ==========================
	PickObject(dt);
	_lock->doStep(dt);
	
	if (_lock->GetSolved()) _zNode[2]->getChildByName("SR_Z03_E02")->setVisible(true);
}

void SRScene::PickObject(float dt) {	
	if (_bopenNode[0]) {
		_player->SetFront(true);
		_player->Mirror(false);
		_procedure[0]->doStep(dt);
	}

	else if (_bopenNode[1]) {
		_player->Mirror(true);
		_player->SetFront(false);
		_procedure[1]->doStep(dt);	

	}
	else if (_bopenNode[3]) {
		_player->Mirror(true);
		_player->SetFront(false);
		if (_btogger[1]) _procedure[2]->doStep(dt);
	}

	else if (_openTalk[0] || _openTalk[1]) {
		_player->Mirror(true);
		_player->SetFront(false);
		const char* x;
		if (_talkContent==0) x = "dialoge/SR/SR_corner01.png";
		else if(_talkContent == 1)x = "dialoge/SR/SR_corner02.png";
		else if (_talkContent == 2)x = "dialoge/SR/SR_corner03.png";
		else if (_talkContent == 3)x = "dialoge/SR/SR_corner04.png";
		_player->Talk(x, false);
		if(_talkContent ==1) _player->Talk(x, true);
		_player->SetIsTalking(true);
		_openTalk[0] = false;
		_openTalk[1] = false;
	}

	else if (_openTalk[2]) {
		_player->Mirror(true);
		_player->SetFront(true);	
		const char* x;
		if (_talkContent) x = "dialoge/SR/SR_sofa01.png";
		else x = "dialoge/SR/SR_sofa02.png";
		_player->Talk(x, false);
		_player->SetIsTalking(true);
		_openTalk[2] = false;
	}
	else if (_openTalk[3]) {
		_player->Mirror(false);
		_player->SetFront(false);
		const char* x;
		if (_talkContent == 0) x = "dialoge/SR/SR_shelf01.png";
		else if (_talkContent == 1) x = "dialoge/SR/SR_shelf02.png";
		else if (_talkContent == 2) x = "dialoge/SR/SR_shelf03.png";
		_player->Talk(x, true);
		_player->SetIsTalking(true);
		_openTalk[3] = false;
	}


	else if (_openOut) {
		log("to corridor");
		_openOut = !_openOut;
		// to corridor================================
		this->unschedule(schedule_selector(SRScene::doStep));
		C2_Scene_01::_from = 0;
		Director::getInstance()->replaceScene(C2_Scene_01::createScene());
	}
}


void SRScene::reset() {
	CBag::getInstance()->reset();

	// reset scene===============================
	_bWalk = false; //detect if player is walking
	
	_zNode[0]->setVisible(false);
	_zNode[1]->setVisible(false);
	_zNode[2]->setVisible(false);
	_zNode[3]->setVisible(false);

	_procedure[0]->setVisible(false);
	_procedure[1]->setVisible(false);
	_procedure[2]->setVisible(false);

	//_xmlscene->editItemState(0, false, _rootNode);
	//_xmlscene->editItemState(1, true, _zNode[0]);
	//_xmlscene->editItemState(2, false, _zNode[1]);
	//_xmlscene->editItemState(3, true, _zNode[1]);
	//_xmlscene->editItemState(4, false, _zNode[2]);

	//_pTrigger[0].reset();
	//_pTrigger[1].reset();

	xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);

	_bsolve[0] = false;
	_bsolve[1] = false;

	_touchOut = false;
	_openOut = false;

}


bool SRScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	_touchLoc = pTouch->getLocation();
	_prePos = _touchLoc;

	Rect xx = Rect(1900.0f,1440.0f,140.0f,98.0f);
	if (xx.containsPoint(_touchLoc)) {
		xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);
		xmlBag::getInstance()->sortItems();
	}



	//swipe gesture
	_startX = _touchLoc.x;
	_startY = _touchLoc.y;

	if (!_player->GetIsTalking()) {
		if (_ibagState) { //when bag is open
						  //use items in bag===========================================
			CBag::getInstance()->touchesBegan(_touchLoc);

		}

		if (_btogger[0]) {
			//lock==================
			_lock->TouchBegan(_touchLoc);
		}
	}

	



	return true;
}


void  SRScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{

	_touchLoc = pTouch->getLocation();

	if (!_player->GetIsTalking()) {
		//use items in bag===========================================
		if (_ibagState) { //when bag is open
			CBag::getInstance()->touchesMoved(_touchLoc);
		}

		if (_btogger[0]) {
			//lock==================
			_lock->TouchMoved(_touchLoc);
		}
	}
	
}

void  SRScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{

	_touchLoc = pTouch->getLocation();

	//swipe gesture
	float offsetX = _touchLoc.x - _startX;
	float offsetY = _touchLoc.y - _startY;

	if (!_player->GetIsTalking()) {
		if (_ibagState != 2) {

			// [WALK + PICK OBJECT]===================
			if (offsetX == 0 && offsetY == 0 && !CBag::getInstance()->LightboxState()) { // when screen tapped
				_TargetLoc = _touchLoc;
				if (_touchLoc.y > 227) {

					////player walk =====================================================

					//放大鏡沒開 --------------
					if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2] && !_bopenNode[3]) {
						//沒按重置-------------
						if (_resetRect.containsPoint(_touchLoc)) {

							reset();
						}
						else {
							if (_talkRect[0].containsPoint(_touchLoc)) {
								_touchTalk[0] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 4;
							}
							else {
								_openTalk[0] = false;
								_touchTalk[0] = false;
							}


							if (_talkRect[1].containsPoint(_touchLoc)) {
								_touchTalk[1] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 4;
							}
							else {
								_openTalk[1] = false;
								_touchTalk[1] = false;
							}


							if (_talkRect[2].containsPoint(_touchLoc)) {
								_touchTalk[2] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 2;

							}
							else {
								_openTalk[2] = false;
								_touchTalk[2] = false;
							}


							if (_talkRect[3].containsPoint(_touchLoc)) {
								_touchTalk[3] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 3;
							}
							else {
								_openTalk[3] = false;
								_touchTalk[3] = false;
							}



						}

					}




					//znode[0]開---------------------
					if (_bopenNode[0]) {

						if (!_procedure[0]->GetOpen()) {
							if (_closeRect.containsPoint(_touchLoc)) {
								_bopenNode[0] = !_bopenNode[0];
								_zNode[0]->setVisible(false);
								log("close detect");
							}
						}

						_procedure[0]->TouchBegan(_touchLoc);

					}
					//znode[1]開---------------------
					else if (_bopenNode[1]) {

						if (!_procedure[1]->GetOpen()) {
							if (_closeRect.containsPoint(_touchLoc)) {
								_bopenNode[1] = !_bopenNode[1];
								_zNode[1]->setVisible(false);
								log("close detect");
							}
						}

						_procedure[1]->TouchBegan(_touchLoc);

					}
					//znode[2]開---------------------
					else if (_bopenNode[2] && !_lock->GetState()) {
						if (_btogger[0]) _lock->TouchEnded(_touchLoc);
						else {
							if (_toggerRect[0].containsPoint(_touchLoc)) {
								_btogger[0] = true;
								_zNode[2]->getChildByName("SR_Z03_01")->setVisible(!_btogger[0]);
								_zNode[2]->getChildByName("SR_Z03_02")->setVisible(_btogger[0]);
							}
						}

						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[2] = !_bopenNode[2];
							_zNode[2]->setVisible(false);

							_btogger[0] = false;
							_zNode[2]->getChildByName("SR_Z03_01")->setVisible(!_btogger[0]);
							_zNode[2]->getChildByName("SR_Z03_02")->setVisible(_btogger[0]);
							log("close detect");
						}
					}


					else if (_lock->GetState()) {
						_lock->TouchEnded(_touchLoc);

					}

					//znode[3]開---------------------
					else if (_bopenNode[3]) {
						/*_pTrigger[1].touchesBegan(_touchLoc);*/
						//_procedure[1]->TouchBegan(_touchLoc);


						if (!_procedure[2]->GetOpen()) {
							if (_toggerRect[1].containsPoint(_touchLoc)) {
								if (!_btogger[1]) {
									_btogger[1] = true;
									_zNode[3]->getChildByName("SR_Z04_01")->setVisible(!_btogger[1]);
									_zNode[3]->getChildByName("SR_Z04_02")->setVisible(_btogger[1]);
								}
							}

							else if (_closeRect.containsPoint(_touchLoc)) {
								_player->Mirror(true);
								_player->SetFront(false);
								_bopenNode[3] = !_bopenNode[3];
								_zNode[3]->setVisible(false);

								_btogger[1] = false;
								_zNode[3]->getChildByName("SR_Z04_01")->setVisible(!_btogger[1]);
								_zNode[3]->getChildByName("SR_Z04_02")->setVisible(_btogger[1]);

								log("close detect");
							}
						}

						if (_btogger[1]) _procedure[2]->TouchBegan(_touchLoc); // click book 2 [light box] ===============
					}

					if (!_bWalk) {
						//0
						if (!_bopenNode[0] && _detectRect[0].containsPoint(_touchLoc)) {
							_btouchNode[0] = true;

							_bWalk = 1;
							_player->setPreviousPosition();

							//-------------------------------
							_TargetLoc = _touchLoc;
							log("touched detect");
						}
						else if (!_bopenNode[0] && !_detectRect[0].containsPoint(_touchLoc)) {
							_btouchNode[0] = false;
						}
						//1
						if (!_bopenNode[1] && _detectRect[1].containsPoint(_touchLoc)) {
							_btouchNode[1] = true;

							_bWalk = 1;
							_player->setPreviousPosition();

							//-------------------------------
							_TargetLoc = _touchLoc;
							log("touched detect node2");
						}
						else if (!_bopenNode[1] && !_detectRect[1].containsPoint(_touchLoc)) {
							_btouchNode[1] = false;
						}
						//2
						if (!_bopenNode[2] && _detectRect[2].containsPoint(_touchLoc)) {
							_btouchNode[2] = true;

							_bWalk = 1;
							_player->setPreviousPosition();

							//-------------------------------
							_TargetLoc = _touchLoc;
							log("touched detect node2");
						}
						else if (!_bopenNode[2] && !_detectRect[2].containsPoint(_touchLoc)) {
							_btouchNode[2] = false;
						}

						//3
						if (!_bopenNode[3] && _detectRect[3].containsPoint(_touchLoc)) {
							_btouchNode[3] = true;

							_bWalk = 1;
							_player->setPreviousPosition();

							//-------------------------------
							_TargetLoc = _touchLoc;
							log("touched detect node2");
						}
						else if (!_bopenNode[3] && !_detectRect[3].containsPoint(_touchLoc)) {
							_btouchNode[3] = false;
						}
					}


				}
				else {
					if (!_ibagState) {

						//放大鏡沒開 --------------
						if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {
							if (_talkRect[3].containsPoint(_touchLoc)) {
								_touchTalk[3] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 3;
							}
							else {
								_openTalk[3] = false;
								_touchTalk[3] = false;
							}

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
						
						//znode[0]開---------------------
						if (_bopenNode[0]) {
							//_pTrigger[0].touchesBegan(_touchLoc);

							if (_closeRect.containsPoint(_touchLoc)) {
								_bopenNode[0] = !_bopenNode[0];
								_zNode[0]->setVisible(false);
								log("close detect");
							}

						}

						if (!_bWalk) {
							//0
							if (!_bopenNode[0] && _detectRect[0].containsPoint(_touchLoc)) {
								_btouchNode[0] = true;

								_bWalk = 1;
								_player->setPreviousPosition();

								//-------------------------------
								_TargetLoc = _touchLoc;
								log("touched detect");
							}
							else if (!_bopenNode[0] && !_detectRect[0].containsPoint(_touchLoc)) {
								_btouchNode[0] = false;
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
			i = CBag::getInstance()->touchesEnded(_touchLoc, _ibagState, CURRENT_SCENE, _pTrigger);

			//to detect item used and its effect-------
			/*if (i >= 0) {
			// mix mix

			// add sound

			if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_sudoku.png")) {
			// add debranch
			//_grinding->playEffect();
			_xmlscene->editItemState("GR_Z03_01", true, _zNode[2], 0, 4);
			_xmlscene->editItemState("GR_S01_01", true, _rootNode, 0, 4);
			_bsolve[1] = true;

			}

			else if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_key.png")) {
			// add debranch
			//_grinding->playEffect();
			_xmlscene->editItemState("GR_Z02_01", true, _zNode[1], 2, 3);
			_bsolve[0] = true;

			}



			}*/
		}



	}
	else {
		_player->SetIsTalking(false);
		_player->StopTalking();
	}

}
