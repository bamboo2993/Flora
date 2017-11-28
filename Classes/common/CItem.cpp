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
	_bTouch = false;
}

CItem::~CItem() {
	
}
void CItem::Init(const char* pic) {

	_propSprite = Sprite::createWithSpriteFrameName(pic);
	SetVisible(false);
	this->addChild(_propSprite);
	SetRect();
	_bRetake = false;
	_isStagnant = false;
	_targetNum = 0;
	_itemName = pic;

}

void CItem::Clear() {
	if (_propSprite != nullptr) {
		_propSprite->removeFromParentAndCleanup(true);
	}

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

void CItem::SetRetake() {
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


bool CItem::touchesBegan(Point inPos) {
	if (!_isUsed) {
		_startX = inPos.x;
		_startY = inPos.y;

		if (_propsRect.containsPoint(inPos)) {
			_prePos = inPos;
			_bTouch = true;
			log("xx");
			this->setPosition(_ResetPos);
			_isdrag = true;
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

int CItem::touchesEnded(cocos2d::Point inPos, int bagstate, const char* scene, int num, int pageNum) {

	if (_bTouch ) {
		float offsetX = inPos.x - _startX;
		float offsetY = inPos.y - _startY;

		auto name = xmlBag::getInstance()->getItemName(num);
		auto state = xmlItem::getInstance()->getItemStateXML(name);

		if (offsetX == 5 && offsetY == 5 && bagstate) {
			// observe item

		}


		//for bagstate==1==============
		if (bagstate == 1) {
			//WHEN ITEM IS USED IN SCENE
			if (state) {
				for (size_t i = 0; i < _targetNum; i++) {
					auto s = xmlItem::getInstance()->getTargetSceneXML(name, i);
					if ((!strcmp(s, scene))) {
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
							_isdrag = false;
							return(1);
						}

					}
				}
			}

			//WHEN ITEM IS USED IN BAG FOR MIX & MAKE
			else {
				auto bagNum = detectUse(inPos);
				
				if (bagNum > 0) {
					bagNum = bagNum + 7 * (pageNum - 1);
					auto item = xmlBag::getInstance()->getNameFromArrangement(bagNum);
					if (item != nullptr) {
						int mixState = xmlItem::getInstance()->checkMixingXML(name, item);
						if (mixState >= 0) { //mix correctly==============================
							_propSprite->setVisible(false);
							xmlBag::getInstance()->setBagState(num, false); // item being dragged is used
							SetCanUse(false); //item cannot be used again
							SetUsed(true);

							_isdrag = false;
							return (mixState + 5);
						}
					}

				}

			}

		}

		//for bagstate == 2 [full screen]
		else {
			if(!state) {
				auto bagNum = detectUse(inPos,true);
				if (bagNum > 0) {
					auto item = xmlBag::getInstance()->getNameFromArrangement(bagNum);
					if (item != nullptr) {
						int mixState = xmlItem::getInstance()->checkMixingXML(name, item);
						if (mixState >= 0) { //mix correctly==============================
							_propSprite->setVisible(false);
							xmlBag::getInstance()->setBagState(num, false); // item being dragged is used
							SetCanUse(false); //item cannot be used again
							SetUsed(true);

							_isdrag = false;
							return (mixState + 5);
						}
					}

				}

			}
		}


		//when obj not used (back to its position in bag)
		this->setPosition(_ResetPos);
		_bTouch = false;
		_isdrag = false;
		
	}
	return(0);
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



int CItem::detectUse(cocos2d::Point pos, bool state ) {
	//for bag state=1
	if (!state) {
		if (pos.y > 30.0f) {
			if (pos.y < 200.0f) {
				if (pos.x > 300.0f && pos.x < 1748.0f) {
					int intpart = 0;
					auto x = pos.x - 300.0f;
					x = x / (43 + 170);

					intpart = (int)x;
					float decpart = x - intpart;

					if (decpart < 0.798f) return intpart + 1;
					else return -1;

				}
				else return -1;
			}
			else return -1;
		}
		else return -1;
	}
	
	//for bag state=2
	else {
		if (pos.x > 300.0f) {
			if (pos.x < 1236.0f) {
				if (pos.y > 255.0f && pos.y < 1279.0f) {
					int intpartX = 0;
					auto x = pos.x - 300.0f;
					x = x / (43 + 170);
					intpartX = (int)x;
					float decpartX = x - intpartX;

					int intpartY = 0;
					auto y = pos.y - 255.0f;
					y = y / (43 + 170);
					intpartY = (int)y;
					float decpartY = y - intpartY;

					if (decpartY < 0.798f && decpartX < 0.798f) {
						intpartY = intpartY + 1;
						intpartX = intpartX + 1;
						
						switch (intpartY){

						case 1:
							return (7*4+intpartX);
							break;
						case 2:
							return (7 * 3 + intpartX);
							break;
						case 3:
							return (7 * 2 + intpartX);
							break;
						case 4:
							return (7+ intpartX);
							break;

						case 5:
							return (intpartX);
							break;
						default:
							break;
						}
					}


					else return -1;

				}
				else return -1;
			}
			else return -1;
		}
		else return -1;
	}


}

bool CItem::detectDrag()
{
	return _isdrag;
}
