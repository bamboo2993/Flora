#include "CFall.h"
#include "cocostudio/CocoStudio.h"
#define G -0.09f
#define MOVESPEED 250
USING_NS_CC;

using namespace cocostudio::timeline;

CFall::CFall(Node *node, Layer &parent)
{

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SceneKitchenitem.plist");
	//_obstacle = Sprite::createWithSpriteFrameName("pot02.png");

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("runscene.plist");
	//_obstacle = Sprite::createWithSpriteFrameName("trash.png");

	_obstacle = (cocos2d::Sprite*) node->getChildByName("throw");
	_obstacle->setVisible(false);
	_node = node;

	_Initialpos = _obstacle->getPosition();
	//_obstacle->setPosition(0, 0);
	parent.addChild(_obstacle,6);

	resetObj();

	
}

CFall::~CFall()
{

}

void CFall::resetObj() {
	_state = false;
	_obstacle->setVisible(false);

	_IsDamage = false;	
	srand((unsigned)time(NULL));
	_iType = rand() % 3;
	float x = rand() % 1800 + 100;
	switch (_iType)
	{
	case 0:
		setPosition(_Initialpos.x, _Initialpos.y);
		setRect();
		_vx = 2.0f;
		_vy = 10.0f;
		break;
	case 1:
		setPosition(_Initialpos.x, _Initialpos.y);
		setRect();
		_vx = 2.0f;
		_vy = 8.5f;
		break;
	case 2:
		setPosition(_Initialpos.x, _Initialpos.y);
		setRect();
		_vx = 3.5f;
		_vy = 7.0f;
		break;
	}
}


void CFall::doStep(float dt) {
	if (_state) {
		_obstacle->setVisible(true);

		setRect();
		_vy += G;
		if (_Instancepos.y >= 400) setPosition(_Instancepos.x + _vx - dt*MOVESPEED, _Instancepos.y + _vy); //拋物線
		//else {

		//	if (_Instancepos.x <= -1124) {
		//		resetObj();

		//	}
		//	//_Instancepos.x -= dt*MOVESPEED;
		//	//_obstacle->setPosition(_Instancepos); //落地, 跟著陸地一起移動
		//}
	}
	
	

}


bool CFall::collision(Rect runner)
{
		if (_state && _obstacle->getPositionY() > 400.0f && _trashRect.intersectsRect(runner) && !_IsDamage) {
				_IsDamage = true;
				return true;
		}
		else return false;
}

void CFall::setRect()
{
	Size size = _obstacle->getContentSize();
	auto pos = _node->getPosition();
	_Instancepos = _obstacle->getPosition();
	_trashRect = Rect(_Instancepos.x- size.width / 2 + pos.x, _Instancepos.y- size.height / 2, size.width , size.height );
}



void CFall::setPosition(const float x, const float y)
{
	_obstacle->setPosition(x, y);
}

void CFall::setScale(float f)
{
	_obstacle->setScale(f);
}

void CFall::setstate(bool state) {
	_state = state;
}
bool CFall::getstate() {
	return _state;
}