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

	//�P�_ walk
	_bwithinArea = false;

	_bpickObj = false;
	
	_bsolve[0] =false;
	_bsolve[1] = false;
	_clear = false;
	for (size_t i = 0; i < 5; i++){
		_btouch[i] = false;
	}
	_ibagState = 0;

	_btogger[0] = false;
	_btogger[1] = false;


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

	//// �Q�ε{���������ͧǦC�V�ʵe 
	//// STEP 1 : Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SRScene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/bagItem.plist");
	// ------------------------------------------------------------------------------------------------- 


	////�H Sprite �@���I�� ========================================================================

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
	
	// ���ĻP���� --------------------------------------------------------------------------------

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/GR_bgm.mp3", true);
	//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //�|����@

	////SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������

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


	CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);



	//-------------------------------------------------------------------------------------------------

	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(SRScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(SRScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(SRScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
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

	_detect[3] = (cocos2d::Sprite*)_rootNode->getChildByName("SR_Z04_trigger");
	size = _detect[3]->getContentSize();
	size.width = size.width * 3;
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



}


void SRScene::doStep(float dt){
	
	//walk===================================
	//////�u��b�]�w�d��=============================
	/*if (_bWalk && _bwithinArea) { //when player walk within walkable region
		
		_player->Walk(_TargetLoc);
		_player->go(_TargetLoc);
		if (_player->Walk(_TargetLoc) == false) {
			_bWalk = 0;
			//if (_bonObj) _bpickObj = true; //pick up obj
			_bpickObj = true; //pick up obj
		}

	}
	
	else*/ if (_bWalk) { // when touched obj in scene that can be picked up
		
		// A
		if (_touchLoc.y >  WALK_AREA_2.getMinY() && _touchLoc.y < LINE_Y && _touchLoc.x < WALK_AREA_2.getMinX()) {
			log("walk A");
			_player->Walk(Vec2(WALK_AREA_2.getMinX(), _touchLoc.y));
			_player->go(Vec2(WALK_AREA_2.getMinX(), _touchLoc.y));
			if (_player->Walk(Vec2(WALK_AREA_2.getMinX(), _touchLoc.y)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}
		// A-1
		else if (_touchLoc.y >  WALK_AREA_2.getMaxY() && _touchLoc.x < WALK_AREA_2.getMinX()) {
			log("walk A-1");
			_player->Walk(Vec2(WALK_AREA_2.getMinX()+100, WALK_AREA_2.getMaxY()-100));
			_player->go(Vec2(WALK_AREA_2.getMinX()+100, WALK_AREA_2.getMaxY()- 100));
			if (_player->Walk(Vec2(WALK_AREA_2.getMinX()+ 100, WALK_AREA_2.getMaxY()- 100)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}

		// A-2
		else if (_touchLoc.y <  WALK_AREA_1.getMaxY() && _touchLoc.x < WALK_AREA_2.getMinX()) {
			log("walk A-2");
			_player->Walk(Vec2(WALK_AREA_2.getMinX(), WALK_AREA_2.getMinY()));
			_player->go(Vec2(WALK_AREA_2.getMinX(), WALK_AREA_2.getMinY()));
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
			_player->go(Vec2(_touchLoc.x, WALK_AREA_2.getMinY()));
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMinY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}


		}


		// B-1
		else if (_touchLoc.y > LINE_Y  && _touchLoc.x > WALK_AREA_2.getMinX() && _touchLoc.x < WALK_AREA_3.getMinX()) {
			log("walk B-1");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY()- 100));
			_player->go(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY()- 100));
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY()- 100)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}


		// B-2
		else if (_touchLoc.y > LINE_Y  && _touchLoc.x > WALK_AREA_3.getMinX() && _touchLoc.x < WALK_AREA_4.getMinX()) {
			log("walk B-2");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY()- 100));
			_player->go(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY() - 100));
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY() - 100)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}


		// B-3
		else if (_touchLoc.y > LINE_Y  && _touchLoc.x > WALK_AREA_4.getMinX() && _touchLoc.x < 958.41f) {
			log("walk B-3");

			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY() - 100));
			_player->go(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY() - 100));
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY() - 100)) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}
		}

		// C-1
		else if (_touchLoc.y > LINE_Y && _touchLoc.x > 958.41f && _touchLoc.x < WALK_AREA_4.getMaxX()) {
			log("walk C-1");
			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY()));
			_player->go(Vec2(_touchLoc.x, WALK_AREA_4.getMaxY()));
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
			_player->go(Vec2(_touchLoc.x, WALK_AREA_3.getMaxY()));
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
			_player->go(Vec2(_touchLoc.x, WALK_AREA_2.getMaxY()));
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
			_player->go(Vec2(WALK_AREA_2.getMaxX(), WALK_AREA_2.getMaxY()));
			if (_player->Walk(Vec2(WALK_AREA_2.getMaxX(), WALK_AREA_2.getMaxY())) == false) {
				_bWalk = 0;
				//if (_bonObj) _bpickObj = true; //pick up obj
				_bpickObj = true; //pick up obj
			}

		}



		// D
		else if (_touchLoc.y < WALK_AREA_2.getMaxY() - 100 && _touchLoc.x >  WALK_AREA_1.getMaxX()) {
			log("walk D");
			_player->Walk(Vec2(WALK_AREA_2.getMaxX() - 100, _touchLoc.y));
			_player->go(Vec2(WALK_AREA_2.getMaxX() - 100, _touchLoc.y));
			if (_player->Walk(Vec2(WALK_AREA_2.getMaxX() - 100, _touchLoc.y)) == false) {
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
	_lock->doStep(dt);
	
	if (_lock->GetSolved()) _zNode[2]->getChildByName("SR_Z03_E02")->setVisible(true);
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

		else if (_btouchNode[3]) {
			_zNode[3]->setVisible(true);

			_bopenNode[3] = !_bopenNode[3];
			_btouchNode[3] = !_btouchNode[3];
			//log("show detect node3");

		}

		_bpickObj = false;

	}

	if (_bopenNode[0]) {

		_procedure[0]->doStep(dt);
		
	}



	else if (_bopenNode[1]) {
		_procedure[1]->doStep(dt);
		

	}
	else if (_bopenNode[3]) {
		if (_btogger[1]) _procedure[2]->doStep(dt);
	}

	

}


void SRScene::reset() {
	CBag::getInstance()->reset();

	// reset scene===============================
	_bWalk = false; //detect if player is walking
	_bwithinArea = false;//�P�_ walk

	_bpickObj = false;
	
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

}


bool SRScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
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

	if (_btogger[0]) {
		//lock==================
		_lock->TouchBegan(_touchLoc);
	}



	return true;
}


void  SRScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{

	_touchLoc = pTouch->getLocation();

	//use items in bag===========================================
	if (_ibagState) { //when bag is open
		CBag::getInstance()->touchesMoved(_touchLoc);
	}
	
	if (_btogger[0]) {
		//lock==================
		_lock->TouchMoved(_touchLoc);
	}
}

void  SRScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
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

				//��j��S�} --------------
				if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2] && !_bopenNode[3]) {
					//�S�����m-------------
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




				//znode[0]�}---------------------
				if (_bopenNode[0]) {
					_bwithinArea = false;

					if (!_procedure[0]->GetOpen()) {
						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[0] = !_bopenNode[0];
							_zNode[0]->setVisible(false);
							log("close detect");
						}
					}
					
					_procedure[0]->TouchBegan(_touchLoc);

				}
				//znode[1]�}---------------------
				else if (_bopenNode[1]) {
					_bwithinArea = false;
					
					if (!_procedure[1]->GetOpen()) {
						if (_closeRect.containsPoint(_touchLoc)) {
							_bopenNode[1] = !_bopenNode[1];
							_zNode[1]->setVisible(false);
							log("close detect");
						}
					}

					_procedure[1]->TouchBegan(_touchLoc);
					
				}
				//znode[2]�}---------------------
				else if (_bopenNode[2] && !_lock->GetState()) {
					_bwithinArea = false;
					if(_btogger[0]) _lock->TouchEnded(_touchLoc);
					else {
						if (_toggerRect[0].containsPoint(_touchLoc)){
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

				//znode[3]�}---------------------
				else if (_bopenNode[3]) {
					_bwithinArea = false;
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

				//3
				if (!_bopenNode[3] && _detectRect[3].containsPoint(_touchLoc)) {
					_btouchNode[3] = true;
					log("touched detect node2");
				}
				else if (!_bopenNode[3] && !_detectRect[3].containsPoint(_touchLoc)) {
					_btouchNode[3] = false;
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

					//��j��S�} --------------
					if (!_bopenNode[0] && !_bopenNode[1] && !_bopenNode[2]) {
						//�S�����m-------------

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

					//znode[0]�}---------------------
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