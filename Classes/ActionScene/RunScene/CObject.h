#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CObject : public cocos2d::Node
{
private:
	int _num;		// 所包含可切換的物件個數
	cocos2d::Node   *_objRoot;	// 每一個物件包含三個可以切換顯示的物件
	cocos2d::Sprite *_objSprite[3];  // 每一個物件的顯示主體
	cocos2d::Point  _LocPt;		// 每一個物件的預設位置


	int _inum;
public:
	CObject();
	static CObject* create(); // 建立物件的實體
	void init(const char* csbfilename, int n, const cocos2d::Point &loc);
	void resetObj();


};