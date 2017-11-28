#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "common\CButton.h"


class CLock : public cocos2d::Layer
{
private:
	CButton *_button[6];
	cocos2d::Sprite *_numberA[10];
	cocos2d::Sprite *_numberB[10];
	cocos2d::Sprite *_numberC[10];

	int _nowA, _nowB, _nowC; // number displayed now
	int _ansA, _ansB, _ansC; // the correct password

	cocos2d::Rect    _bgRect;

	cocos2d::Rect    _lockAreaRect;

	bool _btouch; 
	//bool _type; // 0:up down key type, 1:calculator type
	bool _check; //check if the number now is equal to the password set
	bool _state; // state if the lock is being opened

	cocos2d::LabelTTF *label;


public:
	CLock(bool type=false);
	~CLock();

	virtual bool init(const char *bg = NULL);
	
	void SetArea(const cocos2d::Rect &lockarea);
	void SetPassword(int a, int b, int c);
	void reset();

	bool GetState();

	void doStep(float dt);


	//Ä²¸I
	bool TouchBegan(const cocos2d::Point pt);
	bool TouchMoved(const cocos2d::Point pt);
	bool TouchEnded(const cocos2d::Point pt);

	// implement the "static create()" method manually
	//CREATE_FUNC(CLock);
};


