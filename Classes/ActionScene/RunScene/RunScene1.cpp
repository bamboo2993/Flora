#include "RunScene1.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
//#include <unordered_map>
//#include "ActionScene\RunScene\CObject.h"

#include "GameScene\BR_Scene.h"

USING_NS_CC;

#define MOVESPEED 250

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;
using namespace std;

Scene* RunScene1::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	//auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);


	// 'layer' is an autorelease object
	auto layer = RunScene1::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

RunScene1::~RunScene1()
{
	_bcolBike = false;
	_bpause = false;

	_bwin = false;
	_brestart = false;
	_bStart = false;
	_t = 0.0f;
	//AnimationCache::destroyInstance();  // ���� AnimationCache ���o���귽
	//SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	//Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool RunScene1::init()
{
	_aniStop = false;
	_skip = false;
	_picTimeCount = 0;
	_cutPic = false;
	_cutPic_2 = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// Ū�J�x�s�h�i�Ϥ��� plist ��----------------------------------------------------
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("runscene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("runscene02.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/ater.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SceneKitchenitem.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("childhoodAni.plist");

	
	auto rootNode = CSLoader::createNode("RunScene.csb");
	this->addChild(rootNode);


	this->_blur[0] = (cocos2d::Sprite *)rootNode->getChildByName("b1");
	this->addChild(_blur[0], 20);
	_blur[0]->setVisible(false);
	this->_blur[1] = (cocos2d::Sprite *)rootNode->getChildByName("b2");
	this->addChild(_blur[1], 20);
	_blur[1]->setVisible(false);

	this->_blur[2] = (cocos2d::Sprite *)rootNode->getChildByName("b3");
	this->addChild(_blur[2], 20);
	_blur[2]->setVisible(false);


	


	// ���ĻP���� ----------------------------------------------------------------------------------


	// ���s ------------------------------------------------------------------------------------------
	//palse
	_pause = new CButton();
	_pause->setButtonInfo("apple.png", "apple.png", *this, Vec2(visibleSize.width / 10, visibleSize.height / 12 * 11));

	// reset button
	_reset = (cocos2d::Sprite*) rootNode->getChildByName("reset");
	Size size = _reset->getContentSize();
	Point pos = _reset->getPosition();
	_resetRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);

	auto _reset2 = (cocos2d::Sprite*) rootNode->getChildByName("replay");
	size = _reset2->getContentSize();
	pos = _reset2->getPosition();
	_resetRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);



	// runner setup ----------------------------------------------------------------------------
	_runner = new CRunner("ater_001.png", *this);
	_runner->setAnimation("Animation/boyanim.plist");
	_runner->setPosition(320, 650);


	Setbg(); //�]�w�I�� + ��ê��

	// bicycle obstacle-----------------------------------------------------
	_bike= Sprite::create("res/ActionScene/RunScene/skateboarder.png");
	_bike->setPosition(2150, 450); 
	_bikePt = _bike->getPosition();
	this->addChild(_bike,5);

	// trash obstacle----------------------------------------
	_trash[0] = new CFall(_fgnode1, *this);
//	_trash[0]->setPosition(300, 700);
	_trash[1] = new CFall(_fgnode2, *this);
//	_trash[0]->setPosition(900, 700);
	

//Ani
	_opendoorAnimation_2 = new AniScene();
	_opendoorAnimation_2->init(Point(visibleSize.width / 2, origin.y + visibleSize.height / 2.0), *this, 5, "2");
	_pic105 = Sprite::createWithSpriteFrameName("105.png");
	_pic105->setPosition(Point(visibleSize.width / 2, origin.y + visibleSize.height / 2.0));
	_pic105->setVisible(true);
	this->addChild(_pic105,20001);
	_opendoorAnimation_1 = new AniScene();
	_opendoorAnimation_1->init(Point(visibleSize.width / 2, origin.y + visibleSize.height / 2.0), *this, 4, "1");

	_skipSprite = Sprite::create("skip_click.png");
	_skipSprite->setPosition(1933.00f, 63.78f);
	this->addChild(_skipSprite,20002);
	size = _skipSprite->getContentSize();
	pos = _skipSprite->getPosition();
	_skipRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height );

	SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/opendoor.mp3", true);


	//-------------------------------------------------------------------------------------------------
	restart();

	//key
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(RunScene1::keyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(RunScene1::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(RunScene1::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(RunScene1::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(RunScene1::doStep));
	return true;
}



void RunScene1::Setbg() {

	// Ū���ëإ� _fgnode1 
	// �Ĥ@�q�e�� --------------------------------------------------------------
	_fgnode1 = CSLoader::createNode("fgnode.csb");
	_fgnode1->setPosition(1024, 0);
	addChild(_fgnode1, 3);

	_fg1Loc[0].x = -238 + (float)0 * 900;
	_fg1Loc[0].y = 0;




	// �إ�2���H���I������
	_fg1obj[0] = CObject::create();
	_fg1obj[0]->init("housenode.csb", 2, _fg1Loc[0]);
	_fgnode1->addChild(_fg1obj[0], -1);	//�[�J_fgnode1 ��
	//_fg1obj[1] = CObject::create();
	//_fg1obj[1]->init("housenode.csb", 3, _fg1Loc[1]);
	//_fgnode1->addChild(_fg1obj[1], -1);	//�[�J_fgnode1 ��



	// �ĤG�q�e�� ----------------------------------------------------------
	_fgnode2 = CSLoader::createNode("fgnode.csb");
	_fgnode2->setPosition(2048 + 1024, 0);
	this->addChild(_fgnode2, 1);
//	for (int i = 0; i < 2; i++)
//	{
		_fg2Loc[0].x = 0;// -405 + (float)0 * 1020;
		_fg2Loc[0].y = 0;
//	}
	// �إߤT���H���I������
	_fg2obj[0] = CObject::create();
	_fg2obj[0]->init("housenode.csb", 2, _fg2Loc[0]);
	_fgnode2->addChild(_fg2obj[0], -1);	// �[�J _fgnode2 ��
	//_fg2obj[1] = CObject::create();
	//_fg2obj[1]->init("housenode.csb", 3, _fg2Loc[1]);
	//_fgnode2->addChild(_fg2obj[1], -1);	// �[�J _fgnode2 ��

	



	//�إ߻�ê��

	_obstacle1[0] = new CObstacle();
	_obstacle1[0]->init("obstaclenode.csb", 3, Vec2(472, 370), *this);
	_obstacle1[0]->resetPos(2 * 2048, 370);
	_obstacle1[0]->setVis(false);

	_obstacle1[1] = new CObstacle();
	_obstacle1[1]->init("obstaclenode.csb", 3,  Vec2(1342, 370), *this);
	_obstacle1[1]->resetPos(2 * 2048, 370);
	_obstacle1[1]->setVis(false);


	_obstacle1[2] = new CObstacle();
	_obstacle1[2]->init("obstaclenode.csb", 3, Vec2(1836, 370), *this);
	_obstacle1[2]->resetPos(2 * 2048, 370);


	_obstacle1[3] = new CObstacle();
	_obstacle1[3]->init("obstaclenode.csb", 3, Vec2(2846, 370), *this);
	_obstacle1[3]->resetPos(2 * 2048, 370);

	_obstacle1[4] = new CObstacle();
	_obstacle1[4]->init("obstaclenode.csb", 3, Vec2(3718, 370), *this);
	_obstacle1[4]->resetPos(2 * 2048, 370);




	_obstacle2[0] = new CObstacle();
	_obstacle2[0]->init("obstaclenode.csb", 3, Vec2(990, 230), *this);
	_obstacle2[0]->resetPos(2 * 2048, 230);

	_obstacle2[1] = new CObstacle();
	_obstacle2[1]->init("obstaclenode.csb", 3, Vec2(2289, 230), *this);
	_obstacle2[1]->resetPos(2 * 2048, 230);

	_obstacle2[2] = new CObstacle();
	_obstacle2[2]->init("obstaclenode.csb", 3, Vec2(3325, 230), *this);
	_obstacle2[2]->resetPos(2 * 2048, 230);
}






void RunScene1::doStep(float dt)
{
	if (!_aniStop) {
		/*if (!once) {
			SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/opendoor.mp3", true);
			once = true;
		}*/
		if (_skip) {
			_pic105->setVisible(false);
			if (_opendoorAnimation_2->skip(dt)&&_opendoorAnimation_1->skip(dt)) {
				_aniStop = true;
				_skipSprite->setVisible(false);
				//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������
				SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/Tomorrow.mp3", true);
			}
		}
		else {
			if (!_cutPic) {
				if (_opendoorAnimation_1->doStep(dt)) {
					_cutPic = true;
				}
			}
			else {
				if (!_cutPic_2) {
					_picTimeCount += dt;
					if (_picTimeCount > 2.0f) {
						_pic105->setVisible(false);
						_cutPic_2 = true;
					}
				}
				else
				{
					if (_opendoorAnimation_2->doStep(dt)) {
					_aniStop = true;
					_skipSprite->setVisible(false);
					//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������
					SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/Tomorrow.mp3", true);
					}
				}
			}
		}
	}
	else
	{

	// �C��e���������� MOVESPEED ��PIXEL
		if (_bStart && !_bpause && !_bwin) {

			_t += dt;

			Point pt1 = _fgnode1->getPosition();
			pt1.x -= dt*MOVESPEED;
			if (pt1.x <= -1024) {
				// �N�D�����ʨ� +2048+1024 �A���������������W�L -1024 �h�������A�ҥH�O 2 * 2048 + pt1.x
				pt1.x = 2 * 2048 + pt1.x;
				// ��s�e��������ܤ��e
				_fg1obj[0]->resetObj();// _fg1obj[1]->resetObj();


				_obstacle1[0]->setVis(true);

				bool i = rand() % 2;
				_obstacle1[1]->setVis(i);
				_obstacle1[2]->setVis(!i);


				_trash[0]->resetObj();


			}

			else if (pt1.x > 1024 && pt1.x <= 2000) {
				_trash[0]->setstate(true);
			}

			_fgnode1->setPosition(pt1);		// �@��

			Point pt2 = _fgnode2->getPosition();
			pt2.x -= dt*MOVESPEED;
			if (pt2.x <= -1024) {
				pt2.x = 2 * 2048 + pt2.x;
				_fg2obj[0]->resetObj();// _fg2obj[1]->resetObj();


				bool i = rand() % 2;

				_obstacle1[3]->setVis(i);
				_obstacle2[1]->setVis(!i);

				_trash[1]->resetObj();

			}
			else if (pt2.x <= 2250) {
				_trash[1]->setstate(true);
			}
			_fgnode2->setPosition(pt2);		// �@��


			_runner->doStep(dt);			// �@��






			//_obstacle[0]->doStep(dt);       // 0~10s
			//_obstacle[1]->doStep(dt);	// 10s ��
			////if (_t > 10.0f) {
			////	_obstacle[1]->doStep(dt);	// 10s ��
			////}
			//if (_t > 30.0f) {
			//	_obstacle[2]->doStep(dt);	//15s ��
			//}


			//_obstacle[3]->doStep(dt);
			//_obstacle[4]->doStep(dt);


			_obstacle1[0]->doStep(dt);
			_obstacle1[1]->doStep(dt);
			_obstacle1[2]->doStep(dt);
			_obstacle1[3]->doStep(dt);
			_obstacle1[4]->doStep(dt);

			_obstacle2[0]->doStep(dt);
			_obstacle2[1]->doStep(dt);
			_obstacle2[2]->doStep(dt);

			if (_t >= 10.0f) {
				if (_trash[0]->getstate())	_trash[0]->doStep(dt);
				if (_trash[1]->getstate())	_trash[1]->doStep(dt);

			}

			////�]�w�}��
			if (_t >= 25.0f && _bikePt.x >= -100) {
				Size size = _bike->getContentSize();
				_bikePt = _bike->getPosition();
				_bikePt.x -= dt * 1000;
				_bike->setPosition(_bikePt);
				_bikeRect = Rect(_bikePt.x - size.width / 2, _bikePt.y - size.height / 2, size.width, size.height);
				int a = 1;
			}

			if (_t >= 35.0f && _runner->_ilife >= 0) {
				//label000->setString("win!");
				 _bwin = true;
				 _runPt = _runner->getPosition();

			}


			if (_runner->_ilife == 2) {
				_blur[0]->setVisible(true);
			}

			else if (_runner->_ilife == 1) {
				_blur[0]->setVisible(false);
				_blur[1]->setVisible(true);
			}

			else if (_runner->_ilife == 0) {
				//			label5->setString("gameover!");
				_blur[1]->setVisible(false);
				_blur[2]->setVisible(true);
				_brestart = true;
				_bStart = false;

			}


			//�P�_�I��===========================================================================
			if (_runner->getstate()) {
				if (_obstacle1[0]->getVis() && _obstacle1[0]->collision(_runner->getbaseRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 0);
					//label000->setString("True11!");
				}


				else if (_obstacle1[1]->getVis() && _obstacle1[1]->collision(_runner->getbaseRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 1);
					//label000->setString("True12!");
				}
				else if (_obstacle1[2]->getVis() && _obstacle1[2]->collision(_runner->getbaseRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 2);
					//label000->setString("True13!");
				}
				else if (_obstacle1[3]->getVis() && _obstacle1[3]->collision(_runner->getbaseRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 2);
					//				label000->setString("True13!");
				}

				else if (_obstacle1[4]->getVis() && _obstacle1[4]->collision(_runner->getbaseRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 2);
					//label000->setString("True13!");
				}


				else if (_trash[0]->collision(_runner->getRect())) {
					_runner->_ilife--;
					//label000->setString("trash!!!!!!!!!!");
					CCLOG("+++ %d\n", 1);
				}

				else if (_trash[1]->collision(_runner->getRect())) {
					_runner->_ilife--;
					//label000->setString("trash!!!!!!!!!!");
					CCLOG("+++ %d\n", 1);
				}

			}

			else if (!_runner->getstate()) {
				if (_obstacle2[0]->getVis() && _obstacle2[0]->collision(_runner->getRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 0);
					//				label000->setString("True11!");
				}


				else if (_obstacle2[1]->getVis() && _obstacle2[1]->collision(_runner->getRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 1);
					//				label000->setString("True12!");
				}
				else if (_obstacle2[2]->getVis() && _obstacle2[2]->collision(_runner->getRect())) {
					_runner->_ilife--;
					CCLOG("intersect %d\n", 2);
					//				label000->setString("True13!");
				}

			}


			else if (_bikeRect.intersectsRect(_runner->getRect()) && !_bcolBike) {
				_bcolBike = true;
				_runner->_ilife--;
				CCLOG("intersect %d\n", 10);
				//label000->setString("True0000!");
			}





			else {
				//label000->setString("False!");
			//	log("eee %d\n");
			}


		}

		if (_bwin) {
			
			_runPt.x += 30;
			_runner->setPosition(_runPt);
			if (_runPt.x > 2048) {
				this->unschedule(schedule_selector(RunScene1::doStep));
				SimpleAudioEngine::getInstance()->stopBackgroundMusic();
				Director::getInstance()->replaceScene(BR_Scene::createScene());
			}


		}
		
	}

}

void RunScene1::restart() {
	_bcolBike = false;
	_bpause = false;

	_bwin = false;
	_bStart = false;
	_t = 0.0f;

	//�������m
	_blur[0]->setVisible(false);
	_blur[1]->setVisible(false);
	_blur[2]->setVisible(false);


	_fgnode1->setPosition(1024, 0);
	_fgnode2->setPosition(2048 + 1024, 0);

	_fg1obj[0]->resetObj();// _fg1obj[1]->resetObj();
	_fg2obj[0]->resetObj();// _fg2obj[1]->resetObj();

	//�H�����m
	_runner->setPosition(320, 480);
	_runner->_ilife = 3;
	_runner->stop();


	//��ê�����m
	_bike->setPosition(2150, 500);

	_trash[0]->resetObj();
	_trash[1]->resetObj();

	//_obstacle[0]->resetPos(2248, 400);
	//_obstacle[1]->resetPos(2748, 400);
	//_obstacle[2]->resetPos(2348, 400);


	_obstacle1[0]->resetObj(true);
	_obstacle1[1]->resetObj(true);
	_obstacle1[2]->resetObj(true);
	_obstacle1[3]->resetObj(true);
	_obstacle1[4]->resetObj(true);

	_obstacle2[0]->resetObj(true);
	_obstacle2[1]->resetObj(true);
	_obstacle2[2]->resetObj(true);

	_obstacle1[0]->setVis(false);
	_obstacle1[1]->setVis(false);
}



bool RunScene1::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	Point touchLoc = pTouch->getLocation();
	if (_aniStop) {
		if (_brestart) {
			restart();
			_brestart = !_brestart;
			return true;

		}
		if (_pause->touchesBegin(touchLoc)) _bpause = !_bpause;
		if (!_bStart && !_brestart && !_bwin) {
			_bStart = true;
			_runner->setGo();
			//label000->setString("START!");
			return true;
		}

		//???�l��?
		if (_bStart && !_bpause) {
			startX = touchLoc.x;
			startY = touchLoc.y;
		}
	}

	




	return true;
}

void  RunScene1::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();
	_pause->touchesMoved(touchLoc);

}

void  RunScene1::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();

	if (!_aniStop) {
		if (_skipRect.containsPoint(touchLoc)) {
			_skip = true;
			log("skip");
		}
	}
	else {
		if (_bStart && !_bpause && !_bwin) {
			//???����?
			endX = touchLoc.x;
			endY = touchLoc.y;
			//�D?�l�O?����?���t
			float offsetX = endX - startX;
			float offsetY = endY - startY;
			if (fabs(offsetX)>fabs(offsetY)) {//���uX��V�OY��V�������q�j�p���P?
				if (offsetX>0) {
					//label->setString("Right!");
					_runner->_bfast = true;
				}
				else {
					//label->setString("Left!");
					_runner->_bslow = true;
				}
			}
			else {
				if (offsetY>0) {
					//label->setString("Up!");
					//_runner->_bjump = true;
					_runner->_bup = true;
				}
				else {
					//label->setString("Down!");
					//_runner->_bsquat = true;
					_runner->_bdown = true;
				}
			}

		}

		else if (_bpause && !_bwin) {
			if (_resetRect[0].containsPoint(touchLoc)) restart();
		}
		else if (_bwin) if (_resetRect[1].containsPoint(touchLoc)) restart();

		_pause->touchesEnded(touchLoc);
	}

	

	
}



void RunScene1::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_A) {
		this->unschedule(schedule_selector(RunScene1::doStep));
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->replaceScene(BR_Scene::createScene());
	}
}