#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CObject : public cocos2d::Node
{
private:
	int _num;		// �ҥ]�t�i����������Ӽ�
	cocos2d::Node   *_objRoot;	// �C�@�Ӫ���]�t�T�ӥi�H������ܪ�����
	cocos2d::Sprite *_objSprite[3];  // �C�@�Ӫ�����ܥD��
	cocos2d::Point  _LocPt;		// �C�@�Ӫ��󪺹w�]��m


	int _inum;
public:
	CObject();
	static CObject* create(); // �إߪ��󪺹���
	void init(const char* csbfilename, int n, const cocos2d::Point &loc);
	void resetObj();


};