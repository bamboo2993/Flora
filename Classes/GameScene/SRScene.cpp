#include "SRScene.h"
#include "cocostudio/CocoStudio.h"

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
	_bwithinArea = false;

	_bpickObj = false;
	
	_bsolve[0] =false;
	_bsolve[1] = false;
	_clear = false;
	for (size_t i = 0; i < 5; i++){
		_btouch[i] = false;
	}
	_ibagState = 0;
	

}
SRScene::~SRScene()
{
	xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);
	xmlBag::getInstance()->sortItems();

	CBag::getInstance()->destroyInstance();
	xmlItem::getInstance()->destroyInstance();
	xmlTrigger::getInstance()->destroyInstance();
	xmlBag::getInstance()->destroyInstance();

//	this->removeAllChildren();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
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




	// 音效與音樂 --------------------------------------------------------------------------------

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/GR_bgm.mp3", true);
	//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //尚未實作

	////SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// 停止背景音樂

	//_pour = (cocostudio::ComAudio *)_rootNode->getChildByName("pour")->getComponent("pour");
//	_powder = (cocostudio::ComAudio *) _rootNode->getChildByName("powder")->getComponent("powder");
//	_mixing = (cocostudio::ComAudio *) _rootNode->getChildByName("mix")->getComponent("mix");
//	_grinding = (cocostudio::ComAudio *) _rootNode->getChildByName("grind")->getComponent("grind");
//	_debranch = (cocostudio::ComAudio *) _rootNode->getChildByName("debranch")->getComponent("debranch");

	// set player==============================================================

	_player = new CPlayer("ater45_001.png", "ater45_001.png", *this);
	_player->setAnimation("Animation/boyanim.plist");

	_player->setPosition(Point(1384.09f, 343.64f));
	_player->setRect();




	//set lightbox (experimental procedure) =================================================================
	//_procedure[0] = CLightbox::create();
	//_procedure[0]->init(_zNode[1], "GR_P01_trigger", "GameScene/SRScene/GR_P01.png");
	//this->addChild(_procedure[0], 10000);

	//_procedure[1] = CLightbox::create();
	//_procedure[1]->init(Rect(587.0f, 761.0f,475.0f,282.0f), "GameScene/SRScene/GR_P02.png");
	//this->addChild(_procedure[1], 10000);

	//_procedure[2] = CLightbox::create();
	//_procedure[2]->init(_zNode[2], "GR_Z03_01", "GameScene/SRScene/GR_P03.png");
	//this->addChild(_procedure[2], 10000);


	//_pTrigger = new CTrigger[2];

	//_pTsudokuRect = new Rect;
	//_pTkeyRect = new Rect;

	////set objects=========================================
	SetObject();

	//
	_xmlscene = new xmlScene("./res/xml/xmlfile_SRScene.xml");
	_xmlscene->parseXML(_rootNode, CURRENT_SCENE, _pTrigger);
	//_xmlscene->parseNodeXML(_zNode[0], "zN0");
	//_xmlscene->parseNodeXML(_zNode[1], "zN1");
	//_xmlscene->parseNodeXML(_zNode[2], "zN2");


	//set bag =================================================================


	CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);



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

	//_pTrigger[0].create();
	//_pTrigger[0].Init("GR_Z01_01", _rootNode, true, 1, _zNode[0]); //sudoku trigger

	//_pTrigger[1].create();
	//_pTrigger[1].Init("GR_Z02_02", _rootNode, true, 1, _zNode[1]); //key trigger

	//
	//this->addChild(_pTrigger);




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
	size.height = size.height * 2;
	pos = _detect[2]->getPosition();
	_detectRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	// reset button===============================================
	_reset = (cocos2d::Sprite*)_rootNode->getChildByName("reset");
	size = _reset->getContentSize();
	pos = _reset->getPosition();
	_resetRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);


	////set sudoku target area
	//auto a = (cocos2d::Sprite*)_zNode[2]->getChildByName("GR_Z03_01");
	//size = a->getContentSize();
	//pos = a->getPosition();
	//_pTsudokuRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	////set key target area
	//a = (cocos2d::Sprite*)_zNode[1]->getChildByName("GR_Z02_01");
	//size = a->getContentSize();
	//pos = a->getPosition();
	//_pTkeyRect[0] = Rect(pos.x - size.width / 2, pos.y, size.width, size.height/2);

	//set close target area [znode]
	auto a = (cocos2d::Sprite*)_zNode[0]->getChildByName("close");
	size = a->getContentSize();
	pos = a->getPosition();
	_closeRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);



}


void SRScene::doStep(float dt){
	
	//walk===================================
	//////只能在設定範圍走=============================
	if (_bWalk && _bwithinArea) { //when player walk within walkable region
		
		_player->Walk(_TargetLoc);
		_player->go(_TargetLoc);
		if (_player->Walk(_TargetLoc) == false) {
			_bWalk = 0;
			//if (_bonObj) _bpickObj = true; //pick up obj
			_bpickObj = true; //pick up obj
		}

	}
	
	else if (_bWalk) { // when touched obj in scene that can be picked up
		
		// A
		if (_touchLoc.y >  WALK_AREA_2.getMinY() && _touchLoc.y < LINE_Y && _touchLoc.x < WALK_AREA_2.getMinX()) {
			log("walk A");
			_player->Walk(Vec2(WALK_AREA_2.getMinX(), _touchLoc.y));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(WALK_AREA_2.getMinX(), _touchLoc.y)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}
		// A-1
		else if (_touchLoc.y >  WALK_AREA_2.getMaxY() && _touchLoc.x < WALK_AREA_2.getMinX()) {
			log("walk A-1");
			_player->Walk(Vec2(WALK_AREA_2.getMinX(), WALK_AREA_2.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(WALK_AREA_2.getMinX(), WALK_AREA_2.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}

		// A-2
		else if (_touchLoc.y <  WALK_AREA_1.getMaxY() && _touchLoc.x < WALK_AREA_2.getMinX()) {
			log("walk A-2");
			_player->Walk(Vec2(WALK_AREA_2.getMinX(), WALK_AREA_2.getMinY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(WALK_AREA_2.getMinX(), WALK_AREA_2.getMinY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}

		// A-3
		else if (_touchLoc.y <  WALK_AREA_1.getMaxY() && _touchLoc.x > WALK_AREA_2.getMinX() && _touchLoc.x < WALK_AREA_1.getMinX()) {
			log("walk A-3");
			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMinY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMinY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}


		// B-1
		else if (_touchLoc.y > LINE_Y  && _touchLoc.x > WALK_AREA_2.getMinX() && _touchLoc.x < WALK_AREA_3.getMinX()) {
			log("walk B-1");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}


		// B-2
		else if (_touchLoc.y > LINE_Y  && _touchLoc.x > WALK_AREA_3.getMinX() && _touchLoc.x < WALK_AREA_4.getMinX()) {
			log("walk B-2");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}


		// B-3
		else if (_touchLoc.y > LINE_Y  && _touchLoc.x > WALK_AREA_4.getMinX() && _touchLoc.x < 958.41f) {
			log("walk B-3");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}

		// C-1
		else if (_touchLoc.y > LINE_Y && _touchLoc.x > 958.41f && _touchLoc.x < WALK_AREA_4.getMaxX()) {
			log("walk C-1");
			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}

		}

		// C-2
		else if (_touchLoc.y > LINE_Y && _touchLoc.x > WALK_AREA_4.getMaxX() && _touchLoc.x < WALK_AREA_3.getMaxX()) {
			log("walk C-2");
			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}

		}

		// C-3
		else if (_touchLoc.y > LINE_Y && _touchLoc.x > WALK_AREA_3.getMaxX() && _touchLoc.x < WALK_AREA_2.getMaxX()) {
			log("walk C-3");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}

		// C-4
		else if (_touchLoc.y > WALK_AREA_2.getMaxY() && _touchLoc.x > WALK_AREA_2.getMaxX()) {
			log("walk C-4");
			_player->Walk(Vec2(WALK_AREA_2.getMaxX(), WALK_AREA_2.getMaxY()));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(WALK_AREA_2.getMaxX(), WALK_AREA_2.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}

		}



		// D
		else if (_touchLoc.y < WALK_AREA_2.getMaxY() && _touchLoc.x >  WALK_AREA_1.getMaxX()) {
			log("walk D");
			_player->Walk(Vec2(WALK_AREA_2.getMaxX(), _touchLoc.y));
			_player->go(_TargetLoc);
			if (_player->Walk(Vec2(WALK_AREA_2.getMaxX(), _touchLoc.y)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}

		}

	}

	else {
		_player->Stop();
	}

	//// pick up obj ==========================
	PickObject(dt);

	
}

void SRScene::PickObject(float dt) {

	// pick up obj ==========================
	if (_bpickObj) { // when item in scene is being touched
		// check which trigger is being touched
		//log("_bpickObj = true");
	
		 //create the corresponding item in bag


		//open node=========
		if (_btouchNode[0]) {
			_zNode[0]->setVisible(true);

			_bopenNode[0] = !_bopenNode[0];
			_btouchNode[0] = !_btouchNode[0];
			//log("show detect");

		}
		else if (_btouchNode[1]) {
			_zNode[1]->setVisible(true);

			_bopenNode[1] = !_bopenNode[1];
			_btouchNode[1] = !_btouchNode[1];
			//log("show detect node2");

		}

		else if (_btouchNode[2]) {
			_zNode[2]->setVisible(true);

			_bopenNode[2] = !_bopenNode[2];
			_btouchNode[2] = !_btouchNode[2];
			//log("show detect node3");

		}

		_bpickObj = false;

	}

	/*if (_bopenNode[0]) {

		_pTrigger[0].doStep(dt);


		//create the corresponding item in bag
		if (_pTrigger[0].GetAddToBag() && !_pTrigger[0].GetPicked()) {

			CBag::getInstance()->AddObj("B_sudoku.png", 1, _pTsudokuRect, false);

			_pTrigger[0].SetAddToBag(false);
			_pTrigger[0].SetPicked(true); // if the object is picked and added into the bag

			_xmlscene->editItemState("GR_Z01_01", false, _zNode[0], 0, 1);
		}
		
	}



	else if (_bopenNode[1]) {
		_pTrigger[1].doStep(dt);

		if (_bsolve[0]) _procedure[0]->doStep(dt);
		 _procedure[1]->doStep(dt);

		//create the corresponding item in bag
		if (_pTrigger[1].GetAddToBag() && !_pTrigger[1].GetPicked()) {

			CBag::getInstance()->AddObj("B_key.png", 1, _pTkeyRect, false);

			_pTrigger[1].SetAddToBag(false);
			_pTrigger[1].SetPicked(true); // if the object is picked and added into the bag

			_xmlscene->editItemState("GR_Z02_02", false, _zNode[1], 2, 3);
		}
		

	}
	else if (_bopenNode[2]) {
		if (_bsolve[1]) _procedure[2]->doStep(dt);
	}

	*/

}


void SRScene::reset() {
	CBag::getInstance()->reset();

	// reset scene===============================
	_bWalk = false; //detect if player is walking
	_bwithinArea = false;//判斷 walk

	_bpickObj = false;
	
	_zNode[0]->setVisible(false);
	_zNode[1]->setVisible(false);
	_zNode[2]->setVisible(false);

	_procedure[0]->setVisible(false);
	_procedure[1]->setVisible(false);
	_procedure[2]->setVisible(false);

	_xmlscene->editItemState(0, false, _rootNode);
	_xmlscene->editItemState(1, true, _zNode[0]);
	_xmlscene->editItemState(2, false, _zNode[1]);
	_xmlscene->editItemState(3, true, _zNode[1]);
	_xmlscene->editItemState(4, false, _zNode[2]);

	_pTrigger[0].reset();
	_pTrigger[1].reset();

	xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);

	_bsolve[0] = false;
	_bsolve[1] = false;

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


	if (_ibagState) { //when bag is open
		//use items in bag===========================================
		CBag::getInstance()->touchesBegan(_touchLoc);

	}

	

	return true;
}


void  SRScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{

	_touchLoc = pTouch->getLocation();

	//use items in bag===========================================
	if (_ibagState) { //when bag is open
		CBag::getInstance()->touchesMoved(_touchLoc);
	}
	
	
}

void  SRScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{

	_touchLoc = pTouch->getLocation();

	//swipe gesture
	float offsetX = _touchLoc.x - _startX;
	float offsetY = _touchLoc.y - _startY;

	if (_ibagState != 2) {

		// [WALK + PICK OBJECT]===================
		if (offsetX == 0 && offsetY == 0 && !CBag::getInstance()->LightboxState()) { // when screen tapped
			if (_touchLoc.y > 227) {
				//walk area ====================================
				if (WALK_AREA_1.containsPoint(_touchLoc) || WALK_AREA_2.containsPoint(_touchLoc) || WALK_AREA_3.containsPoint(_touchLoc) ||
					WALK_AREA_4.containsPoint(_touchLoc)) {
					// detect if touch pts are in walkable area
					_bwithinArea = true;
					log("walk!!");

				}
				else  _bwithinArea = false;

				////player walk =====================================================

				//放大鏡沒開 --------------
				if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {
					//沒按重置-------------
					if (!_resetRect.containsPoint(_touchLoc)) {
						_bWalk = 1;
						_player->setPreviousPosition();

						if (_touchLoc.x > _player->_rpos.x) {
							_player->_bSide = 1;
							_player->Mirror();
						}
						else {
							_player->_bSide = 0;
							_player->Mirror();
						}

						//-------------------------------
						_TargetLoc = _touchLoc;

						//====================================

					}
					else {
						// reset button=========================
						reset();
					}

				}




				//znode[0]開---------------------
				if (_bopenNode[0]) {
					_bwithinArea = false;
					//_pTrigger[0].touchesBegan(_touchLoc);

					if (_closeRect.containsPoint(_touchLoc)) {
						_bopenNode[0] = !_bopenNode[0];
						_zNode[0]->setVisible(false);
						log("close detect");
					}

				}
				//znode[1]開---------------------
				else if (_bopenNode[1]) {
					_bwithinArea = false;
					/*_pTrigger[1].touchesBegan(_touchLoc);

					if(_bsolve[0]) _procedure[0]->TouchBegan(_touchLoc);
					_procedure[1]->TouchBegan(_touchLoc);*/

					if (_closeRect.containsPoint(_touchLoc)) {
						_bopenNode[1] = !_bopenNode[1];
						_zNode[1]->setVisible(false);
						log("close detect");
					}
				}
				//znode[2]開---------------------
				else if (_bopenNode[2]) {
					_bwithinArea = false;
					//if (_bsolve[1])_procedure[2]->TouchBegan(_touchLoc);

					if (_closeRect.containsPoint(_touchLoc)) {
						_bopenNode[2] = !_bopenNode[2];
						_zNode[2]->setVisible(false);
						log("close detect");
					}
				}


				//0
				if (!_bopenNode[0] && _detectRect[0].containsPoint(_touchLoc)) {
					_btouchNode[0] = true;
					log("touched detect");
				}
				else if (!_bopenNode[0] && !_detectRect[0].containsPoint(_touchLoc)) {
					_btouchNode[0] = false;
				}
				//1
				if (!_bopenNode[1] && _detectRect[1].containsPoint(_touchLoc)) {
					_btouchNode[1] = true;
					log("touched detect node2");
				}
				else if (!_bopenNode[1] && !_detectRect[1].containsPoint(_touchLoc)) {
					_btouchNode[1] = false;
				}
				//2
				if (!_bopenNode[2] && _detectRect[2].containsPoint(_touchLoc)) {
					_btouchNode[2] = true;
					log("touched detect node2");
				}
				else if (!_bopenNode[2] && !_detectRect[2].containsPoint(_touchLoc)) {
					_btouchNode[2] = false;
				}

			}
			else {
				if (!_ibagState) {
					//walk area ====================================
					if (WALK_AREA_1.containsPoint(_touchLoc) || WALK_AREA_2.containsPoint(_touchLoc) || WALK_AREA_3.containsPoint(_touchLoc) ||
						WALK_AREA_4.containsPoint(_touchLoc)) {
						// detect if touch pts are in walkable area
						_bwithinArea = true;
						log("walk!!");

					}
					else  _bwithinArea = false;

					////player walk =====================================================

					//放大鏡沒開 --------------
					if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {
						//沒按重置-------------

						_bWalk = 1;
						_player->setPreviousPosition();

						if (_touchLoc.x > _player->_rpos.x) {
							_player->_bSide = 1;
							_player->Mirror();
						}
						else {
							_player->_bSide = 0;
							_player->Mirror();
						}

						//-------------------------------
						_TargetLoc = _touchLoc;

						//====================================

					}

					//znode[0]開---------------------
					if (_bopenNode[0]) {
						_bwithinArea = false;
						//_pTrigger[0].touchesBegan(_touchLoc);

						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[0] = !_bopenNode[0];
							_zNode[0]->setVisible(false);
							log("close detect");
						}

					}

					//0
					if (!_bopenNode[0] && _detectRect[0].containsPoint(_touchLoc)) {
						_btouchNode[0] = true;
						log("touched detect");
					}
					else if (!_bopenNode[0] && !_detectRect[0].containsPoint(_touchLoc)) {
						_btouchNode[0] = false;
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
