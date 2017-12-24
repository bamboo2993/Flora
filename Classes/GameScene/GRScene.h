#pragma once
#pragma once
#ifndef __GR_SCENE_H__
#define __GR_SCENE_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "common\CPlayer.h"

#include "common\CTrigger.h"
#include "common\CItem.h"
#include "common\CLightbox.h"

#include "common\xmlTrigger.h"
#include "common\xmlScene.h"

#include "common\CMix.h"

class GRScene : public cocos2d::Layer
{
private:


	// sound effect====================================
	cocostudio::ComAudio *_pour;
	cocostudio::ComAudio *_powder;
	cocostudio::ComAudio *_mixing;
	cocostudio::ComAudio *_grinding;
	cocostudio::ComAudio *_debranch;



	//set up========================================

	cocos2d::Node *_rootNode;

	cocos2d::Node *_zNode[3]; // zoomin node

	cocos2d::Sprite *_win; //stage clear sprite
	bool _clear; //stage clear

	cocos2d::Sprite *_reset;
	cocos2d::Rect	_resetRect; // reset button

	xmlScene *_xmlscene; // xml file [scene]


	//bag=============================================
	int _ibagState;

	//觸控-----------
	cocos2d::Point _touchLoc;
	cocos2d::Point _prePos;
	cocos2d::Point _itempos;
	float _startX, _startY;

	int _iObjUsed;

	bool bb[2] = { false };


	//player===================================
	CPlayer *_player;
	cocos2d::Point _TargetLoc; // for walk pos
	bool _bWalkpos;


	//判斷 walk------------------
	bool _bWalk = false; //detect if player is walking
	bool _bwithinArea; // detect if touch_pts are in walkable area

	//判斷 物品----------------
	bool _bpickObj; //detect if player arrive near object being picked
	bool _btouchNode[3] = { false };//for E_node
	bool _bopenNode[3] = { false };//for E_node
	bool _btouch[5]; // for item in scene

	// set lightbox==============================
	CLightbox *_procedure[4];



	//set object==========================================================
	CTrigger *_pTrigger; // items that can be taken: 藥水以 red, green, blue, yellow 排列、蒸餾水、玻璃棒、草藥


	cocos2d::Sprite *_detect[3]; // detect zoom node
	cocos2d::Rect	_detectRect[3];
	cocos2d::Rect	_closeRect; // area to close node (znode)

	cocos2d::Rect	*_pTsudokuRect; //sudoku target rect area
	cocos2d::Rect	*_pTkeyRect; //key target rect area

	bool _bsolve[2]; //state if the quiz is being solved

	//spots
	Sprite *_spot[3];
	Rect _spotRect[3];
	bool _toSpot[3];
	bool _isWalking;

public:
	GRScene();
	~GRScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	void doStep(float dt);

	void SetObject(); //set objects in scene
	void PickObject(float dt); //pick and save obj

	void reset();

	void ClearToSpot();
	bool ToSpot0(float dt);
	bool ToSpot1(float dt);
	bool ToSpot2(float dt);


	//觸碰
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 


	// implement the "static create()" method manually
	CREATE_FUNC(GRScene);
};

#endif // __HELLOWORLD_SCENE_H__
