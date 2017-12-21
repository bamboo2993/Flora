#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#ifndef MAX_LOCK_LETTERS
#define MAX_LOCK_LETTERS  5
#endif

class CLock : public cocos2d::Layer
{
private:
	cocos2d::Node *_rootNode;
	cocos2d::Rect	_buttonRect[MAX_LOCK_LETTERS * 2]; //sudoku target rect area
	bool _buttonState[MAX_LOCK_LETTERS*2]; // state if button being pressed

	cocos2d::Sprite *_number[MAX_LOCK_LETTERS];

	int _totDigit; //number of character for password

	int _ansNum; //number of character for password
	int _ans[MAX_LOCK_LETTERS];// the correct password
	int _now[MAX_LOCK_LETTERS]; // number displayed now

	cocos2d::Rect    _lockAreaRect;
	cocos2d::Rect    _exitRect; //exit area

	bool _btouch; 
	bool _type; //
	bool _check; //check if the number now is equal to the password set
	bool _state; // state if the lock is being opened
	bool _solved;

	int _maxNum;
	bool _existZero;
public:
	CLock();
	~CLock();

	virtual bool init(int totDigit, cocos2d::Node *lockNode, int maxNum, bool zero, int keyType);
	
	void SetArea(const cocos2d::Rect &lockarea);

	void SetExitArea(const cocos2d::Rect & lockarea);

	void SetPassword(int num, int ans);
	bool CheckAns();
	void reset();

	bool GetState();

	bool GetSolved();

	void doStep(float dt);


	//Ä²¸I
	bool TouchBegan(const cocos2d::Point pt);
	bool TouchMoved(const cocos2d::Point pt);
	bool TouchEnded(const cocos2d::Point pt);

	// implement the "static create()" method manually
	//CREATE_FUNC(CLock);
};


