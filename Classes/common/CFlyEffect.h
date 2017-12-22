#pragma once
#include "cocos2d.h"

#define GRAVITY -0.09

USING_NS_CC;

class CFlyEffect :public Node
{
private:
	Sprite *_pic;
	Point _endPos;
	float dx, dy, vx, vy;
public:
	
	CFlyEffect();
	~CFlyEffect();

	void setPic(const std::string& picImage);

	void setFly(Point pos);
	bool doStep();
	
};

