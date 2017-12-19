#pragma once
#pragma once
#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__


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

class BMScene : public cocos2d::Layer
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

	cocos2d::Sprite *_bkBlur; // blur background sprite
	cocos2d::Node *_eNode[2]; // ��j�� node

	cocos2d::Sprite *_win; //stage clear sprite
	bool _clear; //stage clear

	cocos2d::Sprite *_reset;
	cocos2d::Rect	_resetRect; // reset button

	xmlScene *_xmlscene; // xml file [scene]


	//bag=============================================
	int _ibagState;

	//Ĳ��-----------
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


	//�P�_ walk------------------
	bool _bWalk = false; //detect if player is walking
	bool _bwithinArea; // detect if touch_pts are in walkable area

	//�P�_ ���~----------------
	bool _bpickObj; //detect if player arrive near object being picked
	bool _btouchNode[2] = { false };//for E_node
	bool _bopenNode[2] = { false };//for E_node
	bool _btouch[5]; // for item in scene

	bool _bbOnFire[2]; // if beaker A/B is on flame

	// set lightbox==============================
	CLightbox *_procedure;



	//set object==========================================================
	CTrigger *_pTrigger; // items that can be taken: �Ĥ��H red, green, blue, yellow �ƦC�B�]�H���B�����ΡB����


	cocos2d::Sprite *_detect[2]; // detect syrup and herb area (��j�� node)
	cocos2d::Rect	_detectRect[2];
	cocos2d::Rect	_offRect[2]; // area to close node (��j�� node)

	cocos2d::Sprite *_trash;
	cocos2d::Rect	_trashRect; // dust bin area


	cocos2d::Sprite *_syrup[4]; // syrup in scene

	cocos2d::Sprite *_alcoholLamp[2]; //�s��O first-> off, second -> on
	cocos2d::Rect	_alcoholLampRect; //�s��O�d��

	cocos2d::Sprite *_beakerA[9]; //�q�M  0: yellow, 1:red, 2:blue, 3:green, 4:orange (1+0), 5:purple (1+2), 6:brown,  7:failed, 8 : empty
	cocos2d::Sprite *_beakerB[9]; //�q�M  0: yellow, 1:red, 2:blue, 3:green, 4:orange (1+0), 5:purple (1+2), 6:brown,  7:failed, 8 : empty
	cocos2d::Rect	*_pbeakerRect; //beaker A+B area

	cocos2d::Sprite *_beakerComplete; //�q�M  brown (completeLvl) [on flame]
	cocos2d::Sprite *_beakerFailed; //�q�M [on flame]
	cocos2d::Rect	_beakerCompleteRect;

	cocos2d::Sprite *_bowl[5];   //����(�i�n)  0:one empty, 1:yellow �icorrect�j, �iothers:failed�j -- 2:garkgreen, 3:green, 4:red
	cocos2d::Rect	*_pbowlRect;


	//mix==============================================

	CMix *_mixA;
	CMix *_mixB;
	CMix *_grind;



public:
	BMScene();
	~BMScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	void doStep(float dt);

	void SetObject(); //set objects in scene

	void PickObject(float dt); //pick and save obj

	void reset();



	//Ĳ�I
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 


	// implement the "static create()" method manually
	CREATE_FUNC(BMScene);
};

#endif // __HELLOWORLD_SCENE_H__
