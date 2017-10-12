#include "labScene.h"
#include "cocostudio/CocoStudio.h"

#define ROOT_NODE   "labScene.csb"
#define CURRENT_SCENE   "labScene.cpp"

#define WALK_AREA_1 Vec2(420.0f,663.0f)
#define WALK_AREA_2 Vec2(1657.0f,663.0f)
#define WALK_AREA_3 Vec2(1657.0f,175.0f)
#define WALK_AREA_4 Vec2(420.0f,175.0f)

#define BAG_OPEN_HEIGHT 150.0f
#define BAG_CLOSE_HEIGHT 440.0f

#define BACKGROUND_FRONT "GameScene/labScene/labScene02.png"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;


Scene* labScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	//auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	// 'layer' is an autorelease object
	auto layer = labScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

labScene::labScene() {
	_bWalk = false; //detect if player is walking
	_bWAbled = true;

	//判斷 walk
	_bwithinArea = false;
	_bonObj = false;

	_bpickObj = false;
	_bpickSyrup[4] = false;
	_bpickWater = false;
	_bpickRod = false;



	_buseSyrupA[4] = false;
	_buseSyrupB[4] = false;

	
	_bgetOSyrup = false;
	_bgetPSyrup = false;
	_bgetBSyrup[2] = false;
	_bpourIntoA = false;
	_bpourIntoB = false;



}
labScene::~labScene()
{
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
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SceneKitchenppl.plist");

	// ------------------------------------------------------------------------------------------------- 


	////以 Sprite 作為背景
	Sprite *bkimage = Sprite::create(BACKGROUND_FRONT);  // 使用 create 函式,給予檔名即可
	bkimage->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2));
	this->addChild(bkimage, 1000);

	_rootNode = CSLoader::createNode(ROOT_NODE);
	addChild(_rootNode);


	//set objects
	SetObject();



	// set player==============================================================

	_player = new CPlayer("boy.png", "boywalk001.png", *this);
	_player->setAnimation("playeranim.plist");

	//_player->setPosition(Point(950, 320));
	_player->setRect();



	//set bag =================================================================
	_bag = CBag::create();
	_bag->Init("trashcan.png", Point(350, 50));
	this->addChild(_bag,2000);
	SetItem();


	//set lightbox =================================================================
	_procedure = CLightbox::create();
	_procedure->init(_rootNode, "detect_procedure", "procedure.png");
	this->addChild(_procedure, 10000);


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

	_syrup[0] = CTrigger::create();
	_syrup[0]->Init("yellow", _rootNode,true);
	this->addChild(_syrup[0]);

	_syrup[1] = CTrigger::create();
	_syrup[1]->Init("red", _rootNode, true);
	this->addChild(_syrup[1]);

	_syrup[2] = CTrigger::create();
	_syrup[2]->Init("blue", _rootNode, true);
	this->addChild(_syrup[2]);

	_syrup[3] = CTrigger::create();
	_syrup[3]->Init("green", _rootNode, true);
	this->addChild(_syrup[3]);

	_water = CTrigger::create();
	_water->Init("water", _rootNode, true);
	this->addChild(_water);

	_glassrod = CTrigger::create();
	_glassrod->Init("glassrod", _rootNode, true);
	this->addChild(_glassrod);


	_herbRect[0] = CTrigger::create();
	_herbRect[0]->Init("detect_herbG", _rootNode, false);
	this->addChild(_herbRect[0]);

	_herbRect[1] = CTrigger::create();
	_herbRect[1]->Init("detect_herbR", _rootNode, false);
	this->addChild(_herbRect[1]);

	_herbRect[2] = CTrigger::create();
	_herbRect[2]->Init("detect_herbDG", _rootNode, false);
	this->addChild(_herbRect[2]);

	_herbRect[3] = CTrigger::create();
	_herbRect[3]->Init("detect_herbY", _rootNode, false);
	this->addChild(_herbRect[3]);





	_alcoholLamp[0] = (cocos2d::Sprite*)_rootNode->getChildByName("alcohol_lamp01");
	_alcoholLamp[1] = (cocos2d::Sprite*)_rootNode->getChildByName("alcohol_lamp02");
	Size size = _alcoholLamp[1]->getContentSize();
	Point pos = _alcoholLamp[1]->getPosition();
	_alcoholLampRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	
	_beakerA[0] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_yellow");

	_beakerA[1] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_red");
	_beakerA[2] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_blue");
	_beakerA[3] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_green");
	_beakerA[4] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_orange");
	_beakerA[5] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_purple");
	_beakerA[6] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_brownSh");
	_beakerA[7] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_false");
	_beakerA[8] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker");
	size = _beakerA[8]->getContentSize();
	pos = _beakerA[8]->getPosition();
	_beakerARect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
	
	_beakerB[0] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_yellow01");
	_beakerB[1] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_red01");
	_beakerB[2] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_blue01");
	_beakerB[3] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_green01");
	_beakerB[4] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_orange01");
	_beakerB[5] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_purple01");
	_beakerB[6] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_brownSh01");
	_beakerB[7] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_false01");
	_beakerB[8] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker01");
	size = _beakerB[8]->getContentSize();
	pos = _beakerB[8]->getPosition();
	_beakerBRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);





	_beakerComplete = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_brown");
	size = _beakerComplete->getContentSize();
	pos = _beakerComplete->getPosition();
	_beakerCompleteRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_bowl[0] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl");
	_bowl[1] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_yellow");
	_bowl[2] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_darkgreen");
	_bowl[3] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_green");
	_bowl[4] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_red");
	size = _bowl[4]->getContentSize();
	pos = _bowl[4]->getPosition();
	_bowlRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


}

void labScene::SetItem() {
	//道具
	for (size_t i = 0; i < 20; i++)
	{
		_obj[i] = CItem::create();
		this->addChild(_obj[i]);

	}

}


void labScene::doStep(float dt)
{

	//walk===================================

	if (_bWalk && _bwithinArea) { //when player walk within walkable region
		_player->Walk(_touchLoc);
		if (_player->Walk(_touchLoc) == false) {
			_bWalk = 0;
			if (_bonObj) _bpickObj = true; //pick up obj
		}
		_player->go();

	}
	else if (_bWalk && _bonObj) { // when touched obj in scene that can be picked up
		//九宮格 2
		if (_touchLoc.y > WALK_AREA_1.y && _touchLoc.x > WALK_AREA_1.x && _touchLoc.x < WALK_AREA_2.x) {
			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_1.y));
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_1.y)) == false) {
				_bWalk = 0;
				_bpickObj = true;
			}
			_player->go();
		}
		//九宮格 3
		else if (_touchLoc.y > WALK_AREA_1.y && _touchLoc.x > WALK_AREA_2.x) {
			_player->Walk(Vec2(WALK_AREA_2.x, WALK_AREA_2.y));
			if (_player->Walk(Vec2(WALK_AREA_2.x, WALK_AREA_2.y)) == false) {
				_bWalk = 0;
				_bpickObj = true;
			}
			_player->go();
		}
		//九宮格 6
		else if (_touchLoc.y > WALK_AREA_3.y && _touchLoc.y < WALK_AREA_2.y && _touchLoc.x > WALK_AREA_2.x) {
			_player->Walk(Vec2(WALK_AREA_2.x, _touchLoc.y));
			if (_player->Walk(Vec2(WALK_AREA_2.x, _touchLoc.y)) == false) {
				_bWalk = 0;
				_bpickObj = true;
			}
			_player->go();
		}
		//九宮格 8
		else if (_touchLoc.y < WALK_AREA_4.y && _touchLoc.x > WALK_AREA_4.x && _touchLoc.x < WALK_AREA_3.x) {
			_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.y));
			if (_player->Walk(Vec2(_touchLoc.x, WALK_AREA_4.y)) == false) {
				_bWalk = 0;
				_bpickObj = true;
			}
			_player->go();
		}
	}
	else _player->Stop();


	// pick up obj ==========================
	PickObject(dt);

	// experiment ============================


	//if (_buseHerb) UseHerb();



	MixSyrup();

	//只能在設定範圍走=============================
	if (!_bonObj &&!(_touchLoc.x > WALK_AREA_4.x && _touchLoc.x > WALK_AREA_1.x && _touchLoc.x < WALK_AREA_2.x && _touchLoc.x < WALK_AREA_3.x
		&& _touchLoc.y > WALK_AREA_4.y  && _touchLoc.y > WALK_AREA_3.y  && _touchLoc.y < WALK_AREA_1.y && _touchLoc.y < WALK_AREA_2.y)) {
		_bWalk = 0;
		_player->Stop();
	}
}

void labScene::PickObject(float dt) {

	// pick up obj ==========================
	if (_bpickObj) { // when item in scene is being touched
		// check which trigger is being touched
		log("_bpickObj = true");
		_syrup[0]->doStep(dt);
		_syrup[1]->doStep(dt);
		_syrup[2]->doStep(dt);
		_syrup[3]->doStep(dt);

		_water->doStep(dt);
		_glassrod->doStep(dt);

		_herbRect[0]->doStep(dt);
		_herbRect[1]->doStep(dt);
		_herbRect[2]->doStep(dt);
		_herbRect[3]->doStep(dt);
		_procedure->doStep(dt);

		// create the corresponding item in bag
		if (_syrup[0]->GetAddToBag() && !_syrup[0]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_yellow.png"); //set bag item
			_obj[_itemTake]->SetTarget(_beakerARect); //set target rect
			_obj[_itemTake]->SetTarget(_beakerBRect);//set target rect
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			_obj[_itemTake]->SetStagnant(true); // set it to be always in bag

			*/
			_bag->AddObj("B_yellow.png", 2, _beakerRect, true);

			_syrup[0]->SetAddToBag(false);
			_syrup[0]->SetPicked(true); // if the object is picked and added into the bag

		}

		else if (_syrup[1]->GetAddToBag() && !_syrup[1]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_red.png");
			_obj[_itemTake]->SetTarget(_beakerARect);
			_obj[_itemTake]->SetTarget(_beakerBRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			_obj[_itemTake]->SetStagnant(true); // set it to be always in bag
			*/
			_bag->AddObj("B_red.png", 2, _beakerRect, true);
			_syrup[1]->SetAddToBag(false);
			_syrup[1]->SetPicked(true);

		}

		else if (_syrup[2]->GetAddToBag() && !_syrup[2]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_blue.png");
			_obj[_itemTake]->SetTarget(_beakerARect);
			_obj[_itemTake]->SetTarget(_beakerBRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			_obj[_itemTake]->SetStagnant(true); // set it to be always in bag
			*/
			_bag->AddObj("B_blue.png", 2, _beakerRect, true);
			_syrup[2]->SetAddToBag(false);
			_syrup[2]->SetPicked(true);

		}

		else if (_syrup[3]->GetAddToBag() && !_syrup[3]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_green.png");
			_obj[_itemTake]->SetTarget(_beakerARect);
			_obj[_itemTake]->SetTarget(_beakerBRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			_obj[_itemTake]->SetStagnant(true); // set it to be always in bag
			*/
			_bag->AddObj("B_green.png", 2, _beakerRect, true);
			_syrup[3]->SetAddToBag(false);
			_syrup[3]->SetPicked(true);

		}

		else if (_water->GetAddToBag() && !_water->GetPicked()) {
			/*_obj[_itemTake]->Init("B_water.png");
			_obj[_itemTake]->Clear();
			_obj[_itemTake]->Init("B_glassrod.png");
			_obj[_itemTake]->SetTarget(_beakerARect);
			_obj[_itemTake]->SetTarget(_beakerBRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			_obj[_itemTake]->SetStagnant(true); // set it to be always in bag
			*/
			_bag->AddObj("B_water.png", 2, _beakerRect, true);
			_water->SetAddToBag(false);
			_water->SetPicked(true);

		}


		else if (_glassrod->GetAddToBag() && !_glassrod->GetPicked()) {
			/*_obj[_itemTake]->Init("B_glassrod.png");
			_obj[_itemTake]->SetTarget(_beakerARect);
			_obj[_itemTake]->SetTarget(_beakerBRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			_obj[_itemTake]->SetStagnant(true); // set it to be always in bag
			*/
			_bag->AddObj("B_glassrod.png", 2, _beakerRect, true);
			_glassrod->SetAddToBag(false);
			_glassrod->SetPicked(true);
		}

		else if (_herbRect[0]->GetAddToBag() && !_herbRect[0]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_herbG.png");
			_obj[_itemTake]->SetRetake(_herbRect[0]);
			_obj[_itemTake]->SetTarget(_bowlRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true);
			*/
			_bag->AddObj("B_herbG.png", 1, _bowlRect, false, true, _herbRect[0]);
			_herbRect[0]->SetAddToBag(false);
			_herbRect[0]->SetPicked(true);
		}
		else if (_herbRect[1]->GetAddToBag() && !_herbRect[1]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_herbR.png");
			_obj[_itemTake]->SetRetake(_herbRect[1]);
			_obj[_itemTake]->SetTarget(_bowlRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true); */

			_bag->AddObj("B_herbR.png", 1, _bowlRect, false, true, _herbRect[1]);
			_herbRect[1]->SetAddToBag(false);
			_herbRect[1]->SetPicked(true);
		}
		else if (_herbRect[2]->GetAddToBag() && !_herbRect[2]->GetPicked()) {
			/*_obj[_itemTake]->Init("B_herbDG.png");
			_obj[_itemTake]->SetRetake(_herbRect[2]);
			_obj[_itemTake]->SetTarget(_bowlRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true); */

			_bag->AddObj("B_herbDG.png", 1, _bowlRect, false, true, _herbRect[2]);
			_herbRect[2]->SetAddToBag(false);
			_herbRect[2]->SetPicked(true);
		}

		else if (_herbRect[3]->GetAddToBag() && !_herbRect[3]->GetPicked()) {
			log("herb");
			/*_obj[_itemTake]->Init("B_herbY.png");
			_obj[_itemTake]->SetRetake(_herbRect[3]); // item can be retake
			_obj[_itemTake]->SetTarget(_bowlRect);
			this->addChild(_obj[_itemTake]);
			_bag->GetItem(_obj[_itemTake]); // put in list
			_obj[_itemTake]->SetVisible(true); // shows in bag
			_obj[_itemTake]->SetCanUse(true); */

			_bag->AddObj("B_herbY.png", 1, _bowlRect, false, true, _herbRect[3]);
			_herbRect[3]->SetAddToBag(false);
			_herbRect[3]->SetPicked(true);
		}




		_itemTake++;

		_bpickObj = false;

	}

}
void labScene::MixSyrup() {
	// detect what is being used===========================
	if (_obj[_iObjUsed]->GetUsed()) {
		if (!strcmp(_obj[_iObjUsed]->GetName(), "B_yellow.png")) {
			if(_obj[_iObjUsed]->GetTarget(0).containsPoint(Vec2(_endX, _endY)))      _buseSyrupA[0] = true;
			else if(_obj[_iObjUsed]->GetTarget(1).containsPoint(Vec2(_endX, _endY))) _buseSyrupB[0] = true;
		}
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_red.png")) {
			if (_obj[_iObjUsed]->GetTarget(0).containsPoint(Vec2(_endX, _endY)))     _buseSyrupA[1] = true;
			else if(_obj[_iObjUsed]->GetTarget(1).containsPoint(Vec2(_endX, _endY))) _buseSyrupB[1] = true;
		}
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_blue.png")) {
			if (_obj[_iObjUsed]->GetTarget(0).containsPoint(Vec2(_endX, _endY)))      _buseSyrupA[2] = true;
			else if (_obj[_iObjUsed]->GetTarget(1).containsPoint(Vec2(_endX, _endY))) _buseSyrupB[2] = true;
		}
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_green.png")) {
			if (_obj[_iObjUsed]->GetTarget(0).containsPoint(Vec2(_endX, _endY)))      _buseSyrupA[3] = true;
			else if (_obj[_iObjUsed]->GetTarget(1).containsPoint(Vec2(_endX, _endY))) _buseSyrupB[3] = true;
		}

		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_water.png")) _buseWater = true;
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_glassrod.png")) _buseGlassrod = true;

		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_herbG.png")) _bgrindG = true;
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_herbR.png")) _bgrindR = true;
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_herbDG.png")) _bgrindDG = true;
		else if (!strcmp(_obj[_iObjUsed]->GetName(), "B_herbY.png")) _bgrindY = true;

	}


	// for beaker A========================================
	//to get 【orange】 and 【purple】 syrup

		// add yellow syrup
		if (_buseSyrupA[0]) {
			_obj[_iObjUsed]->SetUsed(false);
			_beakerA[8]->setVisible(false);
			_beakerA[0]->setVisible(true);


			//add red syrup
			if (_buseSyrupA[1]) {
				_beakerA[1]->setVisible(false);
				_beakerA[4]->setVisible(true);

				log("A get orange syrup");
				_bgetOSyrup = true;
			}
			else if (_buseSyrupA[2] || _buseSyrupA[3]) {
				_beakerA[0]->setVisible(false);
				_beakerA[1]->setVisible(false);
				_beakerA[2]->setVisible(false);
				_beakerA[3]->setVisible(false);
				_beakerA[4]->setVisible(false);
				_beakerA[5]->setVisible(false);
				_beakerA[6]->setVisible(false);
				_beakerA[8]->setVisible(false);

				_beakerA[7]->setVisible(true);
			}
		}

		//add red syrup 
		else if (_buseSyrupA[1]) {
			_obj[_iObjUsed]->SetUsed(false);
			_beakerA[8]->setVisible(false);
			_beakerA[1]->setVisible(true);

			//add blue syrup
			if (_buseSyrupA[2]) {
				_beakerA[1]->setVisible(false);
				_beakerA[5]->setVisible(true);
				_bgetPSyrup = true;
			}
			//add yellow syrup
			else if (_buseSyrupA[0]) {
				_beakerA[1]->setVisible(false);
				_beakerA[4]->setVisible(true);
				_bgetOSyrup = true;
			}
			else if (_buseSyrupA[3]) {
				_beakerA[0]->setVisible(false);
				_beakerA[1]->setVisible(false);
				_beakerA[2]->setVisible(false);
				_beakerA[3]->setVisible(false);
				_beakerA[4]->setVisible(false);
				_beakerA[5]->setVisible(false);
				_beakerA[6]->setVisible(false);
				_beakerA[8]->setVisible(false);

				_beakerA[7]->setVisible(true);
			}
		}

		//add blue syrup
		else if (_buseSyrupA[2]) {
			_obj[_iObjUsed]->SetUsed(false);
			_beakerA[8]->setVisible(false);
			_beakerA[2]->setVisible(true);

			// add red syrup
			if (_buseSyrupA[1]) {
				_beakerA[2]->setVisible(false);
				_beakerA[5]->setVisible(true);
				_bgetPSyrup = true;
			}

			else if (_buseSyrupA[0] || _buseSyrupA[3]) {
				_beakerA[0]->setVisible(false);
				_beakerA[1]->setVisible(false);
				_beakerA[2]->setVisible(false);
				_beakerA[3]->setVisible(false);
				_beakerA[4]->setVisible(false);
				_beakerA[5]->setVisible(false);
				_beakerA[6]->setVisible(false);
				_beakerA[8]->setVisible(false);

				_beakerA[7]->setVisible(true);
			}
		}

		//add green syrup
		else if (_buseSyrupA[3]) {
			_beakerA[8]->setVisible(false);
			_beakerA[3]->setVisible(true);
			_obj[_iObjUsed]->SetUsed(false);

			if (_buseSyrupA[0] || _buseSyrupA[1] || _buseSyrupA[2]) {
				_beakerA[0]->setVisible(false);
				_beakerA[1]->setVisible(false);
				_beakerA[2]->setVisible(false);
				_beakerA[3]->setVisible(false);
				_beakerA[4]->setVisible(false);
				_beakerA[5]->setVisible(false);
				_beakerA[6]->setVisible(false);
				_beakerA[8]->setVisible(false);

				_beakerA[7]->setVisible(true);
			}
		}
	

	 //for beaker B========================================
	//to get 【orange】 and 【purple】 syrup

		//add yellow syrup
		if (_buseSyrupB[0]) {
			_beakerB[8]->setVisible(false);
			_beakerB[0]->setVisible(true);
			_obj[_iObjUsed]->SetUsed(false);

			//add red syrup
			if (_buseSyrupB[1]) {
				_beakerB[1]->setVisible(false);
				_beakerB[4]->setVisible(true);
				_bgetOSyrup = true;
			}
			else if (_buseSyrupB[2] || _buseSyrupB[3]) {
				_beakerB[0]->setVisible(false);
				_beakerB[1]->setVisible(false);
				_beakerB[2]->setVisible(false);
				_beakerB[3]->setVisible(false);
				_beakerB[4]->setVisible(false);
				_beakerB[5]->setVisible(false);
				_beakerB[6]->setVisible(false);
				_beakerB[8]->setVisible(false);

				_beakerB[7]->setVisible(true);
			}
		}


		//add red syrup 
		else if (_buseSyrupB[1]) {
			_obj[_iObjUsed]->SetUsed(false);
			_beakerB[8]->setVisible(false);
			_beakerB[1]->setVisible(true);

			// add blue syrup
			if (_buseSyrupB[2]) {
				_beakerB[1]->setVisible(false);
				_beakerB[5]->setVisible(true);
				_bgetPSyrup = true;
			}

			//add yellow syrup
			else if (_buseSyrupB[0]) {
				_beakerB[1]->setVisible(false);
				_beakerB[4]->setVisible(true);
				_bgetOSyrup = true;
			}

			else if (_buseSyrupB[3]) {
				_beakerB[0]->setVisible(false);
				_beakerB[1]->setVisible(false);
				_beakerB[2]->setVisible(false);
				_beakerB[3]->setVisible(false);
				_beakerB[4]->setVisible(false);
				_beakerB[5]->setVisible(false);
				_beakerB[6]->setVisible(false);
				_beakerB[8]->setVisible(false);

				_beakerB[7]->setVisible(true);
			}
		}

		//add blue syrup
		else if (_buseSyrupB[2]) {
			_beakerB[8]->setVisible(false);
			_beakerB[2]->setVisible(true);
			_obj[_iObjUsed]->SetUsed(false);

			// add red syrup
			if (_buseSyrupB[1]) {
				_beakerB[2]->setVisible(false);
				_beakerB[5]->setVisible(true);
				_bgetPSyrup = true;
			}

			else if (_buseSyrupB[0] || _buseSyrupB[3]) {
				_beakerB[0]->setVisible(false);
				_beakerB[1]->setVisible(false);
				_beakerB[2]->setVisible(false);
				_beakerB[3]->setVisible(false);
				_beakerB[4]->setVisible(false);
				_beakerB[5]->setVisible(false);
				_beakerB[6]->setVisible(false);
				_beakerB[8]->setVisible(false);

				_beakerB[7]->setVisible(true);
			}
		}


		//add green syrup
		else if (_buseSyrupB[3]) {
			_obj[_iObjUsed]->SetUsed(false);
			_beakerB[8]->setVisible(false);
			_beakerB[3]->setVisible(true);

			if (_buseSyrupB[0] || _buseSyrupB[1] || _buseSyrupB[2]) {
				_beakerB[0]->setVisible(false);
				_beakerB[1]->setVisible(false);
				_beakerB[2]->setVisible(false);
				_beakerB[3]->setVisible(false);
				_beakerB[4]->setVisible(false);
				_beakerB[5]->setVisible(false);
				_beakerB[6]->setVisible(false);
				_beakerB[8]->setVisible(false);

				_beakerB[7]->setVisible(true);
			}
		}
	


	// mixing 【A】 and 【B】
	if (_bgetOSyrup && _bgetPSyrup) {

		// 【A】to【B】
		if (_bpourIntoB) {
			_beakerA[4]->setVisible(false);
			_beakerA[5]->setVisible(false);
			_beakerA[8]->setVisible(true); //empty 【A】
			
			_beakerB[4]->setVisible(false);
			_beakerB[5]->setVisible(false);
			_beakerB[6]->setVisible(true); // complete syrup in 【B】

			_bgetBSyrup[1] = true;
		}

		// 【B】to【A】
		else if (_bpourIntoA) {
			_beakerB[4]->setVisible(false);
			_beakerB[5]->setVisible(false);
			_beakerB[8]->setVisible(true); //empty 【B】

			_beakerA[4]->setVisible(false);
			_beakerA[5]->setVisible(false);
			_beakerA[6]->setVisible(true); // complete syrup in 【A】
			_bgetBSyrup[0] = true;
		}

		
	}
}
void labScene::GrindHerb() {
	//grind dark green herb
	if (_bgrindDG) {
		_bowl[0]->setVisible(false);
		_bowl[2]->setVisible(true);
	}
	//grind green herb
	else if (_bgrindG) {
		_bowl[0]->setVisible(false);
		_bowl[3]->setVisible(true);
	}
	//grind red herb
	else if (_bgrindR) {
		_bowl[0]->setVisible(false);
		_bowl[4]->setVisible(true);
	}
	//grind yellow herb
	else if (_bgrindY) {
		_bowl[0]->setVisible(false);
		_bowl[1]->setVisible(true);
	}
}

//void labScene::UseHerb() {
//	if (_bgrindDG) {
//		_bowl[2]->setVisible(false);
//		if (_bgetBSyrup[0]) {
//			_
//		}
//	}
//}
//
//void labScene::EmptyBeakerA() {
//	_beakerA[8]->setVisible(true);
//
//	_beakerA[0]->setVisible(false);
//	_beakerA[1]->setVisible(false);
//	_beakerA[2]->setVisible(false);
//	_beakerA[3]->setVisible(false);
//	_beakerA[4]->setVisible(false);
//	_beakerA[5]->setVisible(false);
//	_beakerA[6]->setVisible(false);
//	_beakerA[7]->setVisible(false);
//
//	_buseSyrup[4] = false;
//	_bgetOSyrup = false;
//	_bgetPSyrup = false;
//	_bgetBSyrup[2] = false;
//	_bpourIntoA = false;
//	_bpourIntoB = false;
//}
//
//void labScene::EmptyBeakerB() {
//	_beakerB[8]->setVisible(true);
//
//	_beakerB[0]->setVisible(false);
//	_beakerB[1]->setVisible(false);
//	_beakerB[2]->setVisible(false);
//	_beakerB[3]->setVisible(false);
//	_beakerB[4]->setVisible(false);
//	_beakerB[5]->setVisible(false);
//	_beakerB[6]->setVisible(false);
//	_beakerB[7]->setVisible(false);
//
//	_buseSyrup[4] = false;
//	_bgetOSyrup = false;
//	_bgetPSyrup = false;
//	_bgetBSyrup[2] = false;
//	_bpourIntoA = false;
//	_bpourIntoB = false;
//}
//
//void labScene::EmptyBowl() {
//	_bowl[0]->setVisible(true);
//
//	_bowl[1]->setVisible(false);
//	_bowl[2]->setVisible(false);
//	_bowl[3]->setVisible(false);
//	_bowl[4]->setVisible(false);
//
//	_bgrindG = false;
//	_bgrindDG = false;
//	_bgrindR = false;
//	_bgrindY = false;
//
//}


bool labScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	_touchLoc = pTouch->getLocation();
	if (!_procedure->GetOpen()) {
		//swipe gesture
		_startX = _touchLoc.x;
		_startY = _touchLoc.y;



		if (_bbagOn) { //when bag is open
			//use items in bag===========================================
			/*for (size_t i = 0; i < _itemTake; i++)
			{
				if (_obj[i]->GetCanUse() == true) {
					if (_obj[i]->touchesBegan(_touchLoc)) {
						log("item touched");
					}
				}

			} */

			_bag->touchesBegan(_touchLoc);

			//if (_isLeft)
			//{
			//	_bag->MoveX(-100.0f);
			//	_isLeft = 0;
			//}
			//if (_isRight)
			//{
			//	_bag->MoveX(100.0f);
			//	_isRight = 0;
			//}
		}


	}
	

	return true;
}


void  labScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{

	_touchLoc = pTouch->getLocation();

	if (!_procedure->GetOpen()) {
		//use items in bag===========================================
		/* for (size_t i = 0; i < _itemTake; i++){
			if (_obj[i]->GetCanUse())_obj[i]->touchesMoved(_touchLoc);
		} */

		_bag->touchesMoved(_touchLoc);
	}

	
	
	
}

void  labScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	_touchLoc = pTouch->getLocation();

	//swipe gesture
	_endX = _touchLoc.x;
	_endY = _touchLoc.y;
	float offsetX = _endX - _startX;
	float offsetY = _endY - _startY;






	if (!_procedure->GetOpen()) {
		//use items in bag===========================================
		/*for (size_t i = 0; i < _itemTake; i++){
			if (_obj[i]->touchesEnded(_touchLoc)) {
				_iObjUsed = i;
				log("used obj");
				if (!_obj[i]->GetStagnant()) { // if object is not stagnant
					// when item is being used
					_obj[i]->SetCanUse(false); //item cannot be used again
					_bag->DeleteItem(_obj[i]); // delete from bag
					if (_obj[i]->GetRetake()) { //if the item can be retake when it is used
						(_obj[i]->GetTrigger())->SetPicked(false); // set it to be enabled
					}
				}


			}

		} */

		if (_bag->touchesEnded(_touchLoc)) {
			// mix mix
			int a;
		}

		

		// open/close/swipe bag
		if (_startY < BAG_OPEN_HEIGHT) { // when touched y< set height

			if (fabs(offsetX) < fabs(offsetY) && offsetY > 0) { // bag oppened
				_bag->setPosition(350, 250);
				_bag->SetItemRect();
				_bbagOn = true;
				log("up");

			}
		}

		if (_bbagOn && _startY <= BAG_CLOSE_HEIGHT) {
			if (fabs(offsetX) > fabs(offsetY)) {  //swipe bag
				if (offsetX < 0) { // left
					_isLeft = 1;
					log("left");
				}
				else { // right
					_isRight = 1;
					log("right");
				}
			}
			else if (fabs(offsetX) < fabs(offsetY)) { // close bag
				if (offsetY < 0) { //down
					_bag->setPosition(350, 0);
					_bag->SetItemRect();
					_bbagOn = false;
					log("down");
					_bag->ResetItemPos();


				}
			}
		}


		if (offsetX == 0 && offsetY == 0) { // when screen tapped

											//walk area ====================================
			if (_touchLoc.x > WALK_AREA_4.x && _touchLoc.x > WALK_AREA_1.x && _touchLoc.x < WALK_AREA_2.x && _touchLoc.x < WALK_AREA_3.x
				&& _touchLoc.y > WALK_AREA_4.y  && _touchLoc.y > WALK_AREA_3.y  && _touchLoc.y < WALK_AREA_1.y && _touchLoc.y < WALK_AREA_2.y) {
				// detect if touch pts are in walkable area

				_bwithinArea = true;
				log("walk!!");

			}
			else  _bwithinArea = false;




			//walk area (touch on obj) and pick obj ====================================
			if (_syrup[0]->touchesBegan(_touchLoc) ||
				_syrup[1]->touchesBegan(_touchLoc) ||
				_syrup[2]->touchesBegan(_touchLoc) ||
				_syrup[3]->touchesBegan(_touchLoc) ||
				_water->touchesBegan(_touchLoc) ||
				_glassrod->touchesBegan(_touchLoc) ||
				_herbRect[0]->touchesBegan(_touchLoc) ||
				_herbRect[1]->touchesBegan(_touchLoc) ||
				_herbRect[2]->touchesBegan(_touchLoc) ||
				_herbRect[3]->touchesBegan(_touchLoc) ||
				_procedure->TouchBegan(_touchLoc)
				) _bonObj = true;

			else _bonObj = false;

			// drag object =============================================
			//drag beaker
			if (_beakerARect.containsPoint(_touchLoc)) {

			}
			if (_beakerBRect.containsPoint(_touchLoc)) {

			}

			//drag bowl
			if (_bowlRect.containsPoint(_touchLoc)) {

			}



			////player walk =====================================================
			//if (!(_isTalking && _bagOpened)) {
			_bWalk = 1;
			if (_touchLoc.x > _player->_rpos.x) {
				_player->_bSide = 1;
				_player->Mirror();
			}
			else {
				_player->_bSide = 0;
				_player->Mirror();
			}

		}
	}
	else _procedure->TouchBegan(_touchLoc);
}
