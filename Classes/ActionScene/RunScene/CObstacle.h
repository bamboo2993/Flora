#pragma once
#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "cocostudio/CocoStudio.h"

class CObstacle {

private:
	int _num;		// 所包含可切換的物件個數
	cocos2d::Node   *_objRoot;	// 每一個物件包含三個可以切換顯示的物件
	cocos2d::Sprite *_objSprite[3];  // 每一個物件的顯示主體
	cocos2d::Point  _LocPt;		// 每一個物件的預設位置

	cocos2d::Point _SetPt; //每一個物件的生成位置


	cocos2d::Point instantpt;
	cocos2d::Rect _obsRect;
	int _inum;
	bool _IsDamage;

	bool _bvisible;
public:
	CObstacle();
	void init(const char* csbfilename, int n, const cocos2d::Point &loc, cocos2d::Layer &parent);
	void resetObj(bool type=false);
	bool collision(cocos2d::Rect runner);

	void doStep(float dt);

	cocos2d::Rect  getRect();
	cocos2d::Point getPosition();
	void resetPos(float x, float y);


	void setVis(bool visible);
	bool getVis();
};