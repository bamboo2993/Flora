#include "CLock.h"


USING_NS_CC;
using namespace cocos2d;

#ifndef MAX_LOCK_LETTERS
#define MAX_LOCK_LETTERS  10
#endif

char g_Object1[MAX_LOCK_LETTERS][20] = {
	"apple.png","cheese.png","oil.png","plate.png",
	"pot01.png","pot02.png","pot03.png","salt.png",
	"soyasause.png","vinegar.png" };

char g_Object2[MAX_LOCK_LETTERS][20] = {
	"apple.png","pot03.png","oil.png","plate.png",
	"pot02.png","salt.png","soyasause.png","cheese.png",
	"vinegar.png" ,"pot01.png" };

char g_Object3[MAX_LOCK_LETTERS][20] = {
	"pot02.png","apple.png","soyasause.png","oil.png"
	,"pot03.png","plate.png","pot01.png","salt.png",
	"vinegar.png", "cheese.png" };

CLock::CLock(bool type) {

	_nowA = rand() % MAX_LOCK_LETTERS;
	_nowB = rand() % MAX_LOCK_LETTERS;
	_nowC = rand() % MAX_LOCK_LETTERS;

	_btouch = false;
	_state = false;
	_check = false;
}


CLock::~CLock() {

	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool CLock::init(const char *bg) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SceneKitchenitem.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile);

	//add dim layer
	CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 170));
	this->addChild(bgLayer);

	//background image
	//auto background = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName("s101bgimg.png");
	////set position
	//background->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	//this->addChild(background);
	CCLayerColor *bgLayer2 = CCLayerColor::create(ccc4(255, 200, 255, 255));
	bgLayer2->setScale(0.5f);
	this->addChild(bgLayer2);

	_bgRect = Rect(512, 384, 1024, 768);
	 
	//setup button
	for (size_t i = 0; i < 6; i++)
	{
		_button[i] = new CButton;

	}
	_button[0]->setButtonInfo("apple.png", "apple.png", *this, Vec2(660, 1030));
	_button[1]->setButtonInfo("apple.png", "apple.png", *this, Vec2(660, 490));

	_button[2]->setButtonInfo("apple.png", "apple.png", *this, Vec2(1060, 1030));
	_button[3]->setButtonInfo("apple.png", "apple.png", *this, Vec2(1060, 490));

	_button[4]->setButtonInfo("apple.png", "apple.png", *this, Vec2(1460, 1030));
	_button[5]->setButtonInfo("apple.png", "apple.png", *this, Vec2(1460, 490));



	//setup number

	for (int i = 0; i < 10; i++){
			_numberA[i] = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(g_Object1[i]);
			_numberA[i]->setPosition(660, 730);
			this->addChild(_numberA[i]);
			_numberA[i]->setVisible(false);

		}

	for (size_t i = 0; i < 10; i++){
			_numberB[i] = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(g_Object1[i]);
			this->addChild(_numberB[i]);
			_numberB[i]->setPosition(1060, 730);
			_numberB[i]->setVisible(false);
		}


	for (size_t i = 0; i < 10; i++)
		{
			_numberC[i] = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(g_Object1[i]);
			this->addChild(_numberC[i]);
			_numberC[i]->setPosition(1460, 730);
			_numberC[i]->setVisible(false);
		}

	_numberA[_nowA]->setVisible(true);
	_numberB[_nowB]->setVisible(true);
	_numberC[_nowC]->setVisible(true);


	////frige
	//_x = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName("bean1_01.png");
	////set position
	//_x->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	//this->addChild(_x,1);
	//Size size = _x->getContentSize();
	//Point pt = _x->getPosition();
	//auto rectx = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);


	label = LabelTTF::create("Please Touch!", "Arial", 36);//?定相?文本的位置  
	label->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 5*4));//放在屏幕的中央
	this->addChild(label, 100);


	this->setVisible(false);

	this->schedule(CC_SCHEDULE_SELECTOR(CLock::doStep));
	return true;
}



//void CLock::Set(const cocos2d::Sprite item, const std::string enlarge) {
//
//	//set enlarge sprite =================================================================================
//
//	Size visibleSize = this->getContentSize();
//
//
//	_enlarge = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(enlarge);
//	//set position
//	_enlarge->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
//	this->addChild(_enlarge, 1);
//
//	Size size = _enlarge->getContentSize();
//	Point pt = _enlarge->getPosition();
//	_enlargeRect = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);
//
//
//}


void CLock::SetArea(const cocos2d::Rect &lockarea) {
	_lockAreaRect = lockarea;
}

void CLock::SetPassword(int a, int b, int c) {
	_ansA = a;
	_ansB = b;
	_ansC = c;
}

void CLock::doStep(float dt) {
	if (_check) {
		if (_nowA == _ansA && _nowB == _ansB && _nowC == _ansC) {
			log("correct");
			label->setString("Correct!");

			_state = true;
			this->setVisible(false);
			_check = false;
		}
		_check = false;
	}
}

void CLock::reset() {
	for (int i = 0; i < 10; i++)
	{
		
		_numberA[i]->setVisible(false);
		_numberB[i]->setVisible(false);
		_numberC[i]->setVisible(false);
	}
	_nowA = rand() % MAX_LOCK_LETTERS;
	_nowB = rand() % MAX_LOCK_LETTERS;
	_nowC = rand() % MAX_LOCK_LETTERS;

	_numberA[_nowA]->setVisible(true);
	_numberB[_nowB]->setVisible(true);
	_numberC[_nowC]->setVisible(true);
}

bool CLock::GetState()
{
	return _state;
}

bool CLock::TouchBegan(const cocos2d::Point pt) {
	
	_button[0]->touchesBegin(pt);
	_button[1]->touchesBegin(pt);
	_button[2]->touchesBegin(pt);
	_button[3]->touchesBegin(pt);
	_button[4]->touchesBegin(pt);
	_button[5]->touchesBegin(pt);

	return true;
}



bool CLock::TouchMoved(const cocos2d::Point pt) {
	_button[0]->touchesMoved(pt);
	_button[1]->touchesMoved(pt);
	_button[2]->touchesMoved(pt);
	_button[3]->touchesMoved(pt);
	_button[4]->touchesMoved(pt);
	_button[5]->touchesMoved(pt);

	return true;
}
bool CLock::TouchEnded(const cocos2d::Point pt) {

	if (_lockAreaRect.containsPoint(pt) && !_btouch) {
	this->setVisible(true);
	_btouch = true;
	return false;
	}

	else if (!_bgRect.containsPoint(pt) && _btouch) {
		if (!_state) {
			this->setVisible(false);
			_btouch = false;

			reset();
			return false;
		}
		return false;
	}




	if (_btouch) {
		if (_button[0]->touchesEnded(pt)) {
			_numberA[_nowA]->setVisible(false);
			if (_nowA >= MAX_LOCK_LETTERS - 1) _nowA = 0;
			else _nowA++;
			_numberA[_nowA]->setVisible(true);
			label->setString("Up!");
			log("a   %d", _nowA);
			_check = true;
		}
		else if (_button[1]->touchesEnded(pt)) {
			_numberA[_nowA]->setVisible(false);
			if (_nowA == 0) _nowA = MAX_LOCK_LETTERS - 1;
			else _nowA--;
			_numberA[_nowA]->setVisible(true);
			label->setString("Down!");
			log("a   %d", _nowA);
			_check = true;
		}
		else if (_button[2]->touchesEnded(pt)) {
			_numberB[_nowB]->setVisible(false);
			if (_nowB >= MAX_LOCK_LETTERS - 1) _nowB = 0;
			else _nowB++;
			_numberB[_nowB]->setVisible(true);
			log("b   %d", _nowB);
			_check = true;
		}
		else if (_button[3]->touchesEnded(pt)) {
			_numberB[_nowB]->setVisible(false);
			if (_nowB == 0) _nowB = MAX_LOCK_LETTERS - 1;
			else _nowB--;
			_numberB[_nowB]->setVisible(true);
			log("b   %d", _nowB);
			_check = true;
		}
		else if (_button[4]->touchesEnded(pt)) {
			_numberC[_nowC]->setVisible(false);
			if (_nowC >= MAX_LOCK_LETTERS - 1) _nowC = 0;
			else _nowC++;
			_numberC[_nowC]->setVisible(true);
			log("c   %d", _nowC);
			_check = true;
		}
		else if (_button[5]->touchesEnded(pt)) {
			_numberC[_nowC]->setVisible(false);
			if (_nowC == 0) _nowC = MAX_LOCK_LETTERS - 1;
			else _nowC--;
			_numberC[_nowC]->setVisible(true);
			log("c   %d", _nowC);
			_check = true;
		}


	}
}