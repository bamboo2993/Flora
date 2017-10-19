#pragma once
#include "cocos2d.h"
#include "CItem.h"
#include <list>

#include "CTrigger.h"
#include "xmlScene.h"
#include "xmlBag .h"
#include "xmlItems.h"

#define ItemNum 7

USING_NS_CC;

class CBag :public Node
{
private:
	const char *_CurrentScene;


	static CBag *_pBag;
	Sprite* _bagSprite;
	std::list<CItem*>_gotItems;
	int _ItemNum;

	int _posY;
	
	//item in bag==================
	CItem *_obj[ItemNum];

	// used within the class
	void GetItem(CItem *x);
	void DeleteItem(CItem *x);

public:
	static CBag* create();
	static CBag* getInstance();
	void destroyInstance(); // ������o�귽

	CBag();
	~CBag();


	void Init(const std::string& pic,Point pos, CTrigger* trigger);
	void ArrangeItem();
	void doStep(float dt);
	void MoveX(float x); // swipe right/left
	void SetItemRect();

	//must be added to main scene. where bag is closed
	void ResetItemPos();

	void SetCurrentScene(const char *scene);

	void reset();



	void SetTarget(cocos2d::Rect target);


	//����bag �̪� item
	void AddObj(const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant = false, bool canRetake = false); // ��item �[�i bag ��
	void AddObjXML(int inum, const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant, bool canRetake);

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	int touchesEnded(cocos2d::Point inPos, const char* scene, CTrigger *trigger);
};

