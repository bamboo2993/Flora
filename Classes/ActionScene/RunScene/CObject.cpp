#include "CObject.h"

#define SHIFT_X 30

USING_NS_CC;
using namespace cocostudio::timeline;
// 建立角色之前，必須先確定已經讀入儲存角色所有動態圖片的 plist 檔

CObject::CObject() {
	for (int i = 0; i < 3; i++) _objSprite[i] = nullptr;
}



CObject* CObject::create()
{
	CObject *fgobj = new (std::nothrow) CObject();
	if (fgobj)
	{
		fgobj->autorelease();       //	加入自動釋放資源的行列中
		return fgobj;               // 傳回所取得的物件實體
	}
	CC_SAFE_DELETE(fgobj);
	return nullptr;
}

void CObject::init(const char* csbfilename, int n, const cocos2d::Point &loc)
{
	_num = n;
	// 讀取並建立物件的內容
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;

	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// 在亂數的改變一下 X 軸座標
	_objRoot->setPosition(pt);
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
	this->addChild(_objRoot);           // 將所建立的 _objRoot 節點，加入這個物件的本身成為他的 child









}

// 改變物件的位置與顯示的內容
void CObject::resetObj()
{
	Point pt = _LocPt;
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);
	_objRoot->setPosition(pt);
	_inum = _inum% _num;
	switch (_inum)
	{
	case 0:
		_objSprite[0]->setVisible(true);
		_objSprite[1]->setVisible(false);
		if (_num == 3)_objSprite[2]->setVisible(false);

		break;
	case 1:
		_objSprite[0]->setVisible(false);
		_objSprite[1]->setVisible(true);
		if (_num == 3)_objSprite[2]->setVisible(false);
		break;
	case 2:
		_objSprite[0]->setVisible(false);
		_objSprite[1]->setVisible(false);
		if (_num == 3)_objSprite[2]->setVisible(true);
		break;
	default:
		break;
	}

}


