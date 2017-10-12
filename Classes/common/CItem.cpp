//CItems in bag

#include "CItem.h"

USING_NS_CC;

CItem* CItem::create() {
	CItem* obj = new(std::nothrow)CItem();
	if (obj) {
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

CItem::CItem() {
	_propSprite = nullptr;
//	_Del = false;
	_isUsed = false;
	_canUse = false;
	_targetNum = 0;
	//_bRetake = false;

}

CItem::~CItem() {
	
}
void CItem::Init(const char* pic) {
	/*
	int len;
	len = strlen(pic);
	_pic = new char[len + 5]; //include ¡i.png¡j
	memcpy(_pic, pic, len + 1);


	int i = _count, j = 0;

	while (i > 0) {
		_cScore[8 - j] = i % 10 + 48;
		i = i / 10;
		j++;
	}
	label1->setString(_cScore);
	*/

	_propSprite = Sprite::createWithSpriteFrameName(pic);
	this->addChild(_propSprite);
	SetRect();
	_bRetake = false;
	_isStagnant = false;

	_itemName = pic;

}

void CItem::Clear() {
	if (_propSprite != nullptr)_propSprite->removeFromParentAndCleanup(true);

}

void CItem::reset() {
	Clear();

	_propSprite = nullptr;
	_isUsed = false;
	_canUse = false;
	_targetNum = 0;

	_bRetake = false;
	_isStagnant = false;
	_itemName = nullptr;
	_bTouch = false;


}

const char* CItem::GetName() {
	return _itemName;
}

void CItem::SetRetake( CTrigger* trigger) {
	_pTrigger = trigger;
	_bRetake = true;
}

void CItem::SetTarget(cocos2d::Rect target) {
	_targetRect[_targetNum] = target;
	_targetNum++;
}

cocos2d::Rect CItem::GetTarget(int num) {
	return _targetRect[num];
}

void CItem::SetStagnant(bool x) {
	_isStagnant = x;
}


void CItem::SetPos(Point pos) {
	_ResetPos = pos;
	this->setPosition(pos);
}

void CItem::SetBagPos(Point pos) {
	_BagPos = pos;
}

void CItem::SetRect(){
	Size size = _propSprite->getContentSize();
	_propsPos = this->getPosition();
	_propsRect = Rect(_propsPos.x - size.width / 2, _propsPos.y - size.height / 2, size.width, size.height);
}
void CItem::SetRect(Point parentPos) {
	Size size = _propSprite->getContentSize();
	_propsPos = this->getPosition();
	_propsRect = Rect(parentPos.x+_propsPos.x - size.width / 2, parentPos.y+_propsPos.y - size.height / 2, size.width, size.height);
}

void CItem::SetVisible(bool x) {
	_bVis = x;
	_propSprite->setVisible(_bVis);
}

bool CItem::GetUsed() {
	return(_isUsed);
}


bool CItem::GetRetake() {
	return(_bRetake);
}
bool CItem::GetStagnant() {
	return(_isStagnant);
}

CTrigger* CItem::GetTrigger() {
	return(_pTrigger);
}

bool CItem::touchesBegan(Point inPos) {
	if (!_isUsed) {
		if (_propsRect.containsPoint(inPos)) {
			_prePos = inPos;
			_bTouch = true;
			log("xx");
			this->setPosition(_ResetPos);
			return(true);
		}
	}
	return(false);
}

bool CItem::touchesMoved(Point inPos) {
	if(_bTouch){
		this->setPosition(this->getPosition()+ (inPos - _prePos));
		_prePos = inPos;
		return(true);
	}
	return(false);
}

bool CItem::touchesEnded(cocos2d::Point inPos) {

	if (_bTouch ) {
		for (size_t i = 0; i < _targetNum; i++) {
			if (_targetRect[i].containsPoint(inPos)) {

				if (!_isStagnant) {
					_propSprite->setVisible(false);
					log("obj not stagnant");
					SetUsed(true);
					//	_Del = true;
				}
				else {
					this->setPosition(_ResetPos); // when item is stagnant
					log("obj stagnant");
					//SetUsed(true);
				}

				_bTouch = false;
				return(true);
			}
		}
		
		//when obj not used (back to its position in bag)
		this->setPosition(_ResetPos);
		_bTouch = false;
			
		
	}
	return(false);
}

void CItem::SetCanUse(bool x) {
	_canUse = x;
}

bool CItem::GetCanUse() {
	return(_canUse);
}
Point CItem::GetResetPos() {
	return(_ResetPos);
}

void CItem::SetUsed(bool x) {
	_isUsed = x;
}

void CItem::MoveX(float x) {
	this->setPosition(_propsPos.x + x, _propsPos.y);
	_ResetPos = Point(_propsPos.x + x, _propsPos.y);
}