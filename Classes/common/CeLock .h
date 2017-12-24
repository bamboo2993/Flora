#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"


#ifndef MAX_LOCK_LETTERS
#define MAX_LOCK_LETTERS  5
#endif

class CeLock : public cocos2d::Layer
{
private:

	cocos2d::Sprite *_number[MAX_LOCK_LETTERS];
	std::list<cocos2d::Sprite*>_typedNum;
	std::list<int>_itypedNum;

	cocos2d::Rect    _buttonRect[12]; //key area
	bool _buttonState[12]; // state if button being pressed


	int _ansNum; //number of character for password
	int _ans[MAX_LOCK_LETTERS];// the correct password

	float _posX, _posY; //position number to appear



	cocos2d::Rect    _lockAreaRect; // trigger lock open
	cocos2d::Rect    _exitRect; //exit area

	bool _btouch; //trigger
	bool _state; // state if the lock is being opened

	bool _check; //check if the number now is equal to the password set


	bool _openState;

	//for error============
	bool _error;
	cocos2d::Sprite *_Error;
	float _tot;

public:
	CeLock();
	~CeLock();

	virtual bool init(const char* bgImage=NULL);
	
	void SetArea(const cocos2d::Rect &lockarea);
	void SetExitArea(cocos2d::Point BLpoint, float width, float height);
	void SetKeyArea(cocos2d::Point BLpoint, float w, float h);
	void SetEnterArea(cocos2d::Point BLpoint, float w, float h);
	void SetNumAppear(float x, float y);
	void SetPassword(int num,int ans); // set atmost 5 digit
	void doStep(float dt);
	void SetReached(bool x);
	void close();
	void reset();

	void GetNum(cocos2d::Sprite * x);
	void DeleteNum();
	void ArrangeNum();

	bool CheckAns();



	bool GetState();

	//Ä²¸I
	bool TouchBegan(const cocos2d::Point pt);
	bool TouchMoved(const cocos2d::Point pt);
	bool TouchEnded(const cocos2d::Point pt); //if return true, switch scene

	// implement the "static create()" method manually
	//CREATE_FUNC(CLock);
};


