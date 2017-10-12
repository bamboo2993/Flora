#include "kitchenScene.h"
#include "cocostudio/CocoStudio.h"




USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;


//
//
//Scene* kitchenScene::createScene()
//{
//	// 'scene' is an autorelease object
//	//auto scene = Scene::create();
//
//	auto scene = Scene::createWithPhysics();
//	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
//	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//	
//
//	// 'layer' is an autorelease object
//	auto layer = kitchenScene::create();
//
//	// add layer as a child to scene
//	scene->addChild(layer);
//
//	// return the scene
//	return scene;
//}
//
//
//kitchenScene::~kitchenScene()
//{
//
//
//	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
//	Director::getInstance()->getTextureCache()->removeUnusedTextures();
//}
//
//bool kitchenScene::init()
//{
//	
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//
//	//////////////////////////////
//	// 1. super init first
//	if (!Layer::init())
//	{
//		return false;
//	}
//	
//	
//	// 讀入儲存多張圖片的 plist 檔
//
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101.plist");
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101bg.plist");
//
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("kitchenSceneitem.plist");
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("kitchenSceneppl.plist");
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("kitchenScenebg.plist");
//	// ------------------------------------------------------------------------------------------------- 
//
//
//	////以 Sprite 作為背景
//	//Sprite *bkimage = Sprite::create(HOME_BACKGROUND);  // 使用 create 函式,給予檔名即可
//
//	_rootNode = CSLoader::createNode("kitchenScene.csb");
//	addChild(_rootNode);
//
//
//	setCabinet();
//	setNPC();
//	setPlayer();
//
//
//
//	
//
//
//	////npc talk=====================================================
//	//_blurbg = (cocos2d::Sprite*)rootNode->getChildByName("blurKitchenbg");
//	//_blurbg->setVisible(false);
//	//this->addChild(_blurbg);
//
//
//
//	//_playerEmo[0] = (cocos2d::Sprite*)rootNode->getChildByName("boyEmotion001");
//	//_playerEmo[0]->setVisible(false);
//	//_blurbg->addChild(_playerEmo[0]);
//
//	//_npcEmo[0] = (cocos2d::Sprite*)rootNode->getChildByName("chefEmotion003");
//	//_npcEmo[0]->setVisible(false);
//	//_blurbg->addChild(_npcEmo[0]);
//
//	//_npcEmo[1] = (cocos2d::Sprite*)rootNode->getChildByName("chefEmotion002");
//	//_npcEmo[1]->setVisible(false);
//	//_blurbg->addChild(_npcEmo[1]);
//
//
//
//
//
//
//	//_talkBox = LayerColor::create(ccc4(0, 0, 0, 170));
//	//_talkBox->setContentSize(CCSizeMake(visibleSize.width / 10 * 9, visibleSize.height / 4));
//	//_talkBox->setVisible(false);
//	//size = _talkBox->getContentSize();
//	//_talkBox->setPosition(Vec2(visibleSize.width / 2 - size.width / 2, visibleSize.height / 8));
//	//_blurbg->addChild(_talkBox);
//
//
//
//	//_talkLabel[0] = Label::createWithBMFont("fonts/couriernew32.fnt", _sTalk[0]);
//	//_talkLabel[0]->setColor(Color3B::WHITE);  // 設定顏色
//
//	//Size labelsize = _talkLabel[0]->getContentSize();
//	//_talkLabel[0]->setPosition(Vec2(size.width / 10 + labelsize.width / 2, size.height / 3 * 2));
//	//_talkLabel[0]->setScale(1.0f);
//	//_talkLabel[0]->setVisible(true);
//
//	//_talkBox->addChild(_talkLabel[0], 1);
//
//	//_talkLabel[1] = Label::createWithBMFont("fonts/couriernew32.fnt", _sTalk[1]);
//	//_talkLabel[1]->setColor(Color3B::WHITE);  // 設定顏色
//
//	//labelsize = _talkLabel[1]->getContentSize();
//	//_talkLabel[1]->setPosition(Vec2(size.width / 10 + labelsize.width / 2, size.height / 3 * 2));
//	//_talkLabel[1]->setScale(1.0f);
//	//_talkLabel[1]->setVisible(false);
//
//	//_talkBox->addChild(_talkLabel[1], 1);
//
//
//	//_talkLabel[2] = Label::createWithBMFont("fonts/couriernew32.fnt", _sTalk[2]);
//	//_talkLabel[2]->setColor(Color3B::WHITE);  // 設定顏色
//
//	//labelsize = _talkLabel[2]->getContentSize();
//	//_talkLabel[2]->setPosition(Vec2(size.width / 10 + labelsize.width / 2, size.height / 3 * 2));
//	//_talkLabel[2]->setScale(1.0f);
//	//_talkLabel[2]->setVisible(false);
//
//	//_talkBox->addChild(_talkLabel[2], 1);
//
//
//
//
//
//
//	////bag button
//	//_bagBtn = new C3SButton();
//	//_bagBtn->PicSetting("bag_normal.png", "bag_click.png", "bag_normal.png", *this, Point(visibleSize.width / 10, visibleSize.height / 10 * 9));
//
//	////===========================================================================================================
//	//
//
//
//
//	//_grid = (cocos2d::Sprite*)rootNode->getChildByName("grid_1");
//	//_grid->setVisible(false);
//	//size = _grid->getContentSize();
//	//pos = _grid->getPosition();
//	//_gridRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
//	//this->addChild(_grid, 20);
//	//Point gridOrigin = Vec2(pos.x - size.width / 2, pos.y - size.height / 2);
//
//	///*_grid->setPhysicsBody(PhysicsBody::createBox(size));
//	//_grid->getPhysicsBody()->setDynamic(false);*/
//
//	////===========================================================================================================
//	//_itemDescribtion = LayerColor::create(ccc4(0, 0, 0, 170));
//	//_itemDescribtion->setContentSize(CCSizeMake(size.width, size.height));
//	//_itemDescribtion->setVisible(false);
//	//_grid->addChild(_itemDescribtion);
//
//	//Size gridSize = _itemDescribtion->getContentSize();
//	//_itemDescribtion->setPosition(Vec2(visibleSize.width / 4 * 3 - gridSize.width / 2 - gridOrigin.x, visibleSize.height / 2 - gridSize.height / 2 - gridOrigin.y - 15));
//
//
//
//
//
//	//_describtionObj = Sprite::createWithSpriteFrameName("cheese.png");
//	//_describtionObj->setScale(2.5f);
//	//_describtionObj->setPosition(Vec2(gridSize.width / 2, gridSize.height / 3 * 2));
//	//_itemDescribtion->addChild(_describtionObj);
//
//
//
//	//_describtionWord = Label::createWithBMFont("fonts/couriernew32.fnt", "This is a cheese.");
//	//_describtionWord->setColor(Color3B::WHITE);  // 設定顏色
//	//_describtionWord->setPosition(Vec2(gridSize.width / 2, gridSize.height / 3));
//	//_describtionWord->setScale(1.1f);
//	//_itemDescribtion->addChild(_describtionWord, 1);
//
//
//
//	////===========================================================================================================
//
//	//_bagObject = (cocos2d::Sprite*)rootNode->getChildByName("cheese");
//	//_bagObject->setVisible(false);
//	//size = _bagObject->getContentSize();
//	//pos = _bagObject->getPosition();
//	//_RectbagObject = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
//	//pos.x = pos.x - gridOrigin.x;
//	//pos.y = pos.y - gridOrigin.y;
//	//_grid->addChild(_bagObject);
//	//_bagObject->setPosition(Vec2(pos));
//
//
//
//	///*_bagObject->setPhysicsBody(PhysicsBody::createBox(size));
//	//_bagObject->getPhysicsBody()->setDynamic(false);*/
//
//
//
//
//	///*
//	//_tran = new cFridge();
//	//_tran->init();
//	//_tran->Setup("flame1_04.png", "cuber05.png", Vec2(300.0f, 200.0f), *this);
//
//	//this->addChild(_tran,2);*/
//
//	////// 自行增加 sprite 將 bean01.png 到螢幕正中間
//	////_bean1 = Sprite::create("scene101/bean01.png");  // 使用 create 函式,給予檔名即可
//	////_bean1->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); // 位置通常放置在螢幕正中間
//	////this->addChild(_bean1, 1);
//
//	////size = _bean1->getContentSize();
//	////Point beanpos = _bean1->getPosition();
//	////rectBean = Rect(beanpos.x - size.width / 2, beanpos.y - size.height / 2, size.width, size.height);
//	////_bOnBean = false;
//
//
//
//
//
//	////_fridgeTop = new cItem();
//	////_fridgeTop->setInfo("fridgeTop.png", *this, Point(222,222),);
//
//
//
//	//-------------------------------------------------------------------------------------------------
//
//	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
//	_listener1->onTouchBegan = CC_CALLBACK_2(kitchenScene::onTouchBegan, this);		//加入觸碰開始事件
//	_listener1->onTouchMoved = CC_CALLBACK_2(kitchenScene::onTouchMoved, this);		//加入觸碰移動事件
//	_listener1->onTouchEnded = CC_CALLBACK_2(kitchenScene::onTouchEnded, this);		//加入觸碰離開事件
//
//	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
//	this->schedule(CC_SCHEDULE_SELECTOR(kitchenScene::doStep));
//	return true;
//}
//
//
//void kitchenScene::setCabinet() {
//	
//	for (int i = 0; i < 6; i++)
//	{
//		_Cabinet[i] = new CBox();
//	}
//
//	// _Cabinet[0]
//	_Cabinet[0]->SetInfo(Vec2(278.12f, 310.21f), Vec2(277.62f, 398.29f), Vec2(418.45f, 435.12f), Vec2(420.44f, 358.98f), Vec2(373.18f, 357.97f), "cabinet03.png", *this);
//	_Cabinet[0]->setItem(Vec2(353.0f , 358.0f), "cheese.png");
//	_Cabinet[0]->setItemCard("cheeseCard.png", *this);
//	this->addChild(_Cabinet[0], 1);
//
//	// _Cabinet[1]
//
//
//}
//
//
//void kitchenScene::setNPC() {
//	_npc = (cocos2d::Sprite*)_rootNode->getChildByName("chef");
//	Size size = _npc->getContentSize();
//	Point pos = _npc->getPosition();
//	this->addChild(_npc);
//
//	_npcRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
//	_npcTalkRect = Rect(pos.x - size.width / 2 - 10, pos.y - size.height / 2 - 10, size.width + 20, size.height + 20);
//
//
//	//_npc->setPhysicsBody(PhysicsBody::createBox(size));
//	//_npc->getPhysicsBody()->setDynamic(false);
//}
//
//void kitchenScene::setPlayer() {
//
//	// set player==============================================================
//
//	_Player = new CPlayer("boy.png", "boywalk001.png",*this);
//	_Player->setAnimation("boyanim.plist");
//	//_player->setPosition(Point(950, 320));
//	_Player->setRect();
//
//}