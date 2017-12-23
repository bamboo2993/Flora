#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class AniScene : public cocos2d::Layer
{
public:
	AniScene();
	~AniScene();
	void init(Point &pt, Layer &parent, int num, char *picname);
	bool doStep(float dt);
	bool skip(float dt);
	void setVis(bool b);

private:
	Sprite *_SpriteCD[10];
	float _tot;
	float _skiptot;
	int idx;
	int picnum;
};