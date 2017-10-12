#pragma once
#pragma once
#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;

class CFall
{
private:
	Sprite *_obstacle;
	Point _Instancepos;
	Point _Initialpos;
	Rect  _trashRect;
	Node *_node;

	float _vx;
	float _vy;


	bool _IsDamage;
	int _iType;

	bool _state;
public:

	CFall(Node *node, Layer &parent);
	~CFall();
	void setRect();
	void setPosition(const float x, const float y);
	void setScale(float f);
	bool collision( Rect runner);


	void doStep(float dt);
	void resetObj();


	void setstate(bool state);
	bool getstate();

};