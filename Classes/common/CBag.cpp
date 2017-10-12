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
	_bagSprite = nullptr;
	_ItemNum = 0;

	_bfull = false;

}

CBag::~CBag() {

	//if (_xmlbag != nullptr) delete _xmlbag;
}

void CBag::Init(const std::string& pic,Point pos) {
	_bagSprite = Sprite::createWithSpriteFrameName(pic);
	_bagSprite->setPosition(1024-pos.x, pos.y);
	this->addChild(_bagSprite);
	this->setPosition(pos);

	//道具
	for (size_t i = 0; i < ItemNum; i++)
	{
		_obj[i] = CItem::create();
		this->addChild(_obj[i]);

	}

	_posY = pos.y;


	//_xmlbag = new xmlBag("./res/xml/xmlfile_bag.xml");
	xmlBag::getInstance()->parseXML();

	this->schedule(CC_SCHEDULE_SELECTOR(CBag::doStep));

}
Point pt;
void CBag::doStep(float dt) {

	std::list <CItem*> ::iterator it3;
	int a;
	//for (it3 = _gotItems.begin(), a = 1; it3 != _gotItems.end(); it3++, a++)
	//{
	//
	//	pt = (*it3)->getPosition();
	//	//log("dostep %d :x= %f, y=%f", a, pt.x, pt.y);
	//	log("_btouch %d :%d", a, (*it3)->_bTouch);
	//}


	if (_ItemNum != _gotItems.size()) {
		_ItemNum = _gotItems.size();
		ArrangeItem();
	}
}

void CBag::GetItem(CItem *x) {
	_gotItems.push_back(x);


	std::list <CItem*> ::iterator it;
	int a;
	for (it = _gotItems.begin(), a=1; it != _gotItems.end(); it++,a++)
	{

		pt = (*it)->getPosition();
		log("%d :x= %f, y=%f",a, pt.x ,pt.y);
		//(*it)->SetBagPos(this->getPosition());


	}

}

void CBag::DeleteItem(CItem *x) {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); )
	{
		if (*it == x)
		{
			_gotItems.erase(it++);
		}
		else
			it++;
	}
}


void CBag::ArrangeItem() {
	std::list <CItem*> ::iterator it;
	int i;

	for (it = _gotItems.begin(), i = 1; it != _gotItems.end(), (i-1) < _gotItems.size(); ++it,i++) 
	{
		(*it)->SetPos(Point((210.0f*i), _posY));
		(*it)->SetRect(this->getPosition());

		//(*it)->
		this->addChild(*it);
		log("dd");
	}

	/*std::list <CItem*> ::iterator it2;
	int a;
	for (it2 = _gotItems.begin(), a = 1; it2 != _gotItems.end(); it2++, a++)
	{

		pt = (*it2)->getPosition();
		log("arranged %d :x= %f, y=%f", a, pt.x, pt.y);
	}*/
}

void CBag::MoveX(float x) {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); it++) 
	{
		(*it)->MoveX(x);
		//(*it)->SetRect();
		(*it)->SetRect(this->getPosition());
	}
}

void CBag::SetItemRect() {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); it++)
	{

		//(*it)->SetRect();

		(*it)->SetRect(this->getPosition());
		(*it)->SetBagPos(this->getPosition());


	}
}

void CBag::ResetItemPos() {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); it++)
	{
		(*it)->SetPos((*it)->GetResetPos());
	}
}







// for 控制bag 裡的 item   API used in main scene

void  CBag::AddObj(const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant, bool canRetake, CTrigger* trigger) {
	
	int itemNo = xmlBag::getInstance()->getBagState(); // get bag num

	if (itemNo >= 0) {
		log("itemNo = %d", itemNo);


		_obj[itemNo]->Clear();
		_obj[itemNo]->Init(pic); //set bag item

		for (int i = 0; i < numTarget; i++) {
			_obj[itemNo]->SetTarget(target[i]); //set target rect
		}

		this->addChild(_obj[itemNo]);

		GetItem(_obj[itemNo]); // put in list
		ArrangeItem();
		_obj[itemNo]->SetVisible(true); // shows in bag
		_obj[itemNo]->SetCanUse(true);

		_obj[itemNo]->SetUsed(false);

		_obj[itemNo]->SetStagnant(isStagnant); // set it to be always in bag

		if (canRetake) _obj[itemNo]->SetRetake(trigger);

		log("picSave = %s", pic);
		xmlBag::getInstance()->setBagState(itemNo, true, pic); // save item data
		_bfull = false;
		if(itemNo>=6) _bfull = true;
	}
	
	/*else {
		_bfull = true;
	}*/

	log(" bag full: %d", _bfull);
}


void  CBag::AddObjXML(int inum, const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant, bool canRetake, CTrigger* trigger) {

	_obj[inum]->Clear();
	_obj[inum]->Init(pic); //set bag item

	for (int i = 0; i < numTarget; i++) {
		_obj[inum]->SetTarget(target[i]); //set target rect
	}

	this->addChild(_obj[inum]);

	GetItem(_obj[inum]); // put in list
	_obj[inum]->SetVisible(true); // shows in bag
	_obj[inum]->SetCanUse(true);

	_obj[inum]->SetUsed(false);

	_obj[inum]->SetStagnant(isStagnant); // set it to be always in bag

	if (canRetake) _obj[inum]->SetRetake(trigger);


}


void CBag::reset() {
	std::list <CItem*> ::iterator it;
	for (it = _gotItems.begin(); it != _gotItems.end(); ){

			_gotItems.erase(it++);
	}
	xmlBag::getInstance()->reset();

	for (size_t i = 0; i < ItemNum; i++) {
		_obj[i]->reset();

	}
	_bfull = false;
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
	return(false);
}

bool CBag::touchesMoved(Point inPos) {
	//use items in bag===========================================
	for (size_t i = 0; i < ItemNum; i++)
	{
		if (_obj[i]->GetCanUse())_obj[i]->touchesMoved(inPos);
	}
	return(false);
}

int CBag::touchesEnded(cocos2d::Point inPos) {
	//use items in bag===========================================
	for (size_t i = 0; i < ItemNum; i++)
	{
		if (_obj[i]->touchesEnded(inPos)) {
			_bfull = false;
			log("used obj");
			if (!_obj[i]->GetStagnant()) { // if object is not stagnant
				// when item is being used
				xmlBag::getInstance()->setBagState(i, false); // save item data

				if (_obj[i]->GetRetake()) { //if the item can be retake when it is used
					if(_obj[i]->GetTrigger()!=nullptr)  (_obj[i]->GetTrigger())->SetPicked(false); // set it to be enabled
					else {
						int code = xmlBag::getInstance()->getTriggerCode(i);
						xmlBag::getInstance()->setTriggerChange(_CurrentScene, code, false, false);
					}
				}

				_obj[i]->SetCanUse(false); //item cannot be used again
				DeleteItem(_obj[i]); // delete from bag
				//_obj[i]->
			}



			return i; // i is used for mixing/ doing things
		}

	}
	
	return(-1);
}


void CBag::SetCurrentScene(const char *scene) {
	_CurrentScene = scene;
}

bool CBag::getState() {
	return !_bfull;
}