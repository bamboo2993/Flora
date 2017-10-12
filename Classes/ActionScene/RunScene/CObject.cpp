#include "CObject.h"

#define SHIFT_X 30

USING_NS_CC;
using namespace cocostudio::timeline;
// �إߨ��⤧�e�A�������T�w�w�gŪ�J�x�s����Ҧ��ʺA�Ϥ��� plist ��

CObject::CObject() {
	for (int i = 0; i < 3; i++) _objSprite[i] = nullptr;
}



CObject* CObject::create()
{
	CObject *fgobj = new (std::nothrow) CObject();
	if (fgobj)
	{
		fgobj->autorelease();       //	�[�J�۰�����귽����C��
		return fgobj;               // �Ǧ^�Ҩ��o���������
	}
	CC_SAFE_DELETE(fgobj);
	return nullptr;
}

void CObject::init(const char* csbfilename, int n, const cocos2d::Point &loc)
{
	_num = n;
	// Ū���ëإߪ��󪺤��e
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;

	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// �b�üƪ����ܤ@�U X �b�y��
	_objRoot->setPosition(pt);
	for (int i = 0; i < n; i++)
	{
		char objname[50];
		sprintf(objname, "object%1d", i + 1);
		_objSprite[i] = (Sprite *)_objRoot->getChildByName(objname);
		_objSprite[i]->setVisible(false);
	}
	// �H���M�w�@�Ӭ� true
	_inum = rand() % n;
	_objSprite[_inum]->setVisible(true);
	this->addChild(_objRoot);           // �N�ҫإߪ� _objRoot �`�I�A�[�J�o�Ӫ��󪺥��������L�� child









}

// ���ܪ��󪺦�m�P��ܪ����e
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


