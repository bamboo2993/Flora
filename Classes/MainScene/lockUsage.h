
#pragma once


#ifndef __SELECTION_SCENE_H__
#define __SELECTION_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "common\cButton.h"



#include "common\CLock.h"
#include "common\CeLock .h"

class lockUsage : public cocos2d::Layer
{
private:
	CButton *_StartBtn;
	bool _bToSelectScene;

	CButton *_ExitBtn;
	CButton *_ContinueBtn;
	bool _bToLoadingScene;



	CeLock *_elock;

	CLock *_lock;
	cocos2d::Rect    _lockAreaRect;


public:
	~lockUsage();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void doStep(float dt);

	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 

																	   // implement the "static create()" method manually
	CREATE_FUNC(lockUsage);
};

#endif // __SELECTION_SCENE_H__
