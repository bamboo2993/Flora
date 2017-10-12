#include "CObstacle.h"

#define SHIFT_X 15
#define NODE_SHIFT Vec2(1024,0)
#define MOVESPEED 250


USING_NS_CC;
using namespace cocostudio::timeline;

CObstacle::CObstacle() {

	_objRoot = nullptr;
	for (int i = 0; i < 3; i++) _objSprite[i] = nullptr;
	_bvisible = true;
}


void CObstacle::init(const char* csbfilename, int n, const cocos2d::Point &loc, cocos2d::Layer &parent)
{
	
	_num = n;
	// 讀取並建立物件的內容
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;
	srand((unsigned)time(NULL));
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// 在亂數的改變一下 X 軸座標
	_objRoot->setPosition(pt);
	parent.addChild(_objRoot, 5);
	for (int i = 0; i < n; i++)
	{
		char objname[50];
		sprintf(objname, "object%1d", i + 1);
		_objSprite[i] = (Sprite *)_objRoot->getChildByName(objname);
		_objSprite[i]->setVisible(false);
	}
	// 隨機決定一個為 true
	_inum = rand() % n;
	_objSprite[_inum]->setVisible(true);

	_IsDamage = false;
	//set rect

	Size size = _objSprite[_inum]->getContentSize();
//	Point p = pt + NODE_SHIFT;
	_obsRect = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);
	//_objRoot->setPhysicsBody(PhysicsBody::createBox(size));
	//_objRoot->getPhysicsBody()->setDynamic(false);
}


// 改變物件的位置與顯示的內容
void CObstacle::resetObj(bool type)
{
	if (type)  instantpt = _LocPt;
	else {
		instantpt = _SetPt;  //設初始位置
		instantpt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);
	}


	
	_objRoot->setPosition(instantpt);

	_IsDamage = false;
	//Point p = pt + NODE_SHIFT;
	Size size;

	_inum = rand() % _num;
	switch (_inum)
	{
	case 0:
		_objSprite[0]->setVisible(true);
		_objSprite[1]->setVisible(false);
		if (_num == 3)_objSprite[2]->setVisible(false);

		size = _objSprite[_inum]->getContentSize();
		_obsRect = Rect(instantpt.x - size.width / 2, instantpt.y - size.height / 2, size.width, size.height);
		break;
	case 1:
		_objSprite[0]->setVisible(false);
		_objSprite[1]->setVisible(true);
		if (_num == 3)_objSprite[2]->setVisible(false);

		size = _objSprite[_inum]->getContentSize();
		_obsRect = Rect(instantpt.x - size.width / 2, instantpt.y - size.height / 2, size.width, size.height);
		break;
	case 2:
		_objSprite[0]->setVisible(false);
		_objSprite[1]->setVisible(false);
		if (_num == 3)_objSprite[2]->setVisible(true);

		size = _objSprite[_inum]->getContentSize();
		_obsRect = Rect(instantpt.x - size.width / 2, instantpt.y - size.height / 2, size.width, size.height);
		break;
	default:
		break;
	}
}


void  CObstacle::doStep(float dt) {

	Size size = _objSprite[_inum]->getContentSize();//抓大小
	//Point pt = _objRoot->getPosition();//抓節點
	//Point instantpt = pt + NODE_SHIFT;
	
	instantpt = _objRoot->getPosition();
	instantpt.x -= dt*MOVESPEED;



	_objRoot->setPosition(instantpt);
	_obsRect = Rect(instantpt.x - size.width / 2, instantpt.y - size.height / 2, size.width, size.height);

	if (instantpt.x <= -60) {
		resetObj();
	}

}


bool CObstacle::collision(cocos2d::Rect runner)
{
	if (_obsRect.intersectsRect(runner) && !_IsDamage) {
		_IsDamage = true;
		return true;
	}
	else return false;
}



void  CObstacle::resetPos(float x, float y) {
	_SetPt = Vec2(x, y);
}


cocos2d::Rect  CObstacle::getRect() {
	return _obsRect;

}

cocos2d::Point CObstacle::getPosition() {
	return _objRoot->getPosition();

}


void CObstacle::setVis(bool visible) {
	_bvisible = visible;
	_objRoot->setVisible(_bvisible);

}


bool CObstacle::getVis() {
	return _bvisible;
}
