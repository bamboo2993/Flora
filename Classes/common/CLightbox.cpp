#include "CLightbox.h"


using namespace cocos2d;


CLightbox* CLightbox::create() {
	CLightbox* obj = new(std::nothrow)CLightbox();
	if (obj) {
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

CLightbox::CLightbox(){

	_btouch = false;
	_isOpen = false;
}


bool CLightbox::init(Node *rootNode, const std::string& item, const std::string& enlarge) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//add dim layer
	CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 170));
	this->addChild(bgLayer);

	
	//set item sprite =================================================================================
	_item = (cocos2d::Sprite*)rootNode->getChildByName(item);

	
	Size size = _item->getContentSize();
	Point pos = _item->getPosition();
	_itemRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//set enlarge sprite =================================================================================

	_enlarge = (cocos2d::Sprite *) Sprite::create(enlarge);
	_enlarge->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	this->addChild(_enlarge, 1);
	size = _enlarge->getContentSize();
	Point pt = _enlarge->getPosition();
	_enlargeRect = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);


	this->setVisible(false);


	return true;
}


void CLightbox::doStep(float dt) {
	if (_btouch && _itemRect.containsPoint(_touchpt)) {
		log("haaaa");
		/*daa->editItemState("kitchen", "obj_apple", false);*/
		
		_btouch = !_btouch;
		_isOpen = !_isOpen;
		this->setVisible(true);
		log("open_procedure");
	}

	//_bTouch = false;
}


bool CLightbox::TouchBegan(const cocos2d::Point pt) {
	log("TouchLB");
	_touchpt = pt;
	if (_itemRect.containsPoint(_touchpt) && !_isOpen) {
		_btouch = true;
		log("go_procedure");
		return true;
	}
	else if(!_enlargeRect.containsPoint(_touchpt) && _isOpen) {
		this->setVisible(false);
		_btouch = true;
		_isOpen = !_isOpen;
		log("close_procedure");
		return false;
	}

}

bool CLightbox::GetOpen() {
	return(_isOpen);
}