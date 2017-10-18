#pragma once
#pragma once
#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "common\CPlayer.h"

//#include "common\CBag.h"
#include "common\CTrigger.h"
#include "common\CItem.h"
#include "common\CLightbox.h"

//#include "common\xmlTrigger.h"
#include "common\xmlScene.h"

#include "common\CMix.h"

class labScene : public cocos2d::Layer
{
private:

	bool _clear;
	// sound effect
	cocostudio::ComAudio *_pour;
	cocostudio::ComAudio *_powder;
	cocostudio::ComAudio *_mixing;
	cocostudio::ComAudio *_grinding;
	cocostudio::ComAudio *_debranch;

	cocos2d::Sprite *_win;
	cocos2d::Sprite *_bkBlur;
	cocos2d::Node *_eNode[2];

	cocos2d::Sprite *_detect[2];
	cocos2d::Rect	_detectRect[2];
	cocos2d::Rect	_offRect[2];
	bool _btouchNode[2] = { false };//for E_node
	bool _bopenNode[2] = { false };//for E_node


	cocos2d::Sprite *_syrup[4];


	cocos2d::Sprite *_trash;
	cocos2d::Rect	_trashRect;

	cocos2d::Sprite *_reset;
	cocos2d::Rect	_resetRect;


	bool _bbOnFire[2];
	
	//mix==========================

	CMix *_mixA;
	CMix *_mixB;
	bool bb[2] = { false };


	CMix *_grind;
	//===============================

	cocos2d::Node *_rootNode;
	cocos2d::Point _touchLoc;

	xmlScene *_xmlscene;

	//set object==========================================================
	//CTrigger *_trigger[10]; // set trigger
	CTrigger *_pTrigger;
	cocos2d::Sprite *_alcoholLamp[2]; //�s��O first-> off, second -> on
	cocos2d::Sprite *_bowl[5];   //����(�i�n)  0:one empty, 1:yellow �icorrect�j, �iothers:failed�j -- 2:garkgreen, 3:green, 4:red


	cocos2d::Rect	_alcoholLampRect;
	cocos2d::Rect	_beakerARect;
	cocos2d::Rect	_beakerBRect;
	cocos2d::Rect	_beakerCompleteRect;
	cocos2d::Rect	_bowlRect;


	cocos2d::Rect	*_pbeakerRect;
	cocos2d::Rect	*_pbowlRect;
	//cocos2d::Rect	_beakerRect[2];






	//CTrigger *_syrup[4]; //�Ĥ� �H yellow, red, blue, green  �ƦC
	//CTrigger *_glassrod; //������
	//CTrigger *_water; //�]�H��
	cocos2d::Sprite *_beakerA[9]; //�q�M  0: yellow, 1:red, 2:blue, 3:green, 4:orange (1+0), 5:purple (1+2), 6:brown,  7:failed, 8 : empty
	cocos2d::Sprite *_beakerB[9]; //�q�M  0: yellow, 1:red, 2:blue, 3:green, 4:orange (1+0), 5:purple (1+2), 6:brown,  7:failed, 8 : empty
	cocos2d::Sprite *_beakerComplete; //�q�M  brown (completeLvl)
	cocos2d::Sprite *_beakerFailed; //�q�M  



	cocos2d::Rect	_syrupRect[4];
	cocos2d::Rect	_glassrodRect;
	cocos2d::Rect	_waterRect;

	CTrigger *_herbRect[4];  //���� 0: ligth green, 1: red, 2: dark green, 3: yellow




	cocos2d::Point _prePos;
	bool _btouch[5];
	cocos2d::Point _itempos;

	//player===================================
	CPlayer *_player;
	bool _bWalk = false; //detect if player is walking
	bool _bWalkpos;
	cocos2d::Point _TargetLoc;

	//�P�_ walk

	bool _bwithinArea; // detect if touch_pts are in walkable area
	//bool _bonObj; //detect if obj is touched up

	bool _bpickObj; //detect if player arrive near object being picked
	bool _bpickSyrup[4]; //pick syrup
	bool _bpickWater; //pick water
	bool _bpickRod; //pick glassrod


	//�P�_ detect what is add into beakerA
	bool _buseSyrupA[4]; // add syrup into beaker �H yellow, red, blue, green  �ƦC

	//�P�_ detect what is add into beakerB
	bool _buseSyrupB[4]; // add syrup into beaker �H yellow, red, blue, green  �ƦC

	bool _bgetOSyrup; //get orange syrup
	bool _bgetPSyrup; // get purple syrup
	bool _bgetBSyrup[2]; // get brown syrup -- 0:beaker�iA�j , 1:beaker�iB�j
	bool _bpourIntoA; //pour �ibeaker B�j into �ibeaker A�j
	bool _bpourIntoB; //pour �ibeaker A�j into �ibeaker B�j

	//�P�_ detect what herb is being grind in bowl
	bool _bgrindG;  //grind �igreen herb�j
	bool _bgrindDG;  //grind �idark green herb�j
	bool _bgrindR;  //grind �ired herb�j
	bool _bgrindY;  //grind �iyellow herb�j


	bool _buseHerb; // add grind �iherb�j into �ibeaker�j

	bool _buseWater;  
	bool _buseGlassrod; 


	//bool _bcompleteSyrup[2]; // complete syrup -- 0:in beakerA, 1:in beakerB





	bool _bbagOn;


	//bag==========================
	//CBag *_bag;
	float _startX, _startY, _endX, _endY;
	bool  _isLeft, _isRight;

	int _iObjUsed; 

	// set lightbox
	CLightbox *_procedure;










public:
	labScene();
	~labScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	void doStep(float dt);

	void SetObject(); //set objects in scene
	void SetItem(); // set items in bag

	void PickObject(float dt); //pick and save obj
	void MixSyrup(); //mix syrup
	void GrindHerb();
	void UseHerb();

	void reset();



	//Ĳ�I
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 


																	   // implement the "static create()" method manually
	CREATE_FUNC(labScene);
};

#endif // __HELLOWORLD_SCENE_H__
