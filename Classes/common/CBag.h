#pragma once
#include "cocos2d.h"
#include "CItem.h"
#include <list>

#include "CTrigger.h"
#include "xmlScene.h"
#include "xmlBag .h"
#include "xmlItems.h"
#include "CLightbox.h"

#define ItemNum 23 // max num of item

USING_NS_CC;

class CBag :public Node
{
private:
	//set up
	static CBag *_pBag;

	Sprite* _bagSprite[2]; 
	Sprite* _bagButton[3];
	Rect _button[2];
	CLightbox *_itemDetail;

	CItem *_obj[ItemNum];
	std::list<CItem*>_gotItems;
	int _ItemNum; // current number of items in bag
	int _bagState;

	int _pageNum; // current page
	int _pageTot; // total page




	const char *_CurrentScene;	


	//item in bag==================


	// used within the class-------
	void GetItem(CItem *x);
	void DeleteItem(CItem *x);




public:
	static CBag* create();
	static CBag* getInstance();
	void destroyInstance(); // 釋放取得資源

	CBag();
	~CBag();


	void Init(const std::string& pic,Point pos, CTrigger* trigger);
	void ArrangeItem();
	void ArrangeItemStateTwo();
	void doStep(float dt);
	void MoveX(float x); // swipe right/left
	void Move(bool direction); // true: page down, false: page up
	void Move(int page);

	void ToStateTwo();	
	void ToStateOne();


	void SetCurrentScene(const char *scene);
	bool itemdrag();

	void reset();



	//void SetTarget(cocos2d::Rect target);


	//控制bag 裡的 item
	void AddObj(const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant = false, bool canRetake = false); // 把item 加進 bag 裡
	void AddObjXML(int inum, const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant, bool canRetake);

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	int touchesEnded(cocos2d::Point inPos, int bagstate, const char* scene, CTrigger *trigger);
	bool LightboxState();
};

