#ifndef __C2_SCENE_01_SCENE_H__
#define __C2_SCENE_01_SCENE_H__

#include "cocos2d.h"
#include "common\CPlayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class C2_Scene_01 : public cocos2d::Layer
{
private:
	
	Sprite *_bg;

	//boy
	CPlayer *_boy;

	//spots
	Sprite *_spot[2];
	Rect _spotRect[2];

	//map
	bool _toSpot[2];
	bool _isWalking;

	//Area
	Sprite *_doorArea;
	Rect _doorRect;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	static cocos2d::Scene* createScene(int n);
	static int _from;

	C2_Scene_01();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);

	//map
	bool ToSpot0(float dt);
	bool ToSpot1(float dt);

	//touch
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 

    // implement the "static create()" method manually
    CREATE_FUNC(C2_Scene_01);
	
};

#endif // __HELLOWORLD_SCENE_H__
