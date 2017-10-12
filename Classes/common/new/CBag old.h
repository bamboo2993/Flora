#pragma once
#include "cocos2d.h"
#include "CItem.h"
#include <list>


#define ItemNum 20

USING_NS_CC;

class CBag :public Node
{
private:
	static CBag *s_instance;
	Sprite* _bagSprite;
	std::list<CItem*>_gotItems;
	int _ItemNum;



	//item in bag==================
	CItem *_obj[ItemNum];

	// used within the class


public:
	//static CBag* instance();
	static CBag* create();
	CBag();
	~CBag();

	void Init(const std::string& pic,Point pos);
	void ArrangeItem();
	void doStep(float dt);
	void MoveX(float x); // swipe right/left
	void SetItemRect();

	//must be added to main scene. where bag is closed
	void ResetItemPos();

	void GetItem(CItem *x);
	void DeleteItem(CItem *x);



	void SetTarget(cocos2d::Rect target);


	//控制bag 裡的 item
	void AddObj(const char* pic, int numTarget, cocos2d::Rect *target, bool isStagnant = false, bool canRetake = false, CTrigger* trigger= nullptr); // 把item 加進 bag 裡


	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	int touchesEnded(cocos2d::Point inPos);
};

