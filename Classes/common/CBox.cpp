#include "CBox.h"

using namespace cocos2d;

CBox::CBox() {


}


CBox::~CBox(){

	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool CBox::init() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();



	return true;
}


void CBox::SetInfo(const cocos2d::Point a, const cocos2d::Point b, const cocos2d::Point c, const cocos2d::Point d, const cocos2d::Point openLoc, const std::string open, cocos2d::Layer &parent) {
	//set close region================================================================================
	SetCloseReg(a, b, c, d);


	//set open sprite =================================================================================
	_open = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(open);
	_openLoc = openLoc;
	_open->setPosition(_openLoc);	//set position 
	parent.addChild(this->_open);	//add child

	Size size = _open->getContentSize();
	_openOrigin = Vec2(_openLoc.x - size.width / 2, _openLoc.y - size.height / 2);



	//set status (off) =====================================================================================
	state = false;
	Status(state);


}

void CBox::SetCloseReg(const cocos2d::Point a, const cocos2d::Point b, const cocos2d::Point c, const cocos2d::Point d) {
	//set region

	//calcucale gradient
	_close.fm = (d.y - a.y) / (d.x - a.x);
	
	//set origin
	if (_close.fm > 0) {
		_close.bgradient = true;
		_close.ARect.origin.x = a.x;
		_close.ARect.origin.y = a.y;
	}
	else {
		_close.bgradient = false;
		_close.fm = -_close.fm;
		_close.ARect.origin.x = a.x;
		_close.ARect.origin.y = d.y;
	}

	//no of division
	if (_close.fm <= 0.2f) _close.iparts = 1;
	else if (_close.fm <= 0.3f) _close.iparts = 2;
	else _close.iparts = 3;

	//grad = positive
	if (_close.bgradient) {
		switch (_close.iparts)
		{
		case 1:
			_close.ARect.size.height = c.y - a.y;
			_close.ARect.size.width = d.x - a.x;
			break;

		case 2:
			_close.ARect.size.width = (d.x - a.x) / 2;
			_close.BRect.size.width = _close.ARect.size.width;

			_close.ARect.size.height = (d.y - a.y) / 2 + (b.y-a.y);
			_close.BRect.size.height = _close.ARect.size.height;

			_close.BRect.origin.x = a.x + _close.ARect.size.width;
			_close.BRect.origin.y = a.y + (d.y - a.y) / 2;

			break;

		case 3:
			_close.ARect.size.width = (d.x - a.x) / 3;
			_close.BRect.size.width = _close.ARect.size.width;
			_close.CRect.size.width = _close.ARect.size.width;

			_close.ARect.size.height = (d.y - a.y) / 3 + (b.y - a.y);
			_close.BRect.size.height = _close.ARect.size.height;
			_close.CRect.size.height = _close.ARect.size.height;

			_close.BRect.origin.x = a.x + _close.ARect.size.width;
			_close.BRect.origin.y = a.y + (d.y - a.y) / 3;

			_close.CRect.origin.x = _close.BRect.origin.x + _close.BRect.size.width;
			_close.CRect.origin.y = _close.BRect.origin.y + (d.y - a.y) / 3;

			break;

		


		}
	}

	else {
		switch (_close.iparts)
		{
		case 1:
			_close.ARect.size.height = b.y - d.y;
			_close.ARect.size.width = d.x - a.x;
			break;

		case 2:
			_close.ARect.size.width = (d.x - a.x) / 2;
			_close.BRect.size.width = _close.ARect.size.width;

			_close.ARect.size.height = (a.y - d.y) / 2 + (b.y - a.y);
			_close.BRect.size.height = _close.ARect.size.height;


			_close.ARect.origin.y = a.y - (a.y - d.y) / 2;

			_close.BRect.origin.x = a.x + _close.ARect.size.width;
			_close.BRect.origin.y = _close.ARect.origin.y - (a.y - d.y) / 2;

			break;

		case 3:
			_close.ARect.size.width = (d.x - a.x) / 3;
			_close.BRect.size.width = _close.ARect.size.width;
			_close.CRect.size.width = _close.ARect.size.width;

			_close.ARect.size.height = (a.y - d.y) / 3 + (b.y - a.y);
			_close.BRect.size.height = _close.ARect.size.height;
			_close.CRect.size.height = _close.ARect.size.height;

			_close.ARect.origin.y = a.y - (a.y - d.y) / 3;

			_close.BRect.origin.x = a.x + _close.ARect.size.width;
			_close.BRect.origin.y = _close.ARect.origin.y - (a.y - d.y) / 3;

			_close.CRect.origin.x = a.x + 2 * _close.ARect.size.width;
			_close.CRect.origin.y = _close.BRect.origin.y - (a.y - d.y) / 3;

			break;

		
		}
	}

}


void CBox::setItem(const cocos2d::Point itemLoc, const std::string item) {
	//set sprite
	_item = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(item);

	_itemLoc.x = itemLoc.x - _openLoc.x;
	_itemLoc.y = itemLoc.y - _openLoc.y;

	_item->setPosition(_itemLoc); //set position 
	_open->addChild(_item, 1);//add child

	Size size = _item->getContentSize();

	_itemRect = Rect(itemLoc.x - size.width / 2, itemLoc.y - size.height / 2, size.width, size.height);


	//_item ->setPhysicsBody(PhysicsBody::createBox(size));
	//_item->getPhysicsBody()->setDynamic(false);

}

void CBox::setItemCard(const std::string card, cocos2d::Layer &parent) {
	//set sprite
	_card = (cocos2d::Sprite *) Sprite::createWithSpriteFrameName(card);

	Size visibleSize = parent.getContentSize();

	//set position
	_card->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3 * 2));

	_card->setOpacity(0);
	parent.addChild(_card, 5);//add child

	//set animation ===============================================

	auto fadein = FadeIn::create(1.25f);
	auto fadeout = FadeOut::create(1.25f);

	auto moveby = MoveBy::create(1.0f, Point(0, visibleSize.height / 6));
	auto movebyBack = moveby->reverse();
	auto movein = EaseSineOut::create(moveby);
	auto moveout = EaseSineIn::create(movebyBack);


	_In = Spawn::createWithTwoActions(fadein, movein);
	_Out = Spawn::createWithTwoActions(fadeout, moveout);
	_delay = DelayTime::create(0.5f);	//°±¯d 0.5 ¬í
	_In->retain();
	_Out->retain();
	_delay->retain();

	_bpress = false;
	_callback = CallFunc::create(CC_CALLBACK_0(CBox::itemPressed, this));
	_callback->retain();
}


void CBox::itemPressed()
{
	_bpress = true;
}



void CBox::Status(bool state) {
	_open->setVisible(state);
}

bool CBox::TouchBegan(const cocos2d::Point pt) {

	//open
	if (_close.ARect.containsPoint(pt) || _close.BRect.containsPoint(pt) || _close.CRect.containsPoint(pt) || _close.DRect.containsPoint(pt) && !state) {
		state = true;
		Status(state);
		return false;
	}

	//close
	if (_close.ARect.containsPoint(pt) || _close.BRect.containsPoint(pt) || _close.CRect.containsPoint(pt) || _close.DRect.containsPoint(pt) 
		&& state && !_itemRect.containsPoint(pt)) {
		state = false;
		Status(state);
		return false;
	}

	// pick item
	if (_itemRect.containsPoint(pt) && state) {

		_item->setVisible(false);

		if (!_bpress) {
			_card->runAction(Sequence::create(_callback, _In, _delay, _Out, NULL));
		}
		return true;
	}
}
