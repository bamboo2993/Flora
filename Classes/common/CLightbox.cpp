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
	for (size_t i = 0; i < MAX_PAGE; i++) {
		_enlarge[i] = nullptr;

	}
	_btouch = false;
	_isOpen = false;
	_bflip = false;
	_ipageNum = 1;
	_icurrentPage = 1;
}

//for items in scene
bool CLightbox::init(Node *rootNode, const std::string& item, const std::string& enlarge) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//add dim layer=
	CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 170));
	this->addChild(bgLayer);

	
	//set item sprite =================================================================================
	_item = (cocos2d::Sprite*)rootNode->getChildByName(item);

	
	Size size = _item->getContentSize();
	Point pos = _item->getPosition();
	_itemRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//set enlarge sprite =================================================================================

	_enlarge[0] = (cocos2d::Sprite *) Sprite::create(enlarge);
	_enlarge[0]->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	this->addChild(_enlarge[0], 1);
	size = _enlarge[0]->getContentSize();
	Point pt = _enlarge[0]->getPosition();
	_enlargeRect[0] = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);


	this->setVisible(false);

	return true;
}

bool CLightbox::init(cocos2d::Rect area, const std::string& enlarge) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//add dim layer
	CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 170));
	this->addChild(bgLayer);


	//set item sprite =================================================================================

	_itemRect = area;

	//set enlarge sprite =================================================================================

	_enlarge[0] = (cocos2d::Sprite *) Sprite::create(enlarge);
	_enlarge[0]->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	this->addChild(_enlarge[0], 1);
	Size size = _enlarge[0]->getContentSize();
	Point pt = _enlarge[0]->getPosition();
	_enlargeRect[0] = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);


	this->setVisible(false);

	return true;
}



//can flip---------------------------------------
bool CLightbox::init(Node *rootNode, const std::string& item, int totPage) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//add dim layer=
	CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 170));
	this->addChild(bgLayer);


	//set item sprite =================================================================================
	_item = (cocos2d::Sprite*)rootNode->getChildByName(item);


	Size size = _item->getContentSize();
	Point pos = _item->getPosition();
	_itemRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	this->setVisible(false);

	_bflip = true;

	_ipageNum = totPage;
	_icurrentPage = 1;

	_enlarge[0]->setVisible(true);
	for (size_t i = 1; i < _ipageNum; i++) {
		_enlarge[i]->setVisible(false);
	}


	return true;
}


void CLightbox::setPage(int pageNum, const std::string & enlarge) {


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//set enlarge sprite =================================================================================

	_enlarge[pageNum-1] = (cocos2d::Sprite *) Sprite::create(enlarge);
	_enlarge[pageNum - 1]->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	this->addChild(_enlarge[pageNum - 1], 1);
	Size size = _enlarge[pageNum - 1]->getContentSize();
	Point pt = _enlarge[pageNum - 1]->getPosition();
	_enlargeRect[pageNum - 1] = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);
}




void CLightbox::doStep(float dt) {
	// for items in scene
	if (_btouch && _itemRect.containsPoint(_touchpt)) {
		log("haaaa");
		/*daa->editItemState("kitchen", "obj_apple", false);*/
		
		_btouch = !_btouch;
		_isOpen = !_isOpen;
		this->setVisible(true);
		log("open_procedure");
	}

	
}



bool CLightbox::TouchBegan(const cocos2d::Point pt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	log("TouchLB");
	_touchpt = pt;

	// for item in scene==============
	
	if (_itemRect.containsPoint(_touchpt) && !_isOpen) {
		_btouch = true;
		log("go_procedure");
		return true;
	}

	else if (!_enlargeRect[0].containsPoint(_touchpt) && _isOpen) {
		this->setVisible(false);
		_btouch = true;
		_isOpen = !_isOpen;
		log("close_procedure");
		return false;
	}

	if (_bflip) {
		if (_enlargeRect[0].containsPoint(_touchpt) && _isOpen) {
			if (_touchpt.x < visibleSize.width / 2.0f) {
				//left [page down]
				if (_icurrentPage > 1) {
					_enlarge[_icurrentPage-1]->setVisible(false);
					_icurrentPage--;
					_enlarge[_icurrentPage-1]->setVisible(true);
				}
				
			}

			else {
				//right [page up]
				if (_icurrentPage < _ipageNum) {
					_enlarge[_icurrentPage-1]->setVisible(false);
					_icurrentPage++;
					_enlarge[_icurrentPage-1]->setVisible(true);
				}

			}







			log("in_procedure");
			return false;
		}
	}

	
	return false;

}


//for bag item description =================================================================================
bool CLightbox::init() {
	
	//add dim layer
	CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 170));
	this->addChild(bgLayer);

	this->setVisible(false);

	return true;
}

void CLightbox::setPic(const std::string & enlarge){


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (_enlarge[0] != nullptr) {
      		_enlarge[0]->removeFromParentAndCleanup(true);
	}

	//set enlarge sprite =================================================================================

	_enlarge[0] = (cocos2d::Sprite *) Sprite::create(enlarge);
	_enlarge[0]->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	this->addChild(_enlarge[0], 1);
	Size size = _enlarge[0]->getContentSize();
	Point pt = _enlarge[0]->getPosition();
	_enlargeRect[0] = Rect(pt.x - size.width / 2, pt.y - size.height / 2, size.width, size.height);



}

//for bag item
bool CLightbox::TouchBegan(const cocos2d::Point pt, int bagstate, const std::string & enlarge) {
	log("TouchLB");
	_touchpt = pt;

	//for bag item detail ===================

	if (!_isOpen) {
		setPic(enlarge);
		this->setVisible(true);
		_isOpen = true;
		return true;
	}
	else if(bagstate==2){
		if (!_enlargeRect[0].containsPoint(_touchpt) && _isOpen) {
			this->setVisible(false);

			_isOpen = false;
		}
		return false;
	}
	else if (bagstate == 1) {
		if (_touchpt.y > 230.0f) {
			if (!_enlargeRect[0].containsPoint(_touchpt) && _isOpen) {
				this->setVisible(false);
				_isOpen = false;
			}
			return false;
		}
		else setPic(enlarge);
		return true;
	}

	return false;

}

bool CLightbox::GetOpen() {
	return(_isOpen);
}