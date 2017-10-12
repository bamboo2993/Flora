#pragma once
#include "cocos2d.h"

USING_NS_CC;

using namespace cocos2d;

class CTrigger :public Node
{
private:
	bool _btype;
	Sprite* _objSprite;
	bool _bTouch; //obj touched
	bool _bVis;

	bool _bPicked;   // if the obj is taken
	bool _bAddToBag; // add obj to bag


	const char *_item;
	Node *_node;

public:
	static CTrigger* create();
	CTrigger();
	~CTrigger();
	Rect _objRect;
//	bool _bDel;

	void Init(const char * pic, Node *rootNode, bool visible, bool type=0, Node *eNode=nullptr);

	
	void doStep(float dt);
	void SetRect();
	void SetVisible(bool x);
	void SetPicked(bool x);  //is picked
	void SetAddToBag(bool x); //can picked

	bool GetVis();
	bool GetTouch();
	bool GetPicked(); // if  the obj is picked up
	bool GetAddToBag();

	void reset();

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesEnded(cocos2d::Point inPos);
};

