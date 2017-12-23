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
	const char* _cstand[2];
	cocos2d::Point _previousPos;
	bool _bfront;

	cocos2d::Sprite *_player;
	cocos2d::Sprite *_rest;
	Sprite* _stand[2];

	cocos2d::Sprite *_sentance;

	cocos2d::Action *_action[2];


	cocos2d::Sequence *_mySequence;
	cocos2d::Action   *_myAction;
	cocos2d::JumpBy   *_myJump;
	int _NoJumps;
	cocos2d::CallFunc *_mycallback;


	bool bStop;

	bool _reachSpot[20];

	cocos2d::Size _contentSize;


public:
	CPlayer(const char*  front, const char*  back, cocos2d::Layer &parent);
	CPlayer(bool isBack, cocos2d::Layer &parent, Point pos, bool isFacingR);
	CPlayer(const std::string body, cocos2d::Layer &parent, Point pos, bool isFacingR);

	~CPlayer();
	void setPosition(const cocos2d::Point &pos);
	void setPosition(const float x, const float y);

	void setPreviousPosition();
	cocos2d::Point getPreviousPosition();

	void setAnimation(const char *plistFile);
	void go(cocos2d::Point pt);
	void go(bool isBack);
	void Update();
	void Stop();
	void Stop(bool isBack);
	void X_Move(float dt, float x);
	void Y_Move(float dt, float y);
	void Talk(const std::string picName,bool isRight);
	void StopTalking();
	void Mirror();
	void Mirror(bool isFacingR);
	bool Walk(Point x);
	const Vec2 getPosition();
	void SetReachSpot(int n, bool f);
	bool GetReachSpot(int n);

	bool _isFacingRight;

	Point _rpos;
	Rect _myRect;
	void setRect();
};

#endif