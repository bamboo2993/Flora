#include "CPlayer.h"
#include <cmath>

#define PIXEL_SEC 100

USING_NS_CC;


CPlayer::CPlayer(const std::string body, const std::string aniBody, cocos2d::Layer &parent)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/ater_1.plist");

	// 讀取角色
	_player = Sprite::createWithSpriteFrameName(body);
	_player->setPosition(visibleSize.width / 2.0f, visibleSize.height / 4.0f);
	parent.addChild(_player);
	bStop = true;
	_isFacingRight = 0;

	

	_body = Sprite::createWithSpriteFrameName(aniBody);
	_player->addChild(_body);

	_body->setVisible(false);
	_contentSize = _body->getContentSize();

	_rpos = _player->getPosition();


	//_body->setScaleY(0.15f);
	//Size size = _body->getContentSize();
	//
	//_body->setPosition(size.width/2, size.height/2*0.15f);
	////size.height /= 3;
	//auto physicsBody = PhysicsBody::createBox(size);
	//physicsBody->setRotationEnable(false);
	//_body->setPhysicsBody(physicsBody);

	//
	for (int i = 0; i < 20; i++) {
		_reachSpot[i] = false;
	}

	_myAction = nullptr;
}

CPlayer::CPlayer(bool isBack, cocos2d::Layer &parent, Point pos, bool isFacingR)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/ater_1.plist");

	_stand[0] = Sprite::create("Animation/ATstand01.png");
	_stand[1] = Sprite::create("Animation/ATstand02.png");
	
	// 讀取角色
	_player = Sprite::create("Animation/ATstand01.png");
	_player = _stand[isBack];
	_player->setAnchorPoint(Point(0.5, 0));
	_player->setPosition(pos);
	parent.addChild(_player);
	bStop = 1;
	_isFacingRight = isFacingR;
	Mirror();

	_rpos = _player->getPosition();

	for (int i = 0; i < 20; i++) {
		_reachSpot[i] = false;
	}

	_myAction = nullptr;
}

CPlayer::CPlayer(const std::string body, cocos2d::Layer &parent, Point pos, bool isFacingR)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/ater_1.plist");

	// 讀取角色
	_player = Sprite::create(body);
	_player->setAnchorPoint(Point(0.5, 0));
	_player->setPosition(pos);
	parent.addChild(_player);
	bStop = 1;
	_isFacingRight = isFacingR;
	Mirror();

	_rpos = _player->getPosition();

	for (int i = 0; i < 20; i++) {
		_reachSpot[i] = false;
	}

	_myAction = nullptr;
}

CPlayer::~CPlayer()
{
	_action[0]->release();
	_action[1]->release();
	if (_myAction != nullptr) _myAction->release();
}

void CPlayer::setRect()
{
	_rpos = _player->getPosition();
	_myRect = Rect(_rpos.x - _contentSize.width / 2, _rpos.y - _contentSize.height / 2, _contentSize.width, _contentSize.height);
	log("%f", _myRect);
}


void CPlayer::Update()
{
	setRect();
}



void CPlayer::setAnimation(const char *plistFile)
{
	// 此處假設儲存角色所有動態圖片的 plist 檔已經讀入
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile(plistFile);
	auto animation = cache->getAnimation("45degree");
	_action[0] = RepeatForever::create(Animate::create(animation));
	_action[0]->retain();

	animation = cache->getAnimation("45degree_back");
	_action[1] = RepeatForever::create(Animate::create(animation));
	_action[1]->retain();
}

void CPlayer::setPosition(const Point &pos)
{
	_player->setPosition(pos);
}

void CPlayer::setPosition(const float x, const float y)
{
	_player->setPosition(x, y);
}

void CPlayer::setPreviousPosition(){
	_previousPos.x = _player->getPosition().x;
	_previousPos.y = _player->getPosition().y - 300.0f;
}

cocos2d::Point CPlayer::getPreviousPosition(){
	return _previousPos;
}


const Vec2 CPlayer::getPosition()
{
	log("%1.0f,%1.0f", _player->getPositionX(), _player->getPositionY());
	return _player->getPosition();
	
}


void CPlayer::go(cocos2d::Point pt)
{
	if (bStop) {
		auto a = _player->getPosition();
		pt.y += 300.0f;
		if (a.y > pt.y)  _player->runAction(_action[0]);
		else _player->runAction(_action[1]);
		
		bStop = false;

	}

}


void CPlayer::go(bool isBack)
{
	if (bStop) {
		if (!isBack)  _player->runAction(_action[0]);
		else _player->runAction(_action[1]);

		bStop = false;

	}

}

void CPlayer::Stop() {
	_player->stopAction(_action[0]);
	_player->stopAction(_action[1]);
	bStop = true;

}

void CPlayer::Stop(bool isBack) {
	_player->stopAction(_action[0]);
	_player->stopAction(_action[1]);
	bStop = true;
	_player = _stand[isBack];
}

void CPlayer::Mirror() {
	if (_isFacingRight) {
		_player->setFlipX(true);
	}
	else {
		_player->setFlipX(false);
	}

}

void CPlayer::Mirror(bool isFacingR) {
	if (isFacingR) {
		_player->setFlipX(true);
	}
	else {
		_player->setFlipX(false);
	}
}

bool CPlayer::Walk(Point i) {
	_rpos = _player->getPosition();
	i.y += 300.0f;
	//	_rpos.y = _rpos.y- _contentSize.height / 2;
	float fx = i.x - _rpos.x;
	float fy = i.y - _rpos.y;
	float t = sqrt(fx*fx + fy*fy);
	if (t >= 1.0f) {
		_player->setPosition(_rpos.x + fx*0.016666f * PIXEL_SEC / t, _rpos.y + fy*0.016666f * PIXEL_SEC / t);
		return true;
	}
	else return(false);
}

void CPlayer::Talk(const std::string picName, bool isRight) {
	_sentance = Sprite::createWithSpriteFrameName(picName);
	Vec2 playerPos = _player->getPosition();

	if (isRight) {
		_sentance->setAnchorPoint(Point(0, 0));
	}
	else{
		_sentance->setAnchorPoint(Point(1, 0));
	}
	
	_sentance->setPosition(playerPos.x, playerPos.y + 50.0f);
	_player->addChild(_sentance);
}

void CPlayer::StopTalking() {
	_sentance->removeFromParent();
}

void CPlayer::SetReachSpot(int n, bool f) {
	_reachSpot[n] = f;
}

bool CPlayer::GetReachSpot(int n) {
	return _reachSpot[n];
}