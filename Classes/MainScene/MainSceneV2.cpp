#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainScene.h"



USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;
//
//Scene* MainScene::createScene()
//{
//	// 'scene' is an autorelease object
//	auto scene = Scene::create();
//
//	// 'layer' is an autorelease object
//	auto layer = MainScene::create();
//
//	// add layer as a child to scene
//	scene->addChild(layer);
//
//	// return the scene
//	return scene;
//}
//
//MainScene::~MainScene()
//{
//	delete _StartBtn;
//	delete _ExitBtn;
//	delete _ContinueBtn;
//
//	this->removeAllChildren();
//	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
//	Director::getInstance()->getTextureCache()->removeUnusedTextures();
//}
//
//bool MainScene::init()
//{
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//	// 1. super init first
//	if (!Layer::init())
//	{
//		return false;
//	}
//
//	// Ū�J�x�s�h�i�Ϥ��� plist ��
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainscene.plist");
//
//	//cocos studio
//		auto rootNode = CSLoader::createNode("MainScene.csb");
//		addChild(rootNode);
//
//	//add background
//	auto bgSprite = Sprite::createWithSpriteFrameName("Mainscene01.png");
//	bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2); //set position
//	addChild(bgSprite, 0);
//
//
//
//	//button======================================================================================================================================
//	//start button
//	_StartBtn = new CButton();
//	_StartBtn->setButtonInfo("startNormal.png", "startTouched.png", *this, Point(visibleSize.width / 2, visibleSize.height / 2));
//
//	_bToSelectScene = false; //start game
//
//	//exit button
//	_ExitBtn = new CButton();
//	_ExitBtn->setButtonInfo("exitNormal.png", "exitTouched.png", *this, Point(visibleSize.width / 2, visibleSize.height / 2));
//	
//
//	//setting button
//	_ContinueBtn = new CButton();
//	_ContinueBtn->setButtonInfo("continueNormal.png", "continueTouched.png", *this, Point(visibleSize.width / 2, visibleSize.height / 2));
//	_bToLoadingScene = false; //setting
//	//=============================================================================================================================================
//
//	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
//	_listener1->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
//	_listener1->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
//	_listener1->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�
//
//	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
//	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::doStep));
//
//	return true;
//}
//
//
//void MainScene::doStep(float dt)
//{
//	if (_bToLoadingScene) { // ������ SecondScene
//						 // ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
//		this->unschedule(schedule_selector(MainScene::doStep));
//		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
//		// �]�w�����������S��
//		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
//		//		Director::getInstance()->replaceScene(pageTurn);
//		Director::getInstance()->replaceScene(kitchenScene::createScene());
//		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//	}
//
//	if (_bToSelectScene) { // ������ SecondScene
//							// ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
//		this->unschedule(schedule_selector(MainScene::doStep));
//		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
//		// �]�w�����������S��
//		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
//		//		Director::getInstance()->replaceScene(pageTurn);
//		Director::getInstance()->replaceScene(SelectionScene::createScene());
//		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//	}
//}
//
//bool MainScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
//{
//	Point touchLoc = pTouch->getLocation();
//
//	_StartBtn->touchesBegin(touchLoc);
//	_ExitBtn->touchesBegin(touchLoc);
//	_ContinueBtn->touchesBegin(touchLoc);
//
//	return true;
//}
//
//void  MainScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
//{
//	Point touchLoc = pTouch->getLocation();
//	_StartBtn->touchesMoved(touchLoc);
//	_ExitBtn->touchesMoved(touchLoc);
//	_ContinueBtn->touchesMoved(touchLoc);
//
//}
//
//void  MainScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
//{
//	Point touchLoc = pTouch->getLocation();
//	if (_StartBtn->touchesEnded(touchLoc)) { // �i�����������
//		_bToSelectScene = true;
//	}
//	if (_ExitBtn->touchesEnded(touchLoc)) { // �i�����������
//		unscheduleAllCallbacks();
//		Director::getInstance()->end();
//	}
//	if (_ContinueBtn->touchesEnded(touchLoc)) { // �i�����������
//		_bToLoadingScene = true;
//	}
//
//}