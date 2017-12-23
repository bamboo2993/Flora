#ifndef __C2_Scene_02_SCENE_H__
#define __C2_Scene_02_SCENE_H__

#include "cocos2d.h"
#include "common\CPlayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class C2_Scene_02 : public cocos2d::Layer
{
private:
	Sprite *_bg;

	//boy
	CPlayer *_boy;

	//spots
	Sprite *_spot[3];
	Rect _spotRect[3];

	//map
	bool _toSpot[3];
	bool _isWalking;

	//Area
	Sprite *_doorArea, *_paintArea;
	Rect _doorRect, _paintRect;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	C2_Scene_02();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);

	//map
	bool ToSpot0(float dt);
	bool ToSpot2(float dt);

	//touch
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 

    // implement the "static create()" method manually
    CREATE_FUNC(C2_Scene_02);
	
};

#endif // __HELLOWORLD_SCENE_H__
