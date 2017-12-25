#include "labScene.h"
#include "cocostudio/CocoStudio.h"

#include "C2_Scene_02.h"

#define ROOT_NODE   "labScene.csb"
#define CURRENT_SCENE   "labScene.cpp"

#define WALK_AREA_1 Rect(0.0f, 0.0f, 724.0f, 172.8f)
#define WALK_AREA_2 Rect(0.0f, 174.88f, 1688.64f, 151.32f)
#define WALK_AREA_3 Rect(799.66f, 326.69f, 1009.14f, 118.8f)
#define WALK_AREA_4 Rect(1151.10f, 450.15f, 384.37f, 102.80f)

#define LINE_Y 170

#define BAG_OPEN_HEIGHT 150.0f
#define BAG_CLOSE_HEIGHT 250.0f

#define BACKGROUND_FRONT "lab_S02"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;




Scene* labScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = labScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

labScene::labScene() {
	_bWalk = false; //detect if player is walking

	//判斷 walk
	_bwithinArea = false;

	_bpickObj = false;
	
	_bmicroscope = false;
	_bsolve[0] =false;
	_bsolve[1] = false;
	_clear = false;
	for (size_t i = 0; i < 5; i++){
		_btouch[i] = false;
	}
	_ibagState = 0;

	for (size_t i = 0; i < TALK_AREA; i++) {
		_touchTalk[i] = false;
		_openTalk[i] = false;
	}
	_touchOut = false;
	_openOut = false;

}
labScene::~labScene()
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
}

// on "init" you need to initialize your instance
bool labScene::init()
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("labScene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/bagItem.plist");
	// ------------------------------------------------------------------------------------------------- 


	////以 Sprite 作為背景 ========================================================================

	_rootNode = CSLoader::createNode(ROOT_NODE);
	addChild(_rootNode);

	Sprite *bkimage = (cocos2d::Sprite *) _rootNode->getChildByName(BACKGROUND_FRONT);
	this->addChild(bkimage, 100);

	_microscope[0] = Sprite::create("GameScene/labScene/lab_E01.png");
	_microscope[0]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_microscope[0]->setVisible(false);
	this->addChild(_microscope[0], 1001);

	_microscope[1] = Sprite::create("GameScene/labScene/lab_E02.png");
	_microscope[1]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_microscope[1]->setVisible(false);
	this->addChild(_microscope[1], 1001);

	_microscope[2] = Sprite::create("GameScene/labScene/lab_E03.png");
	_microscope[2]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_microscope[2]->setVisible(false);
	this->addChild(_microscope[2], 1001);

	_microscope[3] = Sprite::create("GameScene/labScene/lab_E04.png");
	_microscope[3]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_microscope[3]->setVisible(false);
	this->addChild(_microscope[3], 1001);

	_microscope[4] = Sprite::create("GameScene/labScene/lab_E05.png");
	_microscope[4]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_microscope[4]->setVisible(false);
	this->addChild(_microscope[4], 1001);






	//set zoom node====================
	_zNode[0] = (cocos2d::Node*)_rootNode->getChildByName("Node_lab_Z01");
	addChild(_zNode[0], 300);


	_zNode[1] = (cocos2d::Node*)_rootNode->getChildByName("Node_lab_Z02");
	addChild(_zNode[1], 300);

	_zNode[2] = (cocos2d::Node*)_rootNode->getChildByName("Node_lab_Z03");
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

	_player->setPosition(Point(239.82f, 357.38f));
	_player->setRect();
	_player->SetFront(false);
	_player->Mirror(true);



	//set lightbox (experimental procedure) =================================================================
	//_procedure[0] = CLightbox::create();
	//_procedure[0]->init(_zNode[1], "GR_P01_trigger", "GameScene/labScene/GR_P01.png");
	//this->addChild(_procedure[0], 10000);

	//_procedure[1] = CLightbox::create();
	//_procedure[1]->init(Rect(587.0f, 761.0f,475.0f,282.0f), "GameScene/labScene/GR_P02.png");
	//this->addChild(_procedure[1], 10000);

	//_procedure[2] = CLightbox::create();
	//_procedure[2]->init(_zNode[2], "GR_Z03_01", "GameScene/labScene/GR_P03.png");
	//this->addChild(_procedure[2], 10000);


	_pTrigger = new CTrigger[6];

	_pTslidesRect = new Rect;

	////set objects=========================================
	SetObject();

	
	_xmlscene = new xmlScene("./res/xml/xmlfile_labScene.xml");
	_xmlscene->parseXML(_rootNode, CURRENT_SCENE, _pTrigger);
	_xmlscene->parseNodeXML(_zNode[0], "zN0");
	//_xmlscene->parseNodeXML(_zNode[1], "zN1");
	//_xmlscene->parseNodeXML(_zNode[2], "zN2");


	//set bag =================================================================


	//CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);



	//-------------------------------------------------------------------------------------------------

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(labScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(labScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(labScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(labScene::doStep));
	return true;
}





void labScene::SetObject() {
	//set objects

	_pTrigger[0].create();
	_pTrigger[0].Init("lab_01_01", _rootNode, true); // iodine trigger


	_pTrigger[1].create();
	_pTrigger[1].Init("lab_Z01_01", _rootNode, true, 1, _zNode[0]); //DGslides trigger

	_pTrigger[2].create();
	_pTrigger[2].Init("lab_Z01_02", _rootNode, true, 1, _zNode[0]); //Bslides trigger

	_pTrigger[3].create();
	_pTrigger[3].Init("lab_Z01_03", _rootNode, true, 1, _zNode[0]); //dropper trigger

	_pTrigger[4].create();
	_pTrigger[4].Init("lab_Z01_04", _rootNode, true, 1, _zNode[0]); //Gslides trigger

	_pTrigger[5].create();
	_pTrigger[5].Init("lab_Z01_05", _rootNode, true, 1, _zNode[0]); //Pslides trigger

	this->addChild(_pTrigger);




	_detect[0] = (cocos2d::Sprite*)_rootNode->getChildByName("lab_Z01_trigger");
	Size size = _detect[0]->getContentSize();
	size.width = size.width * 3;
	Point pos = _detect[0]->getPosition();
	_detectRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//_detect[1] = (cocos2d::Sprite*)_rootNode->getChildByName("lab_Z02_trigger");
	//size = _detect[1]->getContentSize();
	//size.width = size.width * 0.9;
	//size.height = size.height * 0.6;
	//pos = _detect[1]->getPosition();
	//_detectRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//_detect[2] = (cocos2d::Sprite*)_rootNode->getChildByName("lab_Z03_trigger");
	//size = _detect[2]->getContentSize();
	//size.width = size.width * 3;
	//size.height = size.height * 2;
	//pos = _detect[2]->getPosition();
	//_detectRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	// reset button===============================================
	_reset = (cocos2d::Sprite*)_rootNode->getChildByName("reset");
	size = _reset->getContentSize();
	pos = _reset->getPosition();
	_resetRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);


	////set sudoku target area
	auto a = (cocos2d::Sprite*)_rootNode->getChildByName("target_slides");
	size = a->getContentSize();
	pos = a->getPosition();
	_pTslidesRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width * 1.5, size.height * 2);

	////set key target area
	//a = (cocos2d::Sprite*)_zNode[1]->getChildByName("GR_Z02_01");
	//size = a->getContentSize();
	//pos = a->getPosition();
	//_pTkeyRect[0] = Rect(pos.x - size.width / 2, pos.y, size.width, size.height/2);

	//set close target area [znode]
	a = (cocos2d::Sprite*)_zNode[0]->getChildByName("close");
	size = a->getContentSize();
	pos = a->getPosition();
	_closeRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);



	//set talk area===============================
	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_computer_0");
	size = a->getContentSize();
	pos = a->getPosition();
	size.width = size.width*1.33f;
	size.height = size.height *0.6f;
	_talkRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_plants_1");
	pos = a->getPosition();
	size.width = size.width*1.94f;
	size.height = size.height *0.9f;
	_talkRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	a = (cocos2d::Sprite*)_rootNode->getChildByName("talk_table_2");
	pos = a->getPosition();
	size = a->getContentSize();
	size.width = size.width*1.62f;
	size.height = size.height *1.28f;
	_talkRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);




	//set talk area===============================
	a = (cocos2d::Sprite*)_rootNode->getChildByName("exit");
	size = a->getContentSize();
	pos = a->getPosition();
	size.width = size.width*2.0f;
	size.height = size.height *0.8f;
	_outRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);



}


void labScene::doStep(float dt){
	
	//walk===================================
	//open node=========
	if (_btouchNode[0] || _touchSObj || _touchTalk[0]) {
		_player->Walk(Vec2(908.58f, 368.85f));
		_player->go(Vec2(908.58f, 368.85f));

		if (_player->Walk(Vec2(908.58f, 368.85f)) == false) {
			_bWalk = 0;
			
			//pick up obj


			if (_btouchNode[0]) {

				_zNode[0]->setVisible(true);

				_bopenNode[0] = !_bopenNode[0];
				_btouchNode[0] = !_btouchNode[0];
			}
			else if (_touchTalk[0]) {
				_touchTalk[0] = !_touchTalk[0];
				_openTalk[0] = !_openTalk[0];
			}
			else {
				_touchSObj = !_touchSObj;
				_openSObj = !_openSObj;
			}

			//log("show detect");
		}


	}

	else if (_touchTalk[1]) {
		_player->Walk(Vec2(1501.90f, 177.98f));
		_player->go(Vec2(1501.90f, 177.98f));
		if (_player->Walk(Vec2(1501.90f, 177.98f)) == false) {
			_bWalk = 0;

			//pick up obj

			_touchTalk[1] = !_touchTalk[1];
			_openTalk[1] = !_openTalk[1];



			//log("show detect");
		}

	}

	else if (_touchTalk[2]) {
		_player->Walk(Vec2(1183.30f, 143.35f));
		_player->go(Vec2(1183.30f, 143.35f));
		if (_player->Walk(Vec2(1183.30f, 143.35f)) == false) {
			_bWalk = 0;

			//pick up obj

			_touchTalk[2] = !_touchTalk[2];
			_openTalk[2] = !_openTalk[2];

			//log("show detect");
		}

	}
	
	else if (_touchOut) {
		_player->Walk(Vec2(-9.19f, 45.06f));
		_player->go(Vec2(-9.19f, 45.06f));
		if (_player->Walk(Vec2(-9.19f, 45.06f)) == false) {
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

	
}

void labScene::PickObject(float dt) {

	// pick up obj ==========================
	

	if (_openSObj) {
		_player->Mirror(false);
		_player->SetFront(false);
		_pTrigger[0].doStep(dt);

		//fly to bag==========
		if (CBag::getInstance()->Canfly()) {
			//create the corresponding item in bag
			if (_pTrigger[0].GetAddToBag() && !_pTrigger[0].GetPicked()) {

				CBag::getInstance()->AddObj("B_lodine.png");
				CBag::getInstance()->Fly(_pTrigger[0].GetPos(), "B_lodine.png");
				_pTrigger[0].SetAddToBag(false);
				_pTrigger[0].SetPicked(true);

			}
		}
		_openSObj = false;
	}

	else if (_bopenNode[0]) {
		_player->Mirror(false);
		_player->SetFront(false);
		_pTrigger[1].doStep(dt);
		_pTrigger[2].doStep(dt);
		_pTrigger[3].doStep(dt);
		_pTrigger[4].doStep(dt);
		_pTrigger[5].doStep(dt);


		//fly to bag==========
		if (CBag::getInstance()->Canfly()) {
			//create the corresponding item in bag
			if (_pTrigger[1].GetAddToBag() && !_pTrigger[1].GetPicked()) {

				CBag::getInstance()->AddObj("B_DGslides.png", 1, true, _pTslidesRect);
				CBag::getInstance()->Fly(_pTrigger[1].GetPos(), "B_DGslides.png");

				_pTrigger[1].SetAddToBag(false);
				_pTrigger[1].SetPicked(true); // if the object is picked and added into the bag

				_xmlscene->editItemState("lab_Z01_01", false, _zNode[0], 1, 5);
			}

			else if (_pTrigger[2].GetAddToBag() && !_pTrigger[2].GetPicked()) {

				CBag::getInstance()->AddObj("B_Bslides.png", 1, true, _pTslidesRect);
				CBag::getInstance()->Fly(_pTrigger[2].GetPos(), "B_Bslides.png");

				_pTrigger[2].SetAddToBag(false);
				_pTrigger[2].SetPicked(true); // if the object is picked and added into the bag

				_xmlscene->editItemState("lab_Z01_02", false, _zNode[0], 1, 5);
			}

			else if (_pTrigger[3].GetAddToBag() && !_pTrigger[3].GetPicked()) {

				CBag::getInstance()->AddObj("B_dropper.png");
				CBag::getInstance()->Fly(_pTrigger[3].GetPos(), "B_dropper.png");

				_pTrigger[3].SetAddToBag(false);
				_pTrigger[3].SetPicked(true); // if the object is picked and added into the bag

				_xmlscene->editItemState("lab_Z01_03", false, _zNode[0], 1, 5);
			}

			else if (_pTrigger[4].GetAddToBag() && !_pTrigger[4].GetPicked()) {

				CBag::getInstance()->AddObj("B_Gslides.png", 1, true, _pTslidesRect);
				CBag::getInstance()->Fly(_pTrigger[4].GetPos(), "B_Gslides.png");

				_pTrigger[4].SetAddToBag(false);
				_pTrigger[4].SetPicked(true); // if the object is picked and added into the bag

				_xmlscene->editItemState("lab_Z01_04", false, _zNode[0], 1, 5);
			}

			else if (_pTrigger[5].GetAddToBag() && !_pTrigger[5].GetPicked()) {

				CBag::getInstance()->AddObj("B_Pslides.png", 1, true, _pTslidesRect);
				CBag::getInstance()->Fly(_pTrigger[5].GetPos(), "B_Pslides.png");

				_pTrigger[5].SetAddToBag(false);
				_pTrigger[5].SetPicked(true); // if the object is picked and added into the bag

				_xmlscene->editItemState("lab_Z01_05", false, _zNode[0], 1, 5);
			}
		}



		
		
	}

	else if (_openTalk[0]) {
		_player->SetFront(false);
		_player->Mirror(true);
		const char* x;
		if (_talkContent) x = "dialoge/lab/L2_computer01.png";
		else x = "dialoge/lab/L2_computer02.png";
		_player->Talk(x, true);
		_player->SetIsTalking(true);
		_openTalk[0] = false;
	}

	else if (_openTalk[1]) {
		_player->Mirror(true);
		_player->SetFront(true);
		const char* x;
		if (_talkContent) x = "dialoge/lab/L2_plants01.png";
		else x = "dialoge/lab/L2_plants02.png";
		_player->Talk(x, false);
		_player->SetIsTalking(true);
		_openTalk[1] = false;
	}

	else if (_openTalk[2]) {
		_player->SetFront(true);
		_player->Mirror(false);
		const char* x;
		if (_talkContent) x = "dialoge/lab/L2_table01.png";
		else x = "dialoge/lab/L2_table02.png";
		_player->Talk(x, true);
		_player->SetIsTalking(true);
		_openTalk[2] = false;
	}
	else if (_openOut) {
		log("to corridor");
		_openOut = !_openOut;
		// to corridor================================
		this->unschedule(schedule_selector(labScene::doStep));
		C2_Scene_02::_from = 0;
		Director::getInstance()->replaceScene(C2_Scene_02::createScene());
	}
	

/*
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


void labScene::reset() {
	CBag::getInstance()->reset();

	// reset scene===============================
	_bWalk = false; //detect if player is walking

	_bpickObj = false;
	
	_zNode[0]->setVisible(false);
	//_zNode[1]->setVisible(false);
	//_zNode[2]->setVisible(false);

	//_procedure[0]->setVisible(false);
	//_procedure[1]->setVisible(false);
	//_procedure[2]->setVisible(false);

	_xmlscene->editItemState(0, true, _rootNode);
	_xmlscene->editItemState(1, true, _zNode[0]);
	_xmlscene->editItemState(2, true, _zNode[0]);
	_xmlscene->editItemState(3, true, _zNode[0]);
	_xmlscene->editItemState(4, true, _zNode[0]);
	_xmlscene->editItemState(5, true, _zNode[0]);

	_pTrigger[0].reset();
	_pTrigger[1].reset();
	_pTrigger[2].reset();
	_pTrigger[3].reset();
	_pTrigger[4].reset();
	_pTrigger[5].reset();

	xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);

	_bmicroscope = false;
	//_bsolve[0] = false;
	//_bsolve[1] = false;
	_touchOut = false;
	_openOut = false;
}


bool labScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
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
	}
	

	return true;
}


void  labScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{

	_touchLoc = pTouch->getLocation();
	if (!_player->GetIsTalking()) {
		//use items in bag===========================================
		if (_ibagState) { //when bag is open
			CBag::getInstance()->touchesMoved(_touchLoc);
		}
	}
	
	
	
}

void  labScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{

	_touchLoc = pTouch->getLocation();

	//swipe gesture
	float offsetX = _touchLoc.x - _startX;
	float offsetY = _touchLoc.y - _startY;

	if (!_player->GetIsTalking()) {
		if (_ibagState != 2) {

			// [WALK + PICK OBJECT]===================
			if (offsetX == 0 && offsetY == 0 && !CBag::getInstance()->LightboxState() && !_bmicroscope) { // when screen tapped
				if (_touchLoc.y > 227) {

					////player walk =====================================================

					//放大鏡沒開 --------------
					if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {
						//沒按重置-------------
						if (!_resetRect.containsPoint(_touchLoc)) {


							//====================================
							//detect for [iodine]------
							if (_pTrigger[0].touchesBegan(_touchLoc)) {
								_touchSObj = true;
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
							}

							if (_talkRect[0].containsPoint(_touchLoc)) {
								_touchTalk[0] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 2;
							}
							else {
								_openTalk[0] = false;
								_touchTalk[0] = false;
							}


							if (_talkRect[1].containsPoint(_touchLoc)) {
								_touchTalk[1] = true;
								_bWalk = 1;
								_player->setPreviousPosition();
								_talkContent = rand() % 2;
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

						}
						else {
							// reset button=========================
							reset();
						}



					}




					//znode[0]開---------------------
					if (_bopenNode[0]) {
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
					//znode[1]開---------------------
					else if (_bopenNode[1]) {
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
						//if (_bsolve[1])_procedure[2]->TouchBegan(_touchLoc);

						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[2] = !_bopenNode[2];
							_zNode[2]->setVisible(false);
							log("close detect");
						}
					}

					if (!_bWalk) {
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
						////1
						//if (!_bopenNode[1] && _detectRect[1].containsPoint(_touchLoc)) {
						//	_btouchNode[1] = true;
						//	log("touched detect node2");
						//}
						//else if (!_bopenNode[1] && !_detectRect[1].containsPoint(_touchLoc)) {
						//	_btouchNode[1] = false;
						//}
						////2
						//if (!_bopenNode[2] && _detectRect[2].containsPoint(_touchLoc)) {
						//	_btouchNode[2] = true;
						//	log("touched detect node2");
						//}
						//else if (!_bopenNode[2] && !_detectRect[2].containsPoint(_touchLoc)) {
						//	_btouchNode[2] = false;
						//}
					}




				}
				else {
					if (!_ibagState) {
						//walk area ====================================

						////player walk =====================================================

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

						//znode[0]開---------------------
						if (_bopenNode[0]) {
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

					}
				}

			}

			else if (_bmicroscope && !CBag::getInstance()->LightboxState()) {
				if (_closeRect.containsPoint(_touchLoc)) {
					_bmicroscope = !_bmicroscope;
					for (size_t i = 0; i < 5; i++) {
						_microscope[i]->setVisible(false);
					}

					log("close detect");
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

				if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_Bslides.png") || !strcmp(xmlBag::getInstance()->getItemName(i), "B_DGslides.png") || !strcmp(xmlBag::getInstance()->getItemName(i), "B_Gslides.png") || !strcmp(xmlBag::getInstance()->getItemName(i), "B_Pslides.png")) {
					// add debranch
					//_grinding->playEffect();
					_microscope[0]->setVisible(true);

					_bmicroscope = true;

				}

				else if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_BslidesLOD.png")) {
					// add debranch
					//_grinding->playEffect();
					_microscope[4]->setVisible(true);
					_bmicroscope = true;

				}

				else if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_DGslidesLOD.png")) {
					// add debranch
					//_grinding->playEffect();
					_microscope[2]->setVisible(true);
					_bmicroscope = true;

				}

				else if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_GslidesLOD.png")) {
					// add debranch
					//_grinding->playEffect();
					_microscope[3]->setVisible(true);
					_bmicroscope = true;

				}

				else if (!strcmp(xmlBag::getInstance()->getItemName(i), "B_PslidesLOD.png")) {
					// add debranch
					//_grinding->playEffect();
					_microscope[1]->setVisible(true);
					_bmicroscope = true;

				}



			}
		}
	}

	else {
		_player->SetIsTalking(false);
		_player->StopTalking();
	}

}
