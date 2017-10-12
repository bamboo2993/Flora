#pragma once

#ifndef __KITCHEN_SCENE_H__
#define __KITCHEN_SCENE_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "common\CButton.h"
#include "common\CPlayer.h"
#include "common\CBox.h"



class kitchenScene : public cocos2d::Layer
{
private:
	cocos2d::Node *_rootNode;

	CBox *_Cabinet[6];

	//npc===================================
	cocos2d::Sprite *_npc;
	cocos2d::Rect     _npcRect;
	cocos2d::Rect     _npcTalkRect;

	//player===============================
	CPlayer *_Player;

	
public:


	~kitchenScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	void doStep(float dt);
	


	void setCabinet();
	void setNPC();
	void setPlayer();






	//Ĳ�I
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 


																	   // implement the "static create()" method manually
	CREATE_FUNC(kitchenScene);
};

#endif // __KITCHEN_SCENE_H__
