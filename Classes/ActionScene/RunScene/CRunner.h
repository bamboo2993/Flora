#ifndef __CRUNNER_H__
#define __CRUNNER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"



class CRunner 
{
private:
	bool _bgo;
	bool state;

	cocos2d::Size _runnerSize;
	cocos2d::Point _runnerpt;
	cocos2d::Sprite *_runner;
	cocos2d::Rect    runnerRect;

	//set animation======================
	cocos2d::Action *_runaction;
	cocos2d::Action *_jumpaction;
	cocos2d::Action *_squataction;




//cocos2d::Action *_action;

	// Runner Jump========================================
	cocos2d::JumpBy *_jump;
	cocos2d::CallFunc *_callback;


	cocos2d::Point _runnerptInstance;


public:
	int _ilife;

	CRunner(const char *body, cocos2d::Layer &parent);
	~CRunner();
	void setPosition(const cocos2d::Point &pos);
	void setPosition(const float x, const float y);
	void setAnimation(const char *plistFile);

	void setRect();

	//jump==================================
	void jump();
	int _NoJumps =0;
	void jumpFinished();

	void run();
	void squat();
	void stop();

	bool _bdown;
	bool _bup;
	bool _bfast;
	bool _bslow;


	bool TouchBegan(const cocos2d::Point pt);
	bool TouchMoved(const cocos2d::Point pt);
	bool TouchEnded(const cocos2d::Point pt);

	void setGo();

	void doStep(float dt);
	cocos2d::Rect getRect();
	bool getstate();
};

#endif