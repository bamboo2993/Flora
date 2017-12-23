#include "GRScene.h"
#include "cocostudio/CocoStudio.h"

#define ROOT_NODE   "GRScene.csb"
#define CURRENT_SCENE   "GRScene.cpp"

#define WALK_AREA_1 Rect(195.6f, 2.0f, 554.5f, 512.6f)
#define WALK_AREA_2 Rect(751.0f, 321.0f, 254.0f, 214.8f)
#define WALK_AREA_3 Rect(875.7f, 540.3f, 90.4f, 97.0f)
#define WALK_AREA_4 Rect(969.0f, 560.0f, 340.4f, 178.5f)
#define WALK_AREA_5 Rect(1010.0f, 433.8f, 1039.0f, 124.4f)

#define LINE_X 1058.2f
#define LINE_Y 475.2f

#define BAG_OPEN_HEIGHT 150.0f
#define BAG_CLOSE_HEIGHT 250.0f

#define BACKGROUND_FRONT "GR_S02"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;





Scene* GRScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GRScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

GRScene::GRScene() {
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
GRScene::~GRScene()
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
bool GRScene::init()
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GRScene.plist");
	// ------------------------------------------------------------------------------------------------- 


	////以 Sprite 作為背景 ========================================================================

	_rootNode = CSLoader::createNode(ROOT_NODE);
	addChild(_rootNode);

	Sprite *bkimage = (cocos2d::Sprite *) _rootNode->getChildByName(BACKGROUND_FRONT);
	this->addChild(bkimage, 100);



	//set zoom node====================
	_zNode[0] = (cocos2d::Node*)_rootNode->getChildByName("Node_GR_Z01");
	addChild(_zNode[0], 300);


	_zNode[1] = (cocos2d::Node*)_rootNode->getChildByName("Node_GR_Z02");
	addChild(_zNode[1], 300);

	_zNode[2] = (cocos2d::Node*)_rootNode->getChildByName("Node_GR_Z03");
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

	_player = new CPlayer("aterStand01.png", "aterStand02.png", *this);
	_player->setAnimation("Animation/boyanim.plist");

	_player->setPosition(Point(334, 361));
	_player->setRect();




	//set lightbox (experimental procedure) =================================================================
	_procedure[0] = CLightbox::create();
	_procedure[0]->init(_zNode[1], "GR_Z02_P02_trigger", "GameScene/GRScene/GR_Z02_P02.png");
	this->addChild(_procedure[0], 10000);

	_procedure[1] = CLightbox::create();
	_procedure[1]->init(Rect(587.0f, 761.0f,475.0f,282.0f), "GameScene/GRScene/GR_Z02_P01.png");
	this->addChild(_procedure[1], 10000);

	_procedure[2] = CLightbox::create();
	_procedure[2]->init(_zNode[2], "GR_Z03_01", "GameScene/GRScene/GR_Z03_P01.png");
	this->addChild(_procedure[2], 10000);

	_procedure[3] = CLightbox::create();
	_procedure[3]->init(_zNode[1], "GR_Z02_P03_trigger", "GameScene/GRScene/GR_Z02_P03.png");
	this->addChild(_procedure[3], 10000);


	_pTrigger = new CTrigger[2];

	_pTsudokuRect = new Rect;
	_pTkeyRect = new Rect;

	////set objects=========================================
	SetObject();

	//
	_xmlscene = new xmlScene("./res/xml/xmlfile_GRScene.xml");
	_xmlscene->parseXML(_rootNode, CURRENT_SCENE, _pTrigger);
	_xmlscene->parseNodeXML(_zNode[0], "zN0");
	_xmlscene->parseNodeXML(_zNode[1], "zN1");
	_xmlscene->parseNodeXML(_zNode[2], "zN2");


	//set bag =================================================================


	CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);



	//-------------------------------------------------------------------------------------------------

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(GRScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(GRScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(GRScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(GRScene::doStep));
	return true;
}





void GRScene::SetObject() {
	//set objects

	_pTrigger[0].create();
	_pTrigger[0].Init("GR_Z01_01", _rootNode, true, 1, _zNode[0]); //sudoku trigger

	_pTrigger[1].create();
	_pTrigger[1].Init("GR_Z02_02", _rootNode, true, 1, _zNode[1]); //key trigger

	
	this->addChild(_pTrigger);




	_detect[0] = (cocos2d::Sprite*)_rootNode->getChildByName("GR_Z01_trigger");
	Size size = _detect[0]->getContentSize();
	Point pos = _detect[0]->getPosition();
	_detectRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_detect[1] = (cocos2d::Sprite*)_rootNode->getChildByName("GR_Z02_trigger");
	size = _detect[1]->getContentSize();
	size.width = size.width * 3;
	size.height = size.height*1.2;
	pos = _detect[1]->getPosition();
	_detectRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_detect[2] = (cocos2d::Sprite*)_rootNode->getChildByName("GR_Z03_trigger");
	size = _detect[2]->getContentSize();
	size.width = size.width * 2.5;
	pos = _detect[2]->getPosition();
	_detectRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	// reset button===============================================
	_reset = (cocos2d::Sprite*)_rootNode->getChildByName("reset");
	size = _reset->getContentSize();
	pos = _reset->getPosition();
	_resetRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);


	//set sudoku target area
	auto a = (cocos2d::Sprite*)_zNode[2]->getChildByName("GR_Z03_01");
	size = a->getContentSize();
	pos = a->getPosition();
	_pTsudokuRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//set key target area
	a = (cocos2d::Sprite*)_zNode[1]->getChildByName("GR_Z02_01");
	size = a->getContentSize();
	pos = a->getPosition();
	_pTkeyRect[0] = Rect(pos.x - size.width / 2, pos.y, size.width, size.height/2);

	//set close target area [znode0]
	a = (cocos2d::Sprite*)_zNode[0]->getChildByName("close");
	size = a->getContentSize();
	pos = a->getPosition();
	_closeRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);



}


void GRScene::doStep(float dt){
	
	//walk===================================
	//////只能在設定範圍走=============================
	if (_bWalk && _bwithinArea) { //when player walk within walkable region
		
		if (_player->getPosition().x >= LINE_X-3) {
			if (_TargetLoc.x >= LINE_X) {
				_player->Walk(_TargetLoc);
				_player->go(_TargetLoc);
				if (_player->Walk(_TargetLoc) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
			else {
				_player->Walk(Vec2(_TargetLoc.x, _player->getPreviousPosition().y));
				_player->go(Vec2(_TargetLoc.x, _player->getPreviousPosition().y));
				if (_player->Walk(_TargetLoc) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
		}
		else {
			if (_TargetLoc.x <= LINE_X) {
				_player->Walk(_TargetLoc);
				_player->go(_TargetLoc);
				if (_player->Walk(_TargetLoc) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
			else {
				_player->Walk(Vec2(LINE_X,_TargetLoc.y));
				_player->go(Vec2(LINE_X, _TargetLoc.y));
				if (_player->Walk(_TargetLoc) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
		}



	}
	
	else if (_bWalk) { // when touched obj in scene that can be picked up
		
		// A
		if (_touchLoc.y >  WALK_AREA_1.getMaxY() && _touchLoc.x > WALK_AREA_1.getMinX() && _touchLoc.x < WALK_AREA_2.getMidX()) {
			log("walk A");
			if (_player->getPreviousPosition().x < WALK_AREA_1.getMaxX()) {
				_player->Walk(Vec2(_touchLoc.x, WALK_AREA_1.getMaxY()));
				_player->go(Vec2(_touchLoc.x, WALK_AREA_1.getMaxY()));
				if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_1.getMaxY())) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
			else {
				_player->Walk(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_3.getMaxY()));
				_player->go(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_3.getMaxY()));
				if (_player->Walk(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_3.getMaxY())) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
		}

		// A-1
		else if (_touchLoc.y >  WALK_AREA_1.getMaxY() && _touchLoc.x < WALK_AREA_1.getMinX()) {
			log("walk A-1");

			if (_player->getPreviousPosition().y < WALK_AREA_3.getMidY()) {
				_player->Walk(Vec2(WALK_AREA_1.getMinX(), WALK_AREA_1.getMaxY()));
				_player->go(Vec2(WALK_AREA_1.getMinX(), WALK_AREA_1.getMaxY()));
				if (_player->Walk(Vec2(WALK_AREA_1.getMinX(), WALK_AREA_1.getMaxY())) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
			else  {
				if (_player->getPosition().y-200 >= WALK_AREA_1.getMaxY()+3) {
					
					_player->Walk(Vec2(875.57f, WALK_AREA_1.getMaxY()));
					_player->go(Vec2(875.57f, WALK_AREA_1.getMaxY()));
				}
				else {
					_player->Walk(Vec2(WALK_AREA_1.getMinX(), WALK_AREA_1.getMaxY()));
					_player->go(Vec2(WALK_AREA_1.getMinX(), WALK_AREA_1.getMaxY()));
					if (_player->Walk(Vec2(WALK_AREA_1.getMinX(), WALK_AREA_1.getMaxY())) == false) {
						_bWalk = 0;
						//if (_bonObj) _bpickObj = true; //pick up obj
						_bpickObj = true; //pick up obj
					}
				}
			}	
		}

		// A-2
		else if (_touchLoc.y <  WALK_AREA_1.getMaxY() && _touchLoc.x < WALK_AREA_1.getMinX()) {
			log("walk A-2");

			if (_player->getPreviousPosition().x < WALK_AREA_3.getMinX()) {
				_player->Walk(Vec2(WALK_AREA_1.getMinX(), _touchLoc.y));
				_player->go(Vec2(WALK_AREA_1.getMinX(), _touchLoc.y));
				if (_player->Walk(Vec2(WALK_AREA_1.getMinX(), _touchLoc.y)) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}
			else {
				if (_player->getPosition().x > WALK_AREA_3.getMinX()+3 && _player->getPosition().y-200 > WALK_AREA_2.getMidY() + 3) {
					_player->Walk(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_2.getMidY()));
					_player->go(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_2.getMidY()));

				}
				else {
					_player->Walk(Vec2(WALK_AREA_1.getMinX(), _touchLoc.y));
					_player->go(Vec2(WALK_AREA_1.getMinX(), _touchLoc.y));
					if (_player->Walk(Vec2(WALK_AREA_1.getMinX(), _touchLoc.y)) == false) {
						_bWalk = 0;
						//if (_bonObj) _bpickObj = true; //pick up obj
						_bpickObj = true; //pick up obj
					}
				}
			}
		}

		//B
		else if (_touchLoc.y >  WALK_AREA_3.getMaxY() && _touchLoc.x > WALK_AREA_3.getMinX() && _touchLoc.x < WALK_AREA_3.getMaxX()) {
			log("walk B");
			_player->Walk(Vec2(WALK_AREA_3.getMidX(), WALK_AREA_3.getMidY()));
			_player->go(Vec2(WALK_AREA_3.getMidX(), WALK_AREA_3.getMidY()));
			if (_player->Walk(Vec2(WALK_AREA_3.getMidX(), WALK_AREA_3.getMidY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}

		}

		//C
		else if (_touchLoc.y >  WALK_AREA_4.getMaxY() && _touchLoc.x > WALK_AREA_4.getMinX() && _touchLoc.x < WALK_AREA_4.getMaxX()) {
			log("walk C");
			if (_player->getPreviousPosition().x < WALK_AREA_1.getMaxX()) {
				
				if (_player->getPosition().x < WALK_AREA_3.getMinX() - 3) {
					_player->Walk(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_3.getMinY()));
					_player->go(Vec2(WALK_AREA_3.getMinX(), WALK_AREA_3.getMinY()));
				}

				else {
					_player->Walk(Vec2(WALK_AREA_4.getMidX(), WALK_AREA_4.getMaxY()));
					_player->go(Vec2(WALK_AREA_4.getMidX(), WALK_AREA_4.getMaxY()));
					if (_player->Walk(Vec2(WALK_AREA_4.getMidX(), WALK_AREA_4.getMaxY())) == false) {
						_bWalk = 0;
						//if (_bonObj) _bpickObj = true; //pick up obj
						_bpickObj = true; //pick up obj
					}
				}

				

			}
			else {
				_player->Walk(Vec2(WALK_AREA_4.getMidX(), WALK_AREA_4.getMaxY()));
				_player->go(Vec2(WALK_AREA_4.getMidX(), WALK_AREA_4.getMaxY()));
				if (_player->Walk(Vec2(WALK_AREA_4.getMidX(), WALK_AREA_4.getMaxY())) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}
			}

		}

		//D
		else if (_touchLoc.y >  WALK_AREA_5.getMaxY() && _touchLoc.x > WALK_AREA_4.getMaxX()) {
			log("walk D");

			if (_player->getPreviousPosition().x > WALK_AREA_5.getMinX()) {

				_player->Walk(Vec2(_touchLoc.x, WALK_AREA_5.getMaxY()));
				_player->go(Vec2(_touchLoc.x, WALK_AREA_5.getMaxY()));
				if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_5.getMaxY())) == false) {
					_bWalk = 0;
					//if (_bonObj) _bpickObj = true; //pick up obj
					_bpickObj = true; //pick up obj
				}

			}
			else {


				if (_player->getPosition().y-200 > WALK_AREA_5.getMinY() - 3) {
					_player->Walk(Vec2(_touchLoc.x, WALK_AREA_5.getMaxY()));
					_player->go(Vec2(_touchLoc.x, WALK_AREA_5.getMaxY()));
					if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_5.getMaxY())) == false) {
						_bWalk = 0;
						//if (_bonObj) _bpickObj = true; //pick up obj
						_bpickObj = true; //pick up obj
					}
				}

				else {
					_player->Walk(Vec2(WALK_AREA_3.getMidX(), WALK_AREA_5.getMinY()));
					_player->go(Vec2(WALK_AREA_3.getMidX(), WALK_AREA_5.getMinY()));
					
				}
			}
		}
	}

	else {
		_player->Stop();
	}

	//// pick up obj ==========================
	PickObject(dt);

	
}

void GRScene::PickObject(float dt) {

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

	if (_bopenNode[0]) {

		_pTrigger[0].doStep(dt);

		//fly to bag==========
		if (CBag::getInstance()->Canfly()) {
			//create the corresponding item in bag
			if (_pTrigger[0].GetAddToBag() && !_pTrigger[0].GetPicked()) {
				CBag::getInstance()->AddObj("B_sudoku.png", 1, false, _pTsudokuRect);
				CBag::getInstance()->Fly(_pTrigger[0].GetPos(), "B_sudoku.png");


				_pTrigger[0].SetAddToBag(false);
				_pTrigger[0].SetPicked(true); // if the object is picked and added into the bag

				_xmlscene->editItemState("GR_Z01_01", false, _zNode[0], 0, 1);
			}
			
		}
		
	}



	else if (_bopenNode[1]) {
		_pTrigger[1].doStep(dt);

		if (_bsolve[0]) {
			_procedure[0]->doStep(dt);
			_procedure[3]->doStep(dt);
		}
		 _procedure[1]->doStep(dt);


		 //fly to bag==========
		 if (CBag::getInstance()->Canfly()) {
			 //create the corresponding item in bag
			 if (_pTrigger[1].GetAddToBag() && !_pTrigger[1].GetPicked()) {
				 
				 CBag::getInstance()->AddObj("B_akey.png", 1, false, _pTkeyRect);
				 CBag::getInstance()->Fly(_pTrigger[1].GetPos(), "B_akey.png");
				 _pTrigger[1].SetAddToBag(false);
				 _pTrigger[1].SetPicked(true); // if the object is picked and added into the bag

				 _xmlscene->editItemState("GR_Z02_02", false, _zNode[1], 2, 3);
			 }
		 }

		
		

	}
	else if (_bopenNode[2]) {
		if (_bsolve[1]) _procedure[2]->doStep(dt);
	}



}


void GRScene::reset() {
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
	_procedure[3]->setVisible(false);

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


bool GRScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
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


void  GRScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{

	_touchLoc = pTouch->getLocation();

	//use items in bag===========================================
	if (_ibagState) { //when bag is open
		CBag::getInstance()->touchesMoved(_touchLoc);
	}
	
	
}

void  GRScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
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
					WALK_AREA_4.containsPoint(_touchLoc) || WALK_AREA_5.containsPoint(_touchLoc)) {
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
					_pTrigger[0].touchesBegan(_touchLoc);

					if (_closeRect.containsPoint(_touchLoc)) {
						_bopenNode[0] = !_bopenNode[0];
						_zNode[0]->setVisible(false);
						log("close detect");
					}

				}
				//znode[1]開---------------------
				else if (_bopenNode[1]) {
					_bwithinArea = false;
					_pTrigger[1].touchesBegan(_touchLoc);

					if (!_procedure[0]->GetOpen() && !_procedure[1]->GetOpen() && !_procedure[3]->GetOpen()) {
						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[1] = !_bopenNode[1];
							_zNode[1]->setVisible(false);
							log("close detect");
						}
					}
					if (_bsolve[0]) {
						_procedure[0]->TouchBegan(_touchLoc);
						_procedure[3]->TouchBegan(_touchLoc);
					}
					_procedure[1]->TouchBegan(_touchLoc);

					
				}
				//znode[2]開---------------------
				else if (_bopenNode[2]) {
					_bwithinArea = false;
					if (!_procedure[2]->GetOpen()) {
						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[2] = !_bopenNode[2];
							_zNode[2]->setVisible(false);
							log("close detect");
						}
					}

					if (_bsolve[1])_procedure[2]->TouchBegan(_touchLoc);

					
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
				if (_ibagState == 0) {

					//walk area ====================================
					if (WALK_AREA_1.containsPoint(_touchLoc) || WALK_AREA_2.containsPoint(_touchLoc) || WALK_AREA_3.containsPoint(_touchLoc) ||
						WALK_AREA_4.containsPoint(_touchLoc) || WALK_AREA_5.containsPoint(_touchLoc)) {
						// detect if touch pts are in walkable area
						_bwithinArea = true;
						log("walk!!");

					}
					else  _bwithinArea = false;

					////player walk =====================================================

					//放大鏡沒開 --------------
					if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {

						_bWalk = 1;
						_player->setPreviousPosition();

						//-------------------------------
						_TargetLoc = _touchLoc;

						//====================================

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
		if (i >= 0) {
			// mix mix

			// add sound

			if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_sudoku.png")) {
				// add debranch
				//_grinding->playEffect();
				_xmlscene->editItemState("GR_Z03_01", true, _zNode[2], 0, 4);
				_xmlscene->editItemState("GR_S01_01", true, _rootNode, 0, 4);
				_bsolve[1] = true;

			}

			else if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_akey.png")) {
				// add debranch
				//_grinding->playEffect();
				_xmlscene->editItemState("GR_Z02_01", true, _zNode[1], 2, 3);
				_bsolve[0] = true;

			}



		}
	}

}
