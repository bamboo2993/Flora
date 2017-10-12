#include "Mainscene.h"
#include "SelectionScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

#define HOME_BACKGROUND "MainScene/Mainscene.png"

Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

MainScene::~MainScene()
{
	this->removeAllChildren();
	//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool MainScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	Sprite *bkimage = Sprite::create(HOME_BACKGROUND);  // 使用 create 函式,給予檔名即可
	bkimage->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); // 位置通常放置在螢幕正中間
	//bkimage->setScale(0.365f);
	this->addChild(bkimage, 0);

	//sound
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/bensound-november.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //尚未實作


	//start=======================================================================
	_label1 = Label::createWithBMFont("fonts/couriernew32.fnt", "Start");
	Size size = _label1->getContentSize();

	_label1->setColor(Color3B::WHITE);  // 設定顏色
	_label1->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + visibleSize.height/3 - size.height));
	_label1->setScale(2);
	this->addChild(_label1, 1);
	Point pos = _label1->getPosition();
	_startRect = Rect(pos.x - size.width , pos.y - size.height , size.width *2, size.height*2);

	// exit======================================================================
	_label2 = Label::createWithBMFont("fonts/couriernew32.fnt", "Exit");
	size = _label2->getContentSize();

	_label2->setColor(Color3B::WHITE);  // 設定顏色
	_label2->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + visibleSize.height / 4 - size.height));
	_label2->setScale(2);
	this->addChild(_label2, 1);
	pos = _label2->getPosition();
	_exitRect = Rect(pos.x - size.width, pos.y - size.height, size.width*2, size.height*2);


//=============================================================================================================================================

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::doStep));

	return true;
}


void MainScene::doStep(float dt)
{
	if (_bToGameScene) { // 切換到 SecondScene
						 // 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
		this->unschedule(schedule_selector(MainScene::doStep));
		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// 設定場景切換的特效
		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
		//		Director::getInstance()->replaceScene(pageTurn);
		Director::getInstance()->replaceScene(SelectionScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}

bool MainScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();
	if (_exitRect.containsPoint(touchLoc)) {
		unscheduleAllCallbacks();
		Director::getInstance()->end();
	}
	else if (_startRect.containsPoint(touchLoc)) {
		_bToGameScene = true;
	}

	



	/*_StartBtn->TouchBegan(touchLoc);
	_ExitBtn->TouchBegan(touchLoc);
	_SettingBtn->TouchBegan(touchLoc);*/



	return true;
}

void  MainScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();
	//_StartBtn->TouchMoved(touchLoc);
	//_ExitBtn->TouchMoved(touchLoc);
	//_SettingBtn->TouchMoved(touchLoc);

}

void  MainScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();
	//if (_StartBtn->TouchEnded(touchLoc)) { // 進行場景的切換
	//	_bToGameScene = true;
	//}
	//if (_ExitBtn->TouchEnded(touchLoc)) { // 進行場景的切換
	//	unscheduleAllCallbacks();
	//	Director::getInstance()->end();
	//}
	//if (_SettingBtn->TouchEnded(touchLoc)) { // 進行場景的切換
	//	_bToSettingScene = true;
	//}
	
}