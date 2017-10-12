#pragma once
#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "cocostudio/CocoStudio.h"

class CObstacle {

private:
	int _num;		// �ҥ]�t�i����������Ӽ�
	cocos2d::Node   *_objRoot;	// �C�@�Ӫ���]�t�T�ӥi�H������ܪ�����
	cocos2d::Sprite *_objSprite[3];  // �C�@�Ӫ�����ܥD��
	cocos2d::Point  _LocPt;		// �C�@�Ӫ��󪺹w�]��m

	cocos2d::Point _SetPt; //�C�@�Ӫ��󪺥ͦ���m


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