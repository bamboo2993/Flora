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


	Sprite *bkimage = Sprite::create(HOME_BACKGROUND);  // �ϥ� create �禡,�����ɦW�Y�i
	bkimage->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); // ��m�q�`��m�b�ù�������
	//bkimage->setScale(0.365f);
	this->addChild(bkimage, 0);

	//sound
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/bensound-november.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //�|����@


	//start=======================================================================
	_label1 = Label::createWithBMFont("fonts/couriernew32.fnt", "Start");
	Size size = _label1->getContentSize();

	_label1->setColor(Color3B::WHITE);  // �]�w�C��
	_label1->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + visibleSize.height/3 - size.height));
	_label1->setScale(2);
	this->addChild(_label1, 1);
	Point pos = _label1->getPosition();
	_startRect = Rect(pos.x - size.width , pos.y - size.height , size.width *2, size.height*2);

	// exit======================================================================
	_label2 = Label::createWithBMFont("fonts/couriernew32.fnt", "Exit");
	size = _label2->getContentSize();

	_label2->setColor(Color3B::WHITE);  // �]�w�C��
	_label2->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + visibleSize.height / 4 - size.height));
	_label2->setScale(2);
	this->addChild(_label2, 1);
	pos = _label2->getPosition();
	_exitRect = Rect(pos.x - size.width, pos.y - size.height, size.width*2, size.height*2);


//=============================================================================================================================================

	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::doStep));

	return true;
}


void MainScene::doStep(float dt)
{
	if (_bToGameScene) { // ������ SecondScene
						 // ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
		this->unschedule(schedule_selector(MainScene::doStep));
		//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// �]�w�����������S��
		//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
		//		Director::getInstance()->replaceScene(pageTurn);
		Director::getInstance()->replaceScene(SelectionScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}

bool MainScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
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

void  MainScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();
	//_StartBtn->TouchMoved(touchLoc);
	//_ExitBtn->TouchMoved(touchLoc);
	//_SettingBtn->TouchMoved(touchLoc);

}

void  MainScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();
	//if (_StartBtn->TouchEnded(touchLoc)) { // �i�����������
	//	_bToGameScene = true;
	//}
	//if (_ExitBtn->TouchEnded(touchLoc)) { // �i�����������
	//	unscheduleAllCallbacks();
	//	Director::getInstance()->end();
	//}
	//if (_SettingBtn->TouchEnded(touchLoc)) { // �i�����������
	//	_bToSettingScene = true;
	//}
	
}