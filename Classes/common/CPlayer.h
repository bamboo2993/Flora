#pragma once
#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class CPlayer
{
private:

	cocos2d::Sprite *_player;
	cocos2d::Sprite *_body;
	cocos2d::Sprite *_rest;

	cocos2d::Sprite *_sentance;

	cocos2d::Action *_action[2];


	cocos2d::Sequence *_mySequence;
	cocos2d::Action   *_myAction;
	cocos2d::JumpBy   *_myJump;
	int _NoJumps;
	cocos2d::CallFunc *_mycallback;
//	cocos2d::Point _runnerPt;

	bool bFirst;


	cocos2d::Size _contentSize;


public:
	CPlayer(const std::string body, const std::string aniBody, cocos2d::Layer &parent);
	~CPlayer();
	void setPosition(const cocos2d::Point &pos);
	void setPosition(const float x, const float y);
	void setAnimation(const char *plistFile);
	void go(cocos2d::Point pt);
	void Update();
	void Stop();
	void X_Move(float dt, float x);
	void Y_Move(float dt, float y);
	void Talk(const std::string picName,bool isRight);
	void StopTalking();
	void Mirror();
	bool Walk(Point x);
	const Vec2 getPosition();

	bool _bSide;

	Point _rpos;
	Rect _myRect;
	void setRect();
};

#endif