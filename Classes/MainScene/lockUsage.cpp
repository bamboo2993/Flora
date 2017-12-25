#include "lockUsage.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;


Scene* lockUsage::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = lockUsage::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

lockUsage::~lockUsage()
{


	this->removeAllChildren();
	//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool lockUsage::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// Ū�J�x�s�h�i�Ϥ��� plist ��
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainscene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SceneKitchenitem.plist");

	//cocos studio
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);

	////add background
	//auto bgSprite = Sprite::createWithSpriteFrameName("Mainscene02.png");
	//bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2); //set position
	//addChild(bgSprite, 0);






	auto bgSprite = Sprite::createWithSpriteFrameName("apple.png");
	bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2); //set position
	addChild(bgSprite, 0);
	Size size = bgSprite->getContentSize();
	Point p = bgSprite->getPosition();

	_lockAreaRect = Rect(p.x - size.width / 2, p.y - size.height / 2, size.width, size.height);

	//_lock = new CLock();
	//_lock->init();
	//this->addChild(_lock);

	//_lock->SetArea(_lockAreaRect);
	//_lock->SetPassword(1, 1, 1);


	_elock = new CeLock(2);
	_elock->init("GameScene/lab_door.png");
	this->addChild(_elock);
	_elock->SetArea(_lockAreaRect);
	_elock->SetKeyArea(Point(896.8f,692.6f),287,254);
	_elock->SetEnterArea(Point(896.8f, 617.6f), 287, 72);
	_elock->SetNumAppear(1149.0f+40.0f, 1032.0f);
	_elock->SetPassword(3,721);


	//=============================================================================================================================================

	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(lockUsage::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(lockUsage::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(lockUsage::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(lockUsage::doStep));

	return true;
}


void lockUsage::doStep(float dt)
{

	//if (_bToScene001) { // ������ SecondScene
	//						// ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
	//	this->unschedule(schedule_selector(lockUsage::doStep));
	//	//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	//	// �]�w�����������S��
	//	//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
	//	//		Director::getInstance()->replaceScene(pageTurn);
	//	Director::getInstance()->replaceScene(SceneKitchen::createScene());
	//	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//}

	//if (_bToScene002) { // ������ SecondScene
	//					   // ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
	//	this->unschedule(schedule_selector(lockUsage::doStep));
	//	//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	//	// �]�w�����������S��
	//	//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
	//	//		Director::getInstance()->replaceScene(pageTurn);
	//	Director::getInstance()->replaceScene(selectScene::createScene());
	//	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//}

	//if (_bToScene003) { // ������ SecondScene
	//					// ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
	//	this->unschedule(schedule_selector(lockUsage::doStep));
	//	//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	//	// �]�w�����������S��
	//	//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
	//	//		Director::getInstance()->replaceScene(pageTurn);
	//	Director::getInstance()->replaceScene(selectScene::createScene());
	//	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//}
}

bool lockUsage::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	Point touchLoc = pTouch->getLocation();
	//_lock->TouchBegan(touchLoc);
	_elock->TouchBegan(touchLoc);

	/*_StartBtn->touchesBegin(touchLoc);
	_ExitBtn->touchesBegin(touchLoc);
	_ContinueBtn->touchesBegin(touchLoc);*/

	return true;
}

void  lockUsage::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();

	//_lock->TouchMoved(touchLoc);
	_elock->TouchMoved(touchLoc);
	/*_StartBtn->touchesMoved(touchLoc);
	_ExitBtn->touchesMoved(touchLoc);
	_ContinueBtn->touchesMoved(touchLoc);*/

}

void  lockUsage::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();

	//_lock->TouchEnded(touchLoc);
	_elock->TouchEnded(touchLoc);

	//if (_StartBtn->touchesEnded(touchLoc)) { // �i�����������
	//	_bToSelectScene = true;
	//}
	//if (_ExitBtn->touchesEnded(touchLoc)) { // �i�����������
	//	unscheduleAllCallbacks();
	//	Director::getInstance()->end();
	//}
	//if (_ContinueBtn->touchesEnded(touchLoc)) { // �i�����������
	//	_bToLoadingScene = true;
	//}

}