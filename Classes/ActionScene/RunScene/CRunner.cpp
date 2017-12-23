#include "CRunner.h"
#include "cocos2d.h"

#include "cocostudio/CocoStudio.h"

#define ORIGIN_POSITION Vec2(320, 600)
#define FAST_POSITION 500
#define SLOW_POSITION 220
#define UP_POSITION 600
#define DOWN_POSITION 480


USING_NS_CC;


//
// 建立角色之前，必須先確定已經讀入儲存角色所有動態圖片的 plist 檔
//

CRunner::CRunner(const char *body, cocos2d::Layer &parent)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/ater.plist");
	// 讀取角色
	_runner = Sprite::createWithSpriteFrameName(body);
	_runner->setPosition(0,0); // 預設放在 (0,0) 
	//_runnerRoot->setPhysicsBody(PhysicsBody::createBox(_runnerSize));
	//_runnerRoot->getPhysicsBody()->setDynamic(false);

	_runnerSize = _runner->getContentSize();
	_runnerpt = _runner->getPosition();
	parent.addChild(_runner,10);


	//_runner->setPhysicsBody(PhysicsBody::createBox(_runnerSize));
	//_runner->getPhysicsBody()->setDynamic(false);

	//jump====================================================
	// 建立 runnerRoot 所需的 jumpAction
	_jump = cocos2d::JumpBy::create(1.3f, Point(_runnerpt.x, _runnerpt.y), 350, 1);
	_jump->retain();
	_callback = CallFunc::create(CC_CALLBACK_0(CRunner::jumpFinished, this));
	_callback->retain();

	_bup = false;
	_bdown = false;

	_bfast = false;
	_bslow = false;

	_ilife = 3;

	_runner->setFlipX(true);
	_bgo = false;

	state = true;
}

CRunner::~CRunner()
{
	//_action->release();
}

bool CRunner::getstate() {
	return state;
}

void CRunner::jumpFinished()
{
	// do something on complete
	log("B %d\n", _NoJumps);

	_NoJumps--;
}



void CRunner::setAnimation(const char *plistFile)
{
	// 此處假設儲存角色所有動態圖片的 plist 檔已經讀入
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile(plistFile);

	auto animation = cache->getAnimation("running");
	_runaction = RepeatForever::create(Animate::create(animation));
	_runaction->retain();
	//Director::getInstance()->getActionManager()->addAction(_runaction, this->_runner, true);




	//animation = cache->getAnimation("jumping");
	//_jumpaction = RepeatForever::create(Animate::create(animation));
	//_jumpaction->retain();

	//animation = cache->getAnimation("squating");
	//_squataction = RepeatForever::create(Animate::create(animation));
	//_squataction->retain();

}

void CRunner::jump() {
	_runner->runAction(Sequence::create(_jump, _callback, NULL));
	_NoJumps++;


	////_runner->setVisible(true);
	//_runner->runAction(_jumpaction);
	//Director::getInstance()->getActionManager()->addAction(_jumpaction, this->_runner, true);
}

void CRunner::run()
{

	//_runnerRoot->setVisible(true);
	//_runner->runAction(_runaction);
	//Director::getInstance()->getActionManager()->addAction(_runaction, this->_runner, true);
	if (_bgo) {
		_runner->runAction(_runaction);
		_bgo = !_bgo;
	}

}




//
//void CRunner::squat()
//{
//	//_runnerRoot->setVisible(true);
//	_runner->runAction(_squataction);
//	Director::getInstance()->getActionManager()->addAction(_squataction, this->_runner, true);
//
//
//}













void CRunner::stop() {
	_runner->stopAction(_runaction);
}






void CRunner::setPosition(const Point &pos)
{
	_runner->setPosition(pos);
}

void CRunner::setPosition(const float x, const float y)
{
	_runner->setPosition(x,y);
}


void CRunner::setRect() {
	_runnerSize = _runner->getContentSize();//抓大小
	_runnerpt = _runner->getPosition();//抓節點
	runnerRect = Rect(_runnerpt.x - _runnerSize.width / 2, _runnerpt.y - _runnerSize.height / 2, _runnerSize.width, _runnerSize.height);
}


cocos2d::Rect CRunner::getRect() {
	return runnerRect;

}



void CRunner::setGo() {
	_bgo = true;
}


void CRunner::doStep(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	setRect();
	auto instancepos = _runner->getPosition();
	
	log("pos.x: %d, pos.y: %d", instancepos.x, instancepos.y);
	
	if (_bgo) run();

	if (_bfast){
		if (instancepos.x < FAST_POSITION) {
			_runner->setPositionX(_runner->getPositionX() + 5);
		}
		else _bfast = false;
	}

	else if (_bslow) {
		if (instancepos.x > SLOW_POSITION) {
			_runner->setPositionX(_runner->getPositionX() - 5);
		}
		else _bslow = false;

	}

	else if (_bup) {
		if (instancepos.y < UP_POSITION) {
			_runner->setPositionY(_runner->getPositionY() + 5);
			state = true;
		}
		else _bup = false;

	}

	else if (_bdown) {
		if (instancepos.y > DOWN_POSITION) {
			_runner->setPositionY(_runner->getPositionY() - 5);
			state = false;
		}
		else _bdown = false;

	}



	//if (_bjump && _NoJumps == 0 ) jump();
	//else _bjump = false;

}