#include "CBag.h"


USING_NS_CC;

CBag *CBag::_pBag = nullptr;

CBag* CBag::create() {
	CBag* bag = new(std::nothrow)CBag();
	if (bag) {
		bag->autorelease();
		return bag;
	}
	CC_SAFE_DELETE(bag);
	return nullptr; 
}

CBag* CBag::getInstance() {
	if (_pBag==nullptr) _pBag = new(std::nothrow)CBag();
	return _pBag;

}


void CBag::destroyInstance() { CC_SAFE_DELETE(_pBag); }

CBag::CBag() {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/bagItem.plist");
	_bagSprite[1] = nullptr;
	_bagSprite[0] = nullptr;
	_ItemNum = 0;
	_pageNum = 1;
	_pageTot = 1;
	_bagState = 0;
}

CBag::~CBag() {

	//if (_xmlbag != nullptr) delete _xmlbag;
}

void CBag::Init(Point pos, CTrigger* trigger) {
	_pageNum = 1;


	_bagSprite[1] = Sprite::create("common/bag02.png");  // 使用 create 函式,給予檔名即可
	_bagSprite[1]->setPosition(1024 - pos.x, 768.0f - 115.0f);

	_bagSprite[1]->setVisible(false);
	this->addChild(_bagSprite[1], 0);


	//set lightbox (item detail) =================================================================
	_itemDetail = CLightbox::create();
	_itemDetail->init();
	_itemDetail->setPosition(-pos.x, -115.0f);
	this->addChild(_itemDetail, 1);

	
	_bagSprite[0] = Sprite::createWithSpriteFrameName("bag.png");
	_bagSprite[0]->setPosition(1024-pos.x, 0);
	this->addChild(_bagSprite[0], 2);

	_bagButton[0] = Sprite::createWithSpriteFrameName("arrow_up.png");
	_bagButton[0]->setPosition(1024 - pos.x, 115+60);
	_bagButton[1] = Sprite::createWithSpriteFrameName("arrow_left.png");
	_bagButton[2] = Sprite::createWithSpriteFrameName("arrow_right.png");
	_bagButton[2]->setPosition((213.0f*8), 0);
	this->addChild(_bagButton[0], 2);
	this->addChild(_bagButton[1], 2);
	this->addChild(_bagButton[2], 2);
	Size size = _bagButton[1]->getContentSize();
	Point position = _bagButton[1]->getPosition();
	_button[0] = Rect(172.0f - size.width / 2, 115.0f - size.height / 2, size.width, size.height);
	position = _bagButton[2]->getPosition();
	_button[1] = Rect(position.x + 172.0f - size.width / 2, position.y + 115.0f - size.height / 2, size.width, size.height);


	//道具
	for (size_t i = 0; i < ItemNum; i++){
		_obj[i] = CItem::create();
		_obj[i]->Init("B_akey.png");
		this->addChild(_obj[i], 2);

	}

	this->setPosition(pos);

	xmlBag::getInstance()->parseXML(trigger);

	this->schedule(CC_SCHEDULE_SELECTOR(CBag::doStep));

}

void CBag::doStep(float dt) {

	if (_ItemNum != _gotItems.size()) {
		_ItemNum = _gotItems.size();
		if (_bagState == 1 || !_bagState) ArrangeItem();
		else if (_bagState == 2) ArrangeItemStateTwo();
	}
}

void CBag::GetItem(CItem *x) {
	_gotItems.push_back(x);

}

void CBag::DeleteItem(CItem *x) {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); ){
		if (*it == x){
			_gotItems.erase(it++);
		}
		else
			it++;
	}
}


void CBag::ArrangeItem() {
	std::list <CItem*> ::iterator it;
	int i;
	for (it = _gotItems.begin(), i = 1; it != _gotItems.end(), (i-1) < _gotItems.size(); ++it,i++) {
		xmlBag::getInstance()->setArrangementXML((*it)->GetName(), i);
		
		auto divide = (int) i / 7;
		auto dividedeci = i % 7;

		if (dividedeci == 0) divide--;
		(*it)->SetPos(Point((213.0f*i)+(557.0f*divide), 0));
		(*it)->SetRect(this->getPosition());
		(*it)->SetVisible(true); // shows in bag

		this->addChild(*it);
		log("arrange items in bag");


		_pageTot = divide + 1;
	}

	Move(_pageNum); //flip to the current page
}

void CBag::ArrangeItemStateTwo() {
	std::list <CItem*> ::iterator it;
	int i, j;
	for (it = _gotItems.begin(), i = 1; it != _gotItems.end(), (i - 1) < _gotItems.size(); ++it, i++) {
		xmlBag::getInstance()->setArrangementXML((*it)->GetName(), i);

		auto divide = (int)i / 7;
		auto dividedeci = i % 7;

		if (dividedeci == 0) {
			divide--;
			(*it)->SetPos(Point((213.0f * 7), 1079.0f - (213.0f*divide)));
		}
		else (*it)->SetPos(Point((213.0f*dividedeci), 1079.0f - (213.0f*divide)));
		(*it)->SetRect(this->getPosition());
		(*it)->SetVisible(true); // shows in bag

		this->addChild(*it);
		log("arrange items in bag [STATE TWO]");
	}
}


void CBag::MoveX(float x) {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); it++){
		(*it)->MoveX(x);
		//(*it)->SetRect();
		(*it)->SetRect(this->getPosition());
	}
}


void CBag::Move(bool direction) {
	std::list <CItem*> ::iterator it;
	//page down
	if (direction) {
		for (it = _gotItems.begin(); it != _gotItems.end(); it++) {
			(*it)->MoveX(-2048.0f);
			(*it)->SetRect(this->getPosition());
		}

		_pageNum ++;
	}
	else {
		for (it = _gotItems.begin(); it != _gotItems.end(); it++) {
			(*it)->MoveX(2048.0f);
			(*it)->SetRect(this->getPosition());
		}

		_pageNum--;
	}


}


void CBag::Move(int page) {
	std::list <CItem*> ::iterator it;
	//page down
	if (page > 1) {
		for (it = _gotItems.begin(); it != _gotItems.end(); it++) {
			(*it)->MoveX(-2048.0f*(page - 1));
			(*it)->SetRect(this->getPosition());
		}
	}


}

void CBag::ToStateTwo(){
	_bagSprite[0]->setVisible(false);
	_bagSprite[1]->setVisible(true);

	_bagButton[0]->setVisible(false);
	_bagButton[1]->setVisible(false);
	_bagButton[2]->setVisible(false);

	ArrangeItemStateTwo();

	
}

void CBag::ToStateOne(){
	_bagSprite[0]->setVisible(true);
	_bagSprite[1]->setVisible(false);


	_bagButton[0]->setVisible(true);
	_bagButton[1]->setVisible(true);
	_bagButton[2]->setVisible(true);

	ArrangeItem();
}


// for 控制bag 裡的 item   API used in main scene

void  CBag::AddObj(const char* pic, int numTarget, bool isStagnant, cocos2d::Rect *target, bool canRetake) {
	
	int itemNo = xmlBag::getInstance()->getBagState(); // get bag num

	log("itemNo = %d", itemNo);

	if (itemNo >= 0) {
		_obj[itemNo]->ChangePic(pic); //set bag item

		for (int i = 0; i < numTarget; i++) {
			_obj[itemNo]->SetTarget(target[i]); //set target rect
		}

		//_obj[itemNo]->SetVisible(true); // shows in bag
		_obj[itemNo]->SetCanUse(true);
		_obj[itemNo]->SetUsed(false);
		_obj[itemNo]->SetStagnant(isStagnant); // set it to be always in bag
		if (canRetake) _obj[itemNo]->SetRetake();

		log("picSave = %s", pic);
		xmlBag::getInstance()->setBagState(itemNo, true, pic); // save item data


		this->addChild(_obj[itemNo]);
		GetItem(_obj[itemNo]); // put in list
	}

}


void  CBag::AddObjXML(int inum, const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant, bool canRetake) {

	_obj[inum]->ChangePic(pic); //set bag item

	for (int i = 0; i < numTarget; i++) {
		_obj[inum]->SetTarget(target[i]); //set target rect
	}

	//_obj[inum]->SetVisible(true); // shows in bag
	_obj[inum]->SetCanUse(true);
	_obj[inum]->SetUsed(false);
	_obj[inum]->SetStagnant(isStagnant); // set it to be always in bag
	if (canRetake) _obj[inum]->SetRetake();

	this->addChild(_obj[inum]);
	GetItem(_obj[inum]); // put in list


}


bool CBag::itemdrag(){

	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); it++){
		if ((*it)->detectDrag()) return true;
	}

	return false;
}

void CBag::reset() {
	_ItemNum = 0;
	_pageNum = 1;
	_pageTot = 1;

	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); ){

			_gotItems.erase(it++);
	}
	xmlBag::getInstance()->reset();

	for (size_t i = 0; i < ItemNum; i++) {
		_obj[i]->reset();

	}
}


bool CBag ::touchesBegan(Point inPos) {

	//use items in bag===========================================
	for (size_t i = 0; i < ItemNum; i++){
		if (_obj[i]->GetCanUse() == true) {
			if (_obj[i]->touchesBegan(inPos)) {
				log("item touched (bag)");
			}
		}
		
	}

	if (_bagState==1) {
		if (_button[0].containsPoint(inPos)) {
			log("flip page?");
			if (_pageNum > 1) {
				log("page up");
				Move(false);
				log("page no: %d", _pageNum);
			}
		}

		else if (_button[1].containsPoint(inPos)) {
			log("flip page?");
			if (_pageNum < _pageTot) {
				log("page down");
				Move(true);
				log("page no: %d", _pageNum);
			}

		}
	}
	
	return(false);
}

bool CBag::touchesMoved(Point inPos) {
	//use items in bag===========================================
	for (size_t i = 0; i < ItemNum; i++){
		if (_obj[i]->GetCanUse())_obj[i]->touchesMoved(inPos, _itemDetail->GetOpen());
	}
	return(false);
}

int CBag::touchesEnded(cocos2d::Point inPos, int bagstate, const char* scene, CTrigger *trigger) {
	_bagState = bagstate;
	//use items in bag===========================================
	for (size_t i = 0; i < ItemNum; i++){

		if (_obj[i]->GetCanUse() == true) {
			auto type = _obj[i]->touchesEnded(inPos, _bagState, scene, i,_pageNum, _itemDetail->GetOpen());
			//[USE OBJECT IN SCENE]==================
			if (type == 1) {
				log("used obj");
				if (!_obj[i]->GetStagnant()) { // if object is not stagnant
											   // when item is being used
					xmlBag::getInstance()->setBagState(i, false); // save item data

					if (_obj[i]->GetRetake()) { //if the item can be retake when it is used
						auto name = xmlBag::getInstance()->getItemName(i);
						int code = xmlBag::getInstance()->getTriggerCode(i); //read data from xml (get item's trigger code)
						auto triggerScene = xmlItem::getInstance()->getTriggerSceneXML(name);

						auto ret = strcmp(triggerScene, scene);

						if (ret) {
							//when trigger scene is not current scene, save changes in xml
							xmlTrigger::getInstance()->setTriggerStateXML(triggerScene, code, true);
						}
						else trigger[code].SetPicked(false);
					}

					_obj[i]->SetCanUse(false); //item cannot be used again
					DeleteItem(_obj[i]); // delete from bag
				}



				return i; // i is used for mixing/ doing things
			}
			//xmlBag::getInstance()->getNameFromArrangement
			//[ITEM DETAIL + MIX ITEM]
			else if (type >1) {
				//see item detail --------------------
				if (type == 1000) {

					// observe item
					auto detail = xmlItem::getInstance()->getItemDescriptionXML(_obj[i]->GetName());
					_itemDetail->TouchBegan(inPos, bagstate, detail);	

					if (bagstate == 2) {

						_itemDetail->setLocalZOrder(3);
					}
					else if(bagstate == 1) {
						_itemDetail->setLocalZOrder(1);
					}

					return(-1);
				}
				//mix item in bag-----------------------------
				auto ToBe = type-5;
				int Border;
				if (_bagState == 1) {
					Border = _obj[i]->detectUse(inPos);
					Border = Border + 7 * (_pageNum - 1);
				}
				else if(_bagState ==2) Border = _obj[i]->detectUse(inPos,true);
				
				auto Bnum = xmlBag::getInstance()->getNumFromArrangement(Border);

				auto newObj = xmlItem::getInstance()->getItemNameXML(ToBe);
				if (!_obj[i]->GetStagnant()) {
					DeleteItem(_obj[i]); // delete the used item from bag
				}
				
				auto oldObj = _obj[Bnum]->GetName();

				//set new item ...........
				_obj[Bnum]->Clear(); //clear origin pic
				_obj[Bnum]->Init(newObj); //set new item
				_obj[Bnum]->SetVisible(true); // shows in bag

				//get data from xml..........
				auto targetNum = xmlItem::getInstance()->getTargetNumXML(newObj);
				cocos2d::Rect target[2];
				target[0] = xmlItem::getInstance()->getTargetRectXML(newObj);
				if (targetNum>1) target[1] = xmlItem::getInstance()->getTargetRectXML(newObj, 2);
				bool isStagnant = xmlItem::getInstance()->getStagnantXML(newObj);
				auto canRetake = xmlItem::getInstance()->getRetakeXML(newObj);

				//set values for obj........
				for (int i = 0; i < targetNum; i++) {
					_obj[Bnum]->SetTarget(target[i]); //set target rect
				}

				_obj[Bnum]->SetStagnant(isStagnant); // set it to be always in bag
				if (canRetake) _obj[Bnum]->SetRetake();

				
 				xmlBag::getInstance()->setBagState(oldObj, newObj); // save item data

				return(-1);
			}
		
		}
		
	}
	if (_itemDetail->GetOpen() && inPos.y> 230.0f) _itemDetail->TouchBegan(inPos, bagstate,"a"); //check itemdetail close

	return(-1);
}

//
//void CBag::SetCurrentScene(const char *scene) {
//	_CurrentScene = scene;
//}
//


bool CBag::LightboxState() {
	return _itemDetail->GetOpen();
}

