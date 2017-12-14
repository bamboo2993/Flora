#include "CMeta.h"


USING_NS_CC;

CMeta *CMeta::_pMeta = nullptr;

CMeta* CMeta::create() {
	CMeta* meta = new(std::nothrow)CMeta();
	if (meta) {
		meta->autorelease();
		return meta;
	}
	CC_SAFE_DELETE(meta);
	return nullptr; 
}

CMeta* CMeta::getInstance() {
	if (_pMeta ==nullptr) _pMeta = new(std::nothrow)CMeta();
	return _pMeta;

}


void CMeta::destroyInstance() { CC_SAFE_DELETE(_pMeta); }

CMeta::CMeta() {
	
}

CMeta::~CMeta() {

	//if (_xmlbag != nullptr) delete _xmlbag;
}

void CMeta::Init(Point pos, CTrigger* trigger) {
	

}

