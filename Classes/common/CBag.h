#pragma once
#include "cocos2d.h"
#include "CItem.h"
#include <list>

#include "xmlBag .h"
#include "common\CTrigger.h"


#define ItemNum 7

USING_NS_CC;

class CBag :public Node
{
private:
	//xmlBag *_xmlbag;
	const char *_CurrentScene;


	static CBag *_pBag;
	Sprite* _bagSprite;
	std::list<CItem*>_gotItems;
	int _ItemNum;

	int _posY;

	//bool _bfull;
	
	//item in bag==================
	CItem *_obj[ItemNum];

	// used within the class
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
	void doStep(float dt);
	void MoveX(float x); // swipe right/left
	void SetItemRect();

	//must be added to main scene. where bag is closed
	void ResetItemPos();

	void SetCurrentScene(const char *scene);

	void reset();



	void SetTarget(cocos2d::Rect target);


	//控制bag 裡的 item
	void AddObj(const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant = false, bool canRetake = false, CTrigger* trigger= nullptr); // 把item 加進 bag 裡
	void AddObjXML(int inum, const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant, bool canRetake, CTrigger* trigger = nullptr);

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	int touchesEnded(cocos2d::Point inPos);
};

