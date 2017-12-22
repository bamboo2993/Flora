#include "SelectionScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


#include "ActionScene\RunScene\RunScene1.h"
#include "GameScene\BMScene.h"


USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;


Scene* SelectionScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SelectionScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

SelectionScene::~SelectionScene()
{


	this->removeAllChildren();
	//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool SelectionScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// 讀入儲存多張圖片的 plist 檔

	//cocos studio
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);



	_select[0] = (cocos2d::Sprite*)rootNode->getChildByName("select01");
	Size size = _select[0]->getContentSize();
	Point pos = _select[0]->getPosition();
	_selectRect[0] = Rect(pos.x - size.height / 2*3, pos.y - size.width / 2 * 3, size.height * 3, size.width * 3);

	_select[1] = (cocos2d::Sprite*)rootNode->getChildByName("select02");
	size = _select[1]->getContentSize();
	pos = _select[1]->getPosition();
	_selectRect[1] = Rect(pos.x - size.height / 2 * 3, pos.y - size.width / 2 * 3, size.height * 3, size.width * 3);

	_select[2] = (cocos2d::Sprite*)rootNode->getChildByName("select03");
	size = _select[2]->getContentSize();
	pos = _select[2]->getPosition();
	_selectRect[2] = Rect(pos.x - size.height / 2 * 3, pos.y - size.width / 2 * 3, size.height * 3, size.width * 3);

	//sound
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/bensound-november.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //尚未實作

	//=============================================================================================================================================

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(SelectionScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(SelectionScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(SelectionScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(SelectionScene::doStep));

	return true;
}


void SelectionScene::doStep(float dt)
{
	

	//if (_bToScene001) { // 切換到 SecondScene
	//						// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
	//	this->unschedule(schedule_selector(SelectionScene::doStep));
	//	//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	//	// 設定場景切換的特效
	//	//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
	//	//		Director::getInstance()->replaceScene(pageTurn);
	//	Director::getInstance()->replaceScene(SceneKitchen::createScene());
	//	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//}

	//if (_bToScene002) { // 切換到 SecondScene
	//					   // 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
	//	this->unschedule(schedule_selector(SelectionScene::doStep));
	//	//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	//	// 設定場景切換的特效
	//	//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
	//	//		Director::getInstance()->replaceScene(pageTurn);
	//	Director::getInstance()->replaceScene(selectScene::createScene());
	//	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//}

	//if (_bToScene003) { // 切換到 SecondScene
	//					// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
	//	this->unschedule(schedule_selector(SelectionScene::doStep));
	//	//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	//	// 設定場景切換的特效
	//	//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
	//	//		Director::getInstance()->replaceScene(pageTurn);
	//	Director::getInstance()->replaceScene(selectScene::createScene());
	//	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//}


	if (_bToScene[0]) { // 切換到 SecondScene
							// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
		log("aaa");
		this->unschedule(schedule_selector(SelectionScene::doStep));
		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// 設定場景切換的特效
		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
		//		Director::getInstance()->replaceScene(pageTurn);
		Director::getInstance()->replaceScene(BMScene::createScene());
//		Director::getInstance()->replaceScene(labScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		
	}


	if (_bToScene[1]) { // 切換到 SecondScene
						// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
		log("bbb");
		this->unschedule(schedule_selector(SelectionScene::doStep));
		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// 設定場景切換的特效
		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
		//		Director::getInstance()->replaceScene(pageTurn);
		
		Director::getInstance()->replaceScene(BMScene::createScene());
		//Director::getInstance()->replaceScene(labScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}


	if (_bToScene[2]) { // 切換到 SecondScene
						// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
		log("ccc");
		this->unschedule(schedule_selector(SelectionScene::doStep));
		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// 設定場景切換的特效
		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
		//		Director::getInstance()->replaceScene(pageTurn);
		Director::getInstance()->replaceScene(BMScene::createScene());
///		Director::getInstance()->replaceScene(labScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		
	}
}

bool SelectionScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();
	if (_selectRect[0].containsPoint(touchLoc)) {
		log("a");
		_bToScene[0] = true;


	}
	else if (_selectRect[1].containsPoint(touchLoc)) {
		log("b");
		_bToScene[1] = true;

	}
	else if (_selectRect[2].containsPoint(touchLoc)) {
		log("c");
		_bToScene[2] = true;

	}


	return true;
}

void  SelectionScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();


}

void  SelectionScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();




	//if (_StartBtn->touchesEnded(touchLoc)) { // 進行場景的切換
	//	_bToSelectScene = true;
	//}
	//if (_ExitBtn->touchesEnded(touchLoc)) { // 進行場景的切換
	//	unscheduleAllCallbacks();
	//	Director::getInstance()->end();
	//}
	//if (_ContinueBtn->touchesEnded(touchLoc)) { // 進行場景的切換
	//	_bToLoadingScene = true;
	//}

}