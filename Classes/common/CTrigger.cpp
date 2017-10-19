//CTriggers on map

#include "CTrigger.h"


#include "xmlScene.h"

USING_NS_CC;

CTrigger* CTrigger::create() {
	CTrigger* obj = new(std::nothrow)CTrigger();
	if (obj) {
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

CTrigger::CTrigger() {
	_objSprite = nullptr;
	_bTouch = false;
	_bPicked = false;
	_bAddToBag = false;
//	_bDel = false;
}

CTrigger::~CTrigger() {
}

void CTrigger::Init(const char * pic, Node *rootNode, bool visible, bool type, Node *eNode) {
	_btype= type;
	if (!type) { // type 0 (default)
		_item = pic;
		_node = rootNode;

		_objSprite = (cocos2d::Sprite*)_node->getChildByName(_item);
		_objSprite->setVisible(visible);
		SetRect();
		SetVisible(visible);
	}

	else {
		_item = pic;
		_node = eNode;

		_objSprite = (cocos2d::Sprite*)_node->getChildByName(_item);
		_objSprite->setVisible(visible);
		SetRect();


		
	}

}



void CTrigger::SetRect(){
	Size size = _objSprite->getContentSize();
	Point pos = _objSprite->getPosition();
	_objRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
}

void CTrigger::SetVisible(bool x) {
	_bVis = x;
	_objSprite->setVisible(_bVis);
}


void CTrigger::SetPicked(bool x) {
	_bPicked = x;
}
void CTrigger::SetAddToBag(bool x) {
	_bAddToBag = x;
}



void CTrigger::doStep(float dt) {
	if (_bTouch) {
		//log("sssss");
		// save data

		int itemNo = xmlBag::getInstance()->getBagState(); // get bag num and check if there is space
		if (itemNo >= 0) {
			xmlScene *xmlscene = new xmlScene("./res/xml/xmlfile_labScene.xml");
			xmlscene->editItemState(_item, false, _node, 0, 36);

			this->SetVisible(false);
			_bTouch = !_bTouch;
			_bAddToBag = true;

		}

		
  		

	}

}



void CTrigger::reset() {

	_bTouch = false;
	_bPicked = false;
	_bAddToBag = false;
}


bool CTrigger::touchesBegan(Point inPos) {
	if(_objRect.containsPoint(inPos)){
		_bTouch = true;
//		_bDel = true;

		log("touchaa: %d", _bTouch);
		return(true);
	}
	else _bTouch = false;

	return(false);
}

bool CTrigger::touchesEnded(Point inPos) {

	return(false);
}

bool CTrigger::GetVis() {
	return(_bVis);
}

bool CTrigger::GetTouch() {
	return(_bTouch);
}



bool CTrigger::GetPicked() {
	return(_bPicked);
}
bool CTrigger::GetAddToBag() {
	return(_bAddToBag);
}