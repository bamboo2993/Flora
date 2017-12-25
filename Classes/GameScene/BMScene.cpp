#include "BMScene.h"
#include "cocostudio/CocoStudio.h"

#define ROOT_NODE   "BMScene.csb"
#define CURRENT_SCENE   "BMScene.cpp"

#define BAG_OPEN_HEIGHT 150.0f
#define BAG_CLOSE_HEIGHT 250.0f

#define BACKGROUND_FRONT "GameScene/BMScene/labScene02.png"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;





Scene* BMScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = BMScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

BMScene::BMScene() {
	_bWalk = false; //detect if player is walking

	//�P�_ walk

	_bpickObj = false;
	
	_bbOnFire[0] =false;
	_bbOnFire[1] = false;
	_clear = false;
	for (size_t i = 0; i < 5; i++){
		_btouch[i] = false;
	}
	
	_ibagState = 0;

	_aniStop = false;
	_skip = false;

	for (size_t i = 0; i < 3; i++) {
		_touchSObj[i] = false;
		_openSObj[i] = false;

	}

	for (size_t i = 0; i < TALK_AREA; i++) {
		_touchTalk[i] = false;
		_openTalk[i] = false;

	}


	_btouchNode[0] = false;
	_btouchNode[1] = false;

	_bopenNode[0] = false;
	_bopenNode[1] = false;
}
BMScene::~BMScene()
{
	//xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);
	xmlBag::getInstance()->sortItems();

	//CBag::getInstance()->destroyInstance();
	xmlItem::getInstance()->destroyInstance();
	xmlTrigger::getInstance()->destroyInstance();
	//xmlBag::getInstance()->destroyInstance();

//	this->removeAllChildren();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool BMScene::init()
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BMScene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/bagItem.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("childhoodAni.plist");

	// ------------------------------------------------------------------------------------------------- 

	////�H Sprite �@���I�� ========================================================================
	Sprite *bkimage = Sprite::create(BACKGROUND_FRONT);  // �ϥ� create �禡,�����ɦW�Y�i
	bkimage->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2));
	this->addChild(bkimage, 100);

	_rootNode = CSLoader::createNode(ROOT_NODE);
	addChild(_rootNode);

	_bkBlur = (cocos2d::Sprite*)_rootNode->getChildByName("labScene_blur");
	this->addChild(_bkBlur, 200);

	//set blur node====================
	_eNode[0] = (cocos2d::Node*)_rootNode->getChildByName("FileNode_1");
	addChild(_eNode[0], 300);

	_eNode[1] = (cocos2d::Node*)_rootNode->getChildByName("FileNode_2");
	addChild(_eNode[1], 300);

	//skipBtn
	_skipSprite = (cocos2d::Sprite*)_rootNode->getChildByName("skip");
	Size size = _skipSprite->getContentSize();
	Point pos = _skipSprite->getPosition();
	_skipRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
	this->addChild(_skipSprite,20002);

	

	// set player==============================================================

	_player = new CPlayer("GP_stand01.png", "GP_stand02.png", *this);
	_player->setAnimation("Animation/GPanim.plist");
	_player->setPosition(Point(1030, 725));
	_player->setRect();




	//set lightbox (experimental procedure) =================================================================
	_procedure = CLightbox::create();
	_procedure->init(_rootNode, "detect_procedure", "GameScene/BMScene/procedure.png");
	this->addChild(_procedure, 10000);



	//trigger====
	_pTrigger = new CTrigger[10]; 

	_pbeakerRect = new Rect[2];
	_pbowlRect = new Rect;

	//set objects=========================================
	SetObject();

	
	_xmlscene = new xmlScene("./res/xml/xmlfile_BMScene.xml");
	_xmlscene->parseXML(_rootNode, CURRENT_SCENE, _pTrigger);
	_xmlscene->parseNodeXML(_eNode[0], "eN0");


	// mix=======================================


	_mixA = new CMix(_rootNode);
	_mixA->init(17, 25); // initial setting
	_mixA->SetCombination(18,0, 21,0, 22, 1); //step1: mix red + yellow = orange
	_mixA->SetCombination(18,0, 20,0, 23, 1); // step1: mix red + blue = purple


	_mixA->SetCombination(22,1, 32,1, 24, 2); //step2: mix orange + purple = brown
	_mixA->SetCombination(23,1, 31,1, 24, 2);



	_mixA->SetCombination(24,1, 13,1, 24, 3); // step3: mix brown + red herb
	_mixA->SetCombination(24,1, 8,0, 24, 4); // step4: add water
	_mixA->SetCombination(24,1, 9,0, 24, 5); // mix with glassrod


	_mixB = new CMix(_rootNode);
	_mixB->init(26, 34);
	_mixB->SetCombination(27,0, 30,0, 31, 1); //step1: mix red + yellow = orange
	_mixB->SetCombination(27,0, 29,0, 32, 1); // step1: mix red + blue = purple


	_mixB->SetCombination(31,1, 23,1, 33, 2);
	_mixB->SetCombination(32,1, 22,1, 33, 2);



	_mixB->SetCombination(33, 1, 13, 1, 33, 3);
	_mixB->SetCombination(33, 1, 8, 0, 33, 4);
	_mixB->SetCombination(33, 1, 9, 0, 33, 5);


	_grind = new CMix(_rootNode);
	_grind->init(12,16,false);

	//set bag =================================================================


	CBag::getInstance()->Init(Point(172, -115), _pTrigger);
	this->addChild(CBag::getInstance(), 1000);

	//Animation
	_childhoodAni = new AniScene();
	_childhoodAni->init(Point(visibleSize.width / 2, origin.y + visibleSize.height / 2.0), *this, 10, "0");


	//key
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(BMScene::keyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	reset();

	// ���ĻP���� --------------------------------------------------------------------------------

	SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/memories.mp3", true);
	//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //�|����@

	////SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������

	_pour = (cocostudio::ComAudio *)_rootNode->getChildByName("pour")->getComponent("pour");
	_powder = (cocostudio::ComAudio *) _rootNode->getChildByName("powder")->getComponent("powder");
	_mixing = (cocostudio::ComAudio *) _rootNode->getChildByName("mix")->getComponent("mix");
	_grinding = (cocostudio::ComAudio *) _rootNode->getChildByName("grind")->getComponent("grind");
	_debranch = (cocostudio::ComAudio *) _rootNode->getChildByName("debranch")->getComponent("debranch");

	//-------------------------------------------------------------------------------------------------

	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(BMScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(BMScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(BMScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(BMScene::doStep));
	return true;
}





void BMScene::SetObject() {
	//set objects

	_pTrigger[0].create();
	_pTrigger[0].Init("E_red", _rootNode, true, 1, _eNode[0]);

	_pTrigger[1].create();
	_pTrigger[1].Init("E_green", _rootNode, true, 1, _eNode[0]);

	_pTrigger[2].create();
	_pTrigger[2].Init("E_blue", _rootNode, true, 1, _eNode[0]);

	_pTrigger[3].create();
	_pTrigger[3].Init("E_yellow", _rootNode, true, 1, _eNode[0]);

	_pTrigger[4].create();
	_pTrigger[4].Init("water", _rootNode, true);

	_pTrigger[5].create();
	_pTrigger[5].Init("glassrod", _rootNode, true);


	_pTrigger[6].create();
	_pTrigger[6].Init("herb2", _rootNode, false, 1, _eNode[1]);

	_pTrigger[7].create();
	_pTrigger[7].Init("herb1", _rootNode, false, 1, _eNode[1]);

	_pTrigger[8].create();
	_pTrigger[8].Init("herb3", _rootNode, false, 1, _eNode[1]);

	_pTrigger[9].create();
	_pTrigger[9].Init("herb4", _rootNode, false, 1, _eNode[1]);
	this->addChild(_pTrigger);




	_detect[0] = (cocos2d::Sprite*)_rootNode->getChildByName("detect_syrup");
	Size size = _detect[0]->getContentSize();
	Point pos = _detect[0]->getPosition();
	_detectRect[0] = Rect(pos.x - size.height / 2, pos.y - size.width / 2, size.height, size.width);

	_detect[1] = (cocos2d::Sprite*)_rootNode->getChildByName("detect_herb");
	size = _detect[1]->getContentSize();
	pos = _detect[1]->getPosition();
	_detectRect[1] = Rect(pos.x - size.height / 2, pos.y - size.width / 2, size.height, size.width);


	Sprite* a = (cocos2d::Sprite*) _eNode[0]->getChildByName("E_node1");
	size = a->getContentSize();
	pos = a->getPosition();
	_offRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	Sprite* b = (cocos2d::Sprite*) _eNode[1]->getChildByName("E_node2");
	size = b->getContentSize();
	pos = b->getPosition();
	_offRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);




	_trash = (cocos2d::Sprite*)_rootNode->getChildByName("detect_trash");
	size = _trash->getContentSize();
	pos = _trash->getPosition();
	_trashRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);



	//set talk area
	a = (cocos2d::Sprite*)_rootNode->getChildByName("detect_book_0");
	size = a->getContentSize();
	pos = a->getPosition();
	size.height = size.height/2;
	size.width = size.width*1.3f;
	_talkRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	pos.x += 445;
	_talkRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("deetect_plant_2");
	size = a->getContentSize();
	pos = a->getPosition();
	size.height = size.height *1.3f;
	_talkRect[2] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("detect_mouse_3");
	size = a->getContentSize();
	pos = a->getPosition();
	_talkRect[3] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("detect_mouse_4");
	size = a->getContentSize();
	pos = a->getPosition();
	size.height = size.height * 0.6f;
	size.width = size.width * 0.7f;
	_talkRect[4] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("detect_shelf_5");
	size = a->getContentSize();
	pos = a->getPosition();
	size.height = size.height * 2;
	size.width = size.width * 1.7f;
	_talkRect[5] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	a = (cocos2d::Sprite*)_rootNode->getChildByName("detect_Bboard_6");
	size = a->getContentSize();
	pos = a->getPosition();
	size.height = size.height * 2.1f;
	size.width = size.width * 0.8f;
	_talkRect[6] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);


	// reset button===============================================
	_reset = (cocos2d::Sprite*)_rootNode->getChildByName("reset");
	size = _reset->getContentSize();
	pos = _reset->getPosition();
	_resetRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.height, size.width);




	_syrup[0] = (cocos2d::Sprite*)_rootNode->getChildByName("red");
	_syrup[1] = (cocos2d::Sprite*)_rootNode->getChildByName("green");
	_syrup[2] = (cocos2d::Sprite*)_rootNode->getChildByName("blue");
	_syrup[3] = (cocos2d::Sprite*)_rootNode->getChildByName("yellow");


	_alcoholLamp[0] = (cocos2d::Sprite*)_rootNode->getChildByName("alcohol_lamp01");
	_alcoholLamp[1] = (cocos2d::Sprite*)_rootNode->getChildByName("alcohol_lamp02");
	this->addChild(_alcoholLamp[0], 150);
	this->addChild(_alcoholLamp[1], 150);

	size = _alcoholLamp[1]->getContentSize();
	pos = _alcoholLamp[1]->getPosition();
	_alcoholLampRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);



	_beakerA[0] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_red");
	this->addChild(_beakerA[0], 150);

	_beakerA[1] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_green");
	this->addChild(_beakerA[1], 150);
	_beakerA[2] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_blue");
	this->addChild(_beakerA[2], 150);
	_beakerA[3] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_yellow");
	this->addChild(_beakerA[3], 150);
	_beakerA[4] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_orange");
	this->addChild(_beakerA[4], 150);
	_beakerA[5] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_purple");
	this->addChild(_beakerA[5], 150);
	_beakerA[6] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_brownSh");
	this->addChild(_beakerA[6], 150);
	_beakerA[7] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_false");
	this->addChild(_beakerA[7], 150);
	_beakerA[8] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker");
	this->addChild(_beakerA[8], 150);
	size = _beakerA[8]->getContentSize();
	pos = _beakerA[8]->getPosition();
	_pbeakerRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_beakerB[0] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_red01");
	this->addChild(_beakerB[0], 150);
	_beakerB[1] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_green01");
	this->addChild(_beakerB[1], 150);
	_beakerB[2] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_blue01");
	this->addChild(_beakerB[2], 150);
	_beakerB[3] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_yellow01");
	this->addChild(_beakerB[3], 150);
	_beakerB[4] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_orange01");
	this->addChild(_beakerB[4], 150);
	_beakerB[5] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_purple01");
	this->addChild(_beakerB[5], 150);
	_beakerB[6] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_brownSh01");
	this->addChild(_beakerB[6], 150);
	_beakerB[7] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_false01");
	this->addChild(_beakerB[7], 150);
	_beakerB[8] = (cocos2d::Sprite*)_rootNode->getChildByName("beaker01");
	this->addChild(_beakerB[8], 150);
	size = _beakerB[8]->getContentSize();
	pos = _beakerB[8]->getPosition();
	_pbeakerRect[1] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);





	_beakerComplete = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_brown");
	this->addChild(_beakerComplete, 150);
	_beakerFailed = (cocos2d::Sprite*)_rootNode->getChildByName("beaker_false02");
	this->addChild(_beakerFailed, 150);

	size = _beakerComplete->getContentSize();
	pos = _beakerComplete->getPosition();
	_beakerCompleteRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_bowl[0] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl");
	this->addChild(_bowl[0], 150);
	_bowl[1] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_red");
	this->addChild(_bowl[1], 150);
	_bowl[2] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_green");
	this->addChild(_bowl[2], 150);
	_bowl[3] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_darkgreen");
	this->addChild(_bowl[3], 150);
	_bowl[4] = (cocos2d::Sprite*)_rootNode->getChildByName("bowl_yellow");
	this->addChild(_bowl[4], 150);
	size = _bowl[4]->getContentSize();
	pos = _bowl[4]->getPosition();
	_pbowlRect[0] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//xmlItem::getInstance()->setTargetRectXML(7, _pbowlRect[0]);


	//_pRect = &_bowlRect;
}


void BMScene::doStep(float dt)
{
	if (!_aniStop) {
		if (_skip) {
			if (_childhoodAni->skip(dt)) {
				_aniStop = true;
				//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������
				SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/lab_bgm.mp3", true);
			}
		}
		else {
			if (_childhoodAni->doStep(dt)) {	
				_childhoodAni->setVis(false);
				_aniStop = true;
				//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������
				SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/lab_bgm.mp3", true);
			}

		}
		
	}
	else
	{
		_skipSprite->setVisible(false);
		_mixA->doStep(dt);
		_mixB->doStep(dt);
		_grind->doStep(dt);



		//walk===================================
		//////�u��b�]�w�d��=============================
		
		if (_touchSObj[0] || _touchTalk[0]) {
			_player->Walk(Vec2(1317.35f, 580.0f));
			_player->go(Vec2(1317.35f, 580.0f));
			if (_player->Walk(Vec2(1317.35f, 580.0f)) == false) {
				_bWalk = 0;

				//pick up obj
				if (_touchSObj[0]) {
					_touchSObj[0] = !_touchSObj[0];
					_openSObj[0] = !_openSObj[0];
				}
				else {
					_touchTalk[0] = !_touchTalk[0];
					_openTalk[0] = !_openTalk[0];
				}

				
				//log("show detect");
			}

		}

		else if (_touchSObj[1] || _btouchNode[0] || _touchTalk[1]) {
			_player->Walk(Vec2(1703.04f, 580.0f));
			_player->go(Vec2(1703.04f, 580.0f));
			if (_player->Walk(Vec2(1703.04f, 580.0f)) == false) {
				_bWalk = 0;

				//pick up obj
				if (_btouchNode[0]) {

					_eNode[0]->setVisible(true);

					_bopenNode[0] = !_bopenNode[0];
					_btouchNode[0] = !_btouchNode[0];
					_bkBlur->setVisible(true);
				}
				else if (_touchTalk[1]) {
					_touchTalk[1] = !_touchTalk[1];
					_openTalk[1] = !_openTalk[1];
				}
				else {
					_touchSObj[1] = !_touchSObj[1];
					_openSObj[1] = !_openSObj[1];
				}


				//log("show detect");
			}

		}


		else if (_touchSObj[2]) {
			_player->Walk(Vec2(1200.05f, 141.24f));
			_player->go(Vec2(1200.05f, 141.24f));
			if (_player->Walk(Vec2(1200.05f, 141.24f)) == false) {
				_bWalk = 0;

				//pick up obj

				_touchSObj[2] = !_touchSObj[2];
				_openSObj[2] = !_openSObj[2];
				//log("show detect");
			}

		}





		else if (_btouchNode[1] || _touchTalk[4]) {
			_player->Walk(Vec2(702.90f, 141.24f));
			_player->go(Vec2(702.90f, 141.24f));
			if (_player->Walk(Vec2(702.90f, 141.24f)) == false) {
				_bWalk = 0;

				if (_btouchNode[1]) {
					//pick up obj
					_bkBlur->setVisible(true);
					_eNode[1]->setVisible(true);

					_bopenNode[1] = !_bopenNode[1];
					_btouchNode[1] = !_btouchNode[1];
				}

				else {
					_touchTalk[4] = !_touchTalk[4];
					_openTalk[4] = !_openTalk[4];
				}
				
				//log("show detect");
			}

		}
		
		else if (_touchTalk[2]) {
			_player->Walk(Vec2(1703.04f, _talkRect[2].getMinY()));
			_player->go(Vec2(1703.04f, _talkRect[2].getMinY()));
			if (_player->Walk(Vec2(1703.04f, _talkRect[2].getMinY())) == false) {
				_bWalk = 0;

				//pick up obj

				_touchTalk[2] = !_touchTalk[2];
				_openTalk[2] = !_openTalk[2];
				//log("show detect");
			}

		}

		else if (_touchTalk[3]) {
			_player->Walk(Vec2(1703.04f, _talkRect[3].getMinY()));
			_player->go(Vec2(1703.04f, _talkRect[3].getMinY()));
			if (_player->Walk(Vec2(1703.04f, _talkRect[3].getMinY())) == false) {
				_bWalk = 0;

				//pick up obj

				_touchTalk[3] = !_touchTalk[3];
				_openTalk[3] = !_openTalk[3];
				//log("show detect");
			}

		}
		else if (_touchTalk[5]) {
			_player->Walk(Vec2(_talkRect[5].getMaxX(), _talkRect[3].getMinY()));
			_player->go(Vec2(_talkRect[5].getMaxX(), _talkRect[3].getMinY()));
			if (_player->Walk(Vec2(_talkRect[5].getMaxX(), _talkRect[3].getMinY())) == false) {
				_bWalk = 0;

				//pick up obj

				_touchTalk[5] = !_touchTalk[5];
				_openTalk[5] = !_openTalk[5];
				//log("show detect");
			}

		}

		else if (_touchTalk[6]) {
			_player->Walk(Vec2(_talkRect[5].getMaxX(), _talkRect[5].getMidY()));
			_player->go(Vec2(_talkRect[5].getMaxX(), _talkRect[5].getMidY()));
			if (_player->Walk(Vec2(_talkRect[5].getMaxX(), _talkRect[5].getMidY())) == false) {
				_bWalk = 0;

				//pick up obj

				_touchTalk[6] = !_touchTalk[6];
				_openTalk[6] = !_openTalk[6];
				//log("show detect");
			}

		}


		else {
			_player->Stop();
		}


		//// pick up obj ==========================
		PickObject(dt);

		if (_clear) {
			CBag::getInstance()->reset();
			this->unschedule(schedule_selector(BMScene::doStep));
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->replaceScene(RunScene1::createScene());
		}
	}
}

void BMScene::PickObject(float dt) {

	// pick up obj ==========================

	if (_openSObj[0]) {
		_pTrigger[4].doStep(dt);

		//fly to bag==========
		if (CBag::getInstance()->Canfly()) {
			//create the corresponding item in bag
			if (_pTrigger[4].GetAddToBag() && !_pTrigger[4].GetPicked()) {
				CBag::getInstance()->AddObj("B_water.png", 2, true, _pbeakerRect);
				CBag::getInstance()->Fly(_pTrigger[4].GetPos(), "B_water.png");
				_pTrigger[4].SetAddToBag(false);
				_pTrigger[4].SetPicked(true);



			}
		}
		_openSObj[0] = false;
	}

	else if (_openSObj[1]) {
		_pTrigger[5].doStep(dt);

		//fly to bag==========
		if (_pTrigger[5].GetAddToBag() && !_pTrigger[5].GetPicked()) {
			CBag::getInstance()->AddObj("B_glassrod.png", 2, true, _pbeakerRect);
			CBag::getInstance()->Fly(_pTrigger[5].GetPos(), "B_glassrod.png");
			_pTrigger[5].SetAddToBag(false);
			_pTrigger[5].SetPicked(true);


		}
		_openSObj[1] = false;
	}

	else if (_openSObj[2]) {
		_procedure->doStep(dt);
		_openSObj[2] = false;
	}
		

	else if (_bopenNode[0]) {

		_pTrigger[0].doStep(dt);
		_pTrigger[1].doStep(dt);
		_pTrigger[2].doStep(dt);
		_pTrigger[3].doStep(dt);

		//fly to bag==========
		if (CBag::getInstance()->Canfly()) {
			//create the corresponding item in bag
			if (_pTrigger[0].GetAddToBag() && !_pTrigger[0].GetPicked()) {

				CBag::getInstance()->AddObj("B_red.png", 2, true, _pbeakerRect);
				CBag::getInstance()->Fly(_pTrigger[4].GetPos(), "B_red.png");
				_pTrigger[0].SetAddToBag(false);
				_pTrigger[0].SetPicked(true); // if the object is picked and added into the bag


				_syrup[0]->setVisible(false);
				_xmlscene->editItemState("red", false, _rootNode, 0, 35);
			}

			else if (_pTrigger[1].GetAddToBag() && !_pTrigger[1].GetPicked()) {
				CBag::getInstance()->AddObj("B_green.png", 2, true, _pbeakerRect);
				CBag::getInstance()->Fly(_pTrigger[4].GetPos(), "B_green.png");
				_pTrigger[1].SetAddToBag(false);
				_pTrigger[1].SetPicked(true);


				_syrup[1]->setVisible(false);
				_xmlscene->editItemState("green", false, _rootNode, 0, 35);
			}

			else if (_pTrigger[2].GetAddToBag() && !_pTrigger[2].GetPicked()) {
				CBag::getInstance()->AddObj("B_blue.png", 2, true, _pbeakerRect);
				CBag::getInstance()->Fly(_pTrigger[4].GetPos(), "B_blue.png");
				_pTrigger[2].SetAddToBag(false);
				_pTrigger[2].SetPicked(true);


				_syrup[2]->setVisible(false);
				_xmlscene->editItemState("blue", false, _rootNode, 0, 35);
			}

			else if (_pTrigger[3].GetAddToBag() && !_pTrigger[3].GetPicked()) {

				CBag::getInstance()->AddObj("B_yellow.png", 2, true, _pbeakerRect);
				CBag::getInstance()->Fly(_pTrigger[4].GetPos(), "B_yellow.png");
				_pTrigger[3].SetAddToBag(false);
				_pTrigger[3].SetPicked(true);


				_syrup[3]->setVisible(false);
				_xmlscene->editItemState("yellow", false, _rootNode, 0, 35);
			}

		}

		
		
	}



	else if (_bopenNode[1]) {
		
		_pTrigger[6].doStep(dt);
		_pTrigger[7].doStep(dt);
		_pTrigger[8].doStep(dt);
		_pTrigger[9].doStep(dt);
		//log("node is open");


		//fly to bag==========
		if (CBag::getInstance()->Canfly()) {
			//create the corresponding item in bag
			if (_pTrigger[6].GetAddToBag() && !_pTrigger[6].GetPicked()) {
				CBag::getInstance()->AddObj("B_herbR.png", 1, false, _pbowlRect, true);
				CBag::getInstance()->Fly(_pTrigger[6].GetPos(), "B_herbR.png");
				_pTrigger[6].SetAddToBag(false);
				_pTrigger[6].SetPicked(true);

				//sound effect
				_debranch->playEffect();
			}
			else if (_pTrigger[7].GetAddToBag() && !_pTrigger[7].GetPicked()) {
				CBag::getInstance()->AddObj("B_herbG.png", 1, false, _pbowlRect, true);
				CBag::getInstance()->Fly(_pTrigger[7].GetPos(), "B_herbG.png");
				_pTrigger[7].SetAddToBag(false);
				_pTrigger[7].SetPicked(true);

				//sound effect
				_debranch->playEffect();
			}
			else if (_pTrigger[8].GetAddToBag() && !_pTrigger[8].GetPicked()) {
				CBag::getInstance()->AddObj("B_herbDG.png", 1, false, _pbowlRect, true);
				CBag::getInstance()->Fly(_pTrigger[8].GetPos(), "B_herbDG.png");
				_pTrigger[8].SetAddToBag(false);
				_pTrigger[8].SetPicked(true);


				//sound effect
				_debranch->playEffect();
			}

			else if (_pTrigger[9].GetAddToBag() && !_pTrigger[9].GetPicked()) {
				log("herb");
				CBag::getInstance()->AddObj("B_herbY.png", 1, false, _pbowlRect, true);
				CBag::getInstance()->Fly(_pTrigger[9].GetPos(), "B_herbY.png");
				_pTrigger[9].SetAddToBag(false);
				_pTrigger[9].SetPicked(true);

				//			xmlTrigger::getInstance()->setTriggerStateXML(CURRENT_SCENE, 9, _pTrigger[9].GetPicked(), _pTrigger[9].GetAddToBag());

				//sound effect
				_debranch->playEffect();
			}
		}
		

	}

	else if (_openTalk[0]) {
		_player->Talk("dialoge/BM/BM_bookcase01.png", true);
		_player->SetIsTalking(true);
		_openTalk[0] = false;
	}

	else if (_openTalk[1]) {
		_player->Talk("dialoge/BM/BM_bookcase01.png", false);
		_player->SetIsTalking(true);
		_openTalk[1] = false;
	}

	else if (_openTalk[2]) {
		_player->Mirror(true);
		const char* x;
		if (_talkContent) x = "dialoge/BM/BM_flower01.png";
		else x = "dialoge/BM/BM_flower02.png";
		_player->Talk(x, false);
		_player->SetIsTalking(true);
		_openTalk[2] = false;
	}

	else if (_openTalk[3]) {
		_player->Mirror(true);
		_player->SetFront(true);
		const char* x;
		if (_talkContent) x = "dialoge/BM/BM_mice02.png";
		else x = "dialoge/BM/BM_mice01.png";
		_player->Talk(x, false);
		_player->SetIsTalking(true);
		_openTalk[3] = false;
	}

	else if (_openTalk[4]) {
		_player->SetFront(true);
		const char* x;
		if (_talkContent) x = "dialoge/BM/BM_mice02.png";
		else x = "dialoge/BM/BM_mice01.png";
		_player->Talk(x, true);
		_player->SetIsTalking(true);
		_openTalk[4] = false;
	}

	else if (_openTalk[5]) {
		_player->SetFront(true);
		_player->Mirror(false);
		_player->Talk("dialoge/BM/BM_bookcase01.png", true);
		_player->SetIsTalking(true);
		_openTalk[5] = false;
	}

	else if (_openTalk[6]) {
		_player->Mirror(false);
		const char* x;
		if (_talkContent==0) x = "dialoge/BM/BM_blackboard01.png";
		else if (_talkContent ==1) x = "dialoge/BM/BM_blackboard02.png";
		else if (_talkContent == 2) x = "dialoge/BM/BM_blackboard03.png";
		_player->Talk(x, true);
		_player->SetIsTalking(true);

		_openTalk[6] = false;
	}


}


void BMScene::reset() {
	_mixA->Clear();
	_mixB->Clear();
	_grind->Clear();

	CBag::getInstance()->reset();

	// reset scene===============================
	_bWalk = false; //detect if player is walking

	//�P�_ walk
	

	_bpickObj = false;


	_eNode[0]->setVisible(false);
	_eNode[1]->setVisible(false);

	_procedure->setVisible(false);

	for (size_t i = 0; i < 4; i++) {
		_xmlscene->editItemState(i, true, _rootNode);

	}
	for (size_t i = 4; i < 8; i++) {
		_xmlscene->editItemState(i, true, _eNode[0]);
	}
	_xmlscene->editItemState(8, true, _rootNode);
	_xmlscene->editItemState(9, true, _rootNode);

	_xmlscene->editItemState(35, false, _rootNode);
	_xmlscene->editItemState(36, false, _rootNode);

	for (size_t i = 0; i < 10; i++) {
		_pTrigger[i].reset();

	}
	xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);
	
	_bbOnFire[0] = false;
	_bbOnFire[1] = false;
	_btouch[0] = false;
	_btouch[1] = false;
	_btouch[2] = false;
	_btouch[3] = false;

	_clear = false;
	_ibagState = 0;
}


bool BMScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	_touchLoc = pTouch->getLocation();
	_prePos = _touchLoc;

	Rect xx = Rect(1900.0f,1440.0f,140.0f,98.0f);
	if (xx.containsPoint(_touchLoc)) {
		xmlTrigger::getInstance()->updateTriggerXML(CURRENT_SCENE, _pTrigger);
		xmlBag::getInstance()->sortItems();
	}



	if (!_procedure->GetOpen() && !_clear && !_player->GetIsTalking()) {
		//swipe gesture
		_startX = _touchLoc.x;
		_startY = _touchLoc.y;


		//drag obj from the bg scene==================================
		if (!_bopenNode[0] && !_bopenNode[1] && !_bWalk && !CBag::getInstance()->LightboxState()) {

			//dragging beaker-----------
			if (_pbeakerRect[0].containsPoint(_touchLoc)) {  

				_btouch[0] = true;
				_itempos = _beakerA[8]->getPosition();
				for (size_t i = 0; i < 9; i++) {
					_beakerA[i]->setPosition(_itempos);
				}


			}
			else if (_pbeakerRect[1].containsPoint(_touchLoc)) {   //dragging beaker-----------

				_btouch[1] = true;
				_itempos = _beakerB[8]->getPosition();
				for (size_t i = 0; i < 9; i++) {
					_beakerB[i]->setPosition(_itempos);
				}


			}
			//dragging bowl-----------
			else if (_pbowlRect[0].containsPoint(_touchLoc)) {

				_btouch[2] = true;
				_itempos = _bowl[4]->getPosition();
				for (size_t i = 0; i < 4; i++) {
					_bowl[i]->setPosition(_itempos);
				}
			}
			//dragging beaker on flame----------
			else if (_bbOnFire[0] || _bbOnFire[1]) {
				if (_beakerCompleteRect.containsPoint(_touchLoc)) {
					_btouch[3] = true;
					_itempos = _beakerComplete->getPosition();

					_beakerComplete->setPosition(_itempos);
					_beakerFailed->setPosition(_itempos);


				}
			}
		
		}


		if (_ibagState) { //when bag is open
			//use items in bag===========================================
			CBag::getInstance()->touchesBegan(_touchLoc);

	
		}


	}


	return true;
}


void  BMScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{

	_touchLoc = pTouch->getLocation();

	if (!_procedure->GetOpen() && !_clear && !_player->GetIsTalking()) {
		//use items in bag===========================================
		if (_ibagState) { //when bag is open
			CBag::getInstance()->touchesMoved(_touchLoc);
		}


		if (!CBag::getInstance()->LightboxState()) {
			//dragging beakers and bowl in scene------------
			if (_btouch[0]) {
				for (size_t i = 0; i < 9; i++) {
					_beakerA[i]->setPosition(_beakerA[i]->getPosition() + (_touchLoc - _prePos));
				}

				_prePos = _touchLoc;

			}

			else if (_btouch[1]) {
				for (size_t i = 0; i < 9; i++) {
					_beakerB[i]->setPosition(_beakerB[i]->getPosition() + (_touchLoc - _prePos));
				}

				_prePos = _touchLoc;

			}

			else if (_btouch[2]) {
				for (size_t i = 0; i < 4; i++) {
					_bowl[i]->setPosition(_bowl[i]->getPosition() + (_touchLoc - _prePos));
				}

				_prePos = _touchLoc;

			}

			else if (_btouch[3]) {

				_beakerComplete->setPosition(_beakerComplete->getPosition() + (_touchLoc - _prePos));
				_beakerFailed->setPosition(_beakerFailed->getPosition() + (_touchLoc - _prePos));
				_prePos = _touchLoc;

			}
		}



	}


	
	
	
}

void  BMScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{

	_touchLoc = pTouch->getLocation();
	
	if (!_aniStop) {
		if (_skipRect.containsPoint(_touchLoc)) {
			_skip = true;
		}
	}

	else if (!_clear &&_aniStop) {
		//swipe gesture
		float offsetX = _touchLoc.x - _startX;
		float offsetY = _touchLoc.y - _startY;



		if (!_procedure->GetOpen() && !_player->GetIsTalking()) {


			if (_ibagState != 2) {

				// [WALK + PICK OBJECT]===================
				if (offsetX == 0 && offsetY == 0 && !CBag::getInstance()->LightboxState()) { // when screen tapped
					if (_touchLoc.y > 227) {

						////player walk =====================================================

						//��j��S�}--------------
						if (!_bopenNode[0] && !_bopenNode[1]) {
							//�S�����m--------------
							if (!_resetRect.containsPoint(_touchLoc)) {
								//====================================
								//detect for [water], [glass rod], [experimental procedure]------
								if (_pTrigger[4].touchesBegan(_touchLoc)) {
									_touchSObj[0] = true;
									_bWalk = 1;
									_player->setPreviousPosition();

								}

								else {
									_openSObj[0] = false;
									_touchSObj[0] = false;

								}

								if (_pTrigger[5].touchesBegan(_touchLoc)) {
									_touchSObj[1] = true;
									_bWalk = 1;
									_player->setPreviousPosition();


								}

								else {
									_openSObj[1] = false;
									_touchSObj[1] = false;
								}

								if (_procedure->TouchBegan(_touchLoc)) {
									_touchSObj[2] = true;
									_bWalk = 1;
									_player->setPreviousPosition();

								}
								else {
									_openSObj[2] = false;
									_touchSObj[2] = false;
								}

								if (_talkRect[0].containsPoint(_touchLoc)) {
									_touchTalk[0] = true;
									_bWalk = 1;
									_player->setPreviousPosition();

								}
								else {
									_openTalk[0] = false;
									_touchTalk[0] = false;
								}


								if (_talkRect[1].containsPoint(_touchLoc)) {
									_touchTalk[1] = true;
									_bWalk = 1;
									_player->setPreviousPosition();

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


								if (_talkRect[3].containsPoint(_touchLoc)) {
									_touchTalk[3] = true;
									_bWalk = 1;
									_player->setPreviousPosition();
									_talkContent = rand() % 2;
								}
								else {
									_openTalk[3] = false;
									_touchTalk[3] = false;
								}


								if (_talkRect[5].containsPoint(_touchLoc)) {
									_touchTalk[5] = true;
									_bWalk = 1;
									_player->setPreviousPosition();

								}
								else {
									_openTalk[5] = false;
									_touchTalk[5] = false;
								}

								if (_talkRect[6].containsPoint(_touchLoc)) {
									_touchTalk[6] = true;
									_bWalk = 1;
									_player->setPreviousPosition();
									_talkContent = rand() % 3;
								}
								else {
									_openTalk[6] = false;
									_touchTalk[6] = false;
								}


							}
							else {
								// reset button=========================
								//reset();
							}

						}

						//��j��[0]�}---------------------
						if (_bopenNode[0]) {
							_pTrigger[0].touchesBegan(_touchLoc);
							_pTrigger[1].touchesBegan(_touchLoc);
							_pTrigger[2].touchesBegan(_touchLoc);
							_pTrigger[3].touchesBegan(_touchLoc);

							if (!_offRect[0].containsPoint(_touchLoc)) {
								_bopenNode[0] = !_bopenNode[0];
								_eNode[0]->setVisible(false);
								_bkBlur->setVisible(false);
								log("close detect");
							}
						}
						//��j��[1]�}---------------------
						else if (_bopenNode[1]) {
							_pTrigger[6].touchesBegan(_touchLoc);
							_pTrigger[7].touchesBegan(_touchLoc);
							_pTrigger[8].touchesBegan(_touchLoc);
							_pTrigger[9].touchesBegan(_touchLoc);

							if (!_offRect[1].containsPoint(_touchLoc)) {
								_bopenNode[1] = !_bopenNode[1];
								_eNode[1]->setVisible(false);
								_bkBlur->setVisible(false);
								log("close detect");
							}

						}


						//0
						if (!_bopenNode[0] && _detectRect[0].containsPoint(_touchLoc)) {

							_btouchNode[0] = true;
							_bWalk = 1;
							_player->setPreviousPosition();

							log("touched detect");
						}
						else if (!_bopenNode[0] && !_detectRect[0].containsPoint(_touchLoc)) {
							_btouchNode[0] = false;
						}

						//1
						if (!_bopenNode[1] && _detectRect[1].containsPoint(_touchLoc)) {

							_btouchNode[1] = true;
							_bWalk = 1;
							_player->setPreviousPosition();

							log("touched detect node2");
						}
						else if (!_bopenNode[1] && !_detectRect[1].containsPoint(_touchLoc)) {
							_btouchNode[1] = false;
						}
					}

					else {
						if (!_ibagState) {
							//��j��S�}--------------
							if (!_bopenNode[0] && !_bopenNode[1]) {
								if (_talkRect[4].containsPoint(_touchLoc)) {
									_touchTalk[4] = true;
									_bWalk = 1;
									_player->setPreviousPosition();
									_talkContent = rand() % 2;
								}
								else {
									_openTalk[4] = false;
									_touchTalk[4] = false;
								}
							}
						}
					}

				}

				// drag beakerA=====================================================
				if (_btouch[0] && !_bbOnFire[0]) {
					for (size_t i = 0; i < 9; i++) {
						_beakerA[i]->setPosition(_itempos);
					}

					// to beakerB.......
					if (_pbeakerRect[1].containsPoint(_touchLoc)) {


						if (_mixA->getCurrentObj() != 17) {
							// add pour liquid sound
							_pour->playEffect();
							_mixB->mixing(_mixA->getCurrentObj(), 0);
						}
						_mixA->Clear();

					}

					// to trash.......
					else if (_trashRect.containsPoint(_touchLoc) && _mixA->getCurrentObj() != 17) {
						// add pour liquid sound
						_pour->playEffect();
						// clear beaker
						_mixA->Clear();
					}

					// to alcohol lamp
					else if (_alcoholLampRect.containsPoint(_touchLoc)) {

						if (_mixA->getCurrentObj() != 17) {

							// show beaker on lamp
							if (_mixA->getCompleteStep()) {
								_xmlscene->editItemState(35, true, _rootNode);
								_clear = true;
							}

							else {
								_xmlscene->editItemState(36, true, _rootNode);
							}


							_bbOnFire[0] = true;


							// clear beaker
							_mixA->Clear();
							_xmlscene->editItemState(17, false, _rootNode);

						}


					}



					_btouch[0] = false;
				}

				// drag beakerB--------------------------------------
				else if (_btouch[1] && !_bbOnFire[1]) {
					for (size_t i = 0; i < 9; i++) {
						_beakerB[i]->setPosition(_itempos);
					}

					// to beakerA.......
					if (_pbeakerRect[0].containsPoint(_touchLoc)) {

						if (_mixB->getCurrentObj() != 26) {
							// add pour liquid sound
							_pour->playEffect();
							_mixA->mixing(_mixB->getCurrentObj(), 0);
						}
						_mixB->Clear();

					}


					// to trash.......
					else if (_trashRect.containsPoint(_touchLoc) && _mixB->getCurrentObj() != 26) {
						// add pour liquid sound
						_pour->playEffect();
						// clear beaker
						_mixB->Clear();
					}

					// to alcohol lamp
					else if (_alcoholLampRect.containsPoint(_touchLoc)) {
						if (_mixB->getCurrentObj() != 26) {

							// show beaker on lamp
							if (_mixB->getCompleteStep()) {
								_xmlscene->editItemState(35, true, _rootNode);
								_clear = true;
							}

							else {
								_xmlscene->editItemState(36, true, _rootNode);
							}


							_bbOnFire[1] = true;


							// clear beaker
							_mixB->Clear();
							_xmlscene->editItemState(26, false, _rootNode);
						}


					}



					_btouch[1] = false;
				}

				// drag bowl------------------------------------------
				else if (_btouch[2]) {
					for (size_t i = 0; i < 4; i++) {
						_bowl[i]->setPosition(_itempos);
					}

					// to beakerA.......
					if (_pbeakerRect[0].containsPoint(_touchLoc) && !_bbOnFire[0]) {
						if (_grind->getCurrentObj() != 12 && _mixA->getCurrentObj() != 17) {


							// add pour powder sound
							_powder->playEffect();
							_mixA->mixing(_grind->getCurrentObj(), 0);
							_grind->Clear();
						}


					}

					// to beakerB.......
					else if (_pbeakerRect[1].containsPoint(_touchLoc) && !_bbOnFire[1]) {
						if (_grind->getCurrentObj() != 12 && _mixB->getCurrentObj() != 26) {

							// add pour powder sound
							_powder->playEffect();
							_mixB->mixing(_grind->getCurrentObj(), 0);
							_grind->Clear();
						}


					}

					// to trash.......
					else if (_trashRect.containsPoint(_touchLoc) && _grind->getCurrentObj() != 12) {
						// add pour powder sound
						_powder->playEffect();
						// clear bowl
						_grind->Clear();
					}

					_btouch[2] = false;
				}


				// drag beaker on alcohol lamp -------------------------
				else if (_btouch[3]) {
					_beakerComplete->setPosition(_itempos);
					_beakerFailed->setPosition(_itempos);


					// to trash.......
					if (_trashRect.containsPoint(_touchLoc)) {
						// add pour powder sound


						_xmlscene->editItemState(35, false, _rootNode);
						_xmlscene->editItemState(36, false, _rootNode);
						if (_bbOnFire[0]) {

							_xmlscene->editItemState(17, true, _rootNode);
							_bbOnFire[0] = false;
						}
						else if (_bbOnFire[1]) {

							_xmlscene->editItemState(26, true, _rootNode);
							_bbOnFire[1] = false;
						}

					}

					_btouch[3] = false;
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
					//_useItem = true;
					// mix mix
					if (_pbeakerRect[0].containsPoint(_touchLoc) && !_bbOnFire[0]) {


						if ((!strcmp(xmlBag::getInstance()->getItemName(i), "B_glassrod.png"))) {
							// add stur sound
							_mixing->playEffect();
						}

						else {
							// add pour liquid sound
							_pour->playEffect();
						}
						_mixA->mixing(i);
						log("mix a, %d", i);



					}
					else if (_pbeakerRect[1].containsPoint(_touchLoc) && !_bbOnFire[1]) {

						if ((!strcmp(xmlBag::getInstance()->getItemName(i), "B_glassrod.png"))) {
							// add stur sound
							_mixing->playEffect();
						}
						else {
							// add pour liquid sound
							_pour->playEffect();
						}
						_mixB->mixing(i);
						log("mix b, %d", i);


					}
					else if (_pbowlRect[0].containsPoint(_touchLoc)) {

						// add grind sound

						if ((!strcmp(xmlBag::getInstance()->getItemName(i), "B_herbR.png")) ||
							(!strcmp(xmlBag::getInstance()->getItemName(i), "B_herbG.png")) ||
							(!strcmp(xmlBag::getInstance()->getItemName(i), "B_herbDG.png")) ||
							(!strcmp(xmlBag::getInstance()->getItemName(i), "B_herbY.png"))) {
							// add debranch
							_grinding->playEffect();
						}
						_grind->mixing(i);
					}


				}
				//else _useItem = false;
			}


		}
		else {
			_procedure->TouchBegan(_touchLoc);
			if (_player->GetIsTalking()) {
				_player->SetIsTalking(false);
				_player->StopTalking();
				log("close talk");
			}
			
		}
	}
}

void BMScene::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_A) {
		_clear = true;
		this->unschedule(schedule_selector(BMScene::doStep));
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->replaceScene(RunScene1::createScene());
	}
}