#ifndef __C2_Scene_02_SCENE_H__
#define __C2_Scene_02_SCENE_H__

#include "cocos2d.h"
#include "common\CPlayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"


#include "common\CeLock .h"

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

	//lock
	CeLock *_elock;


	//bag=============================================
	int _ibagState;

	//觸控-----------
	cocos2d::Point _touchLoc;
	cocos2d::Point _itempos;
	float _startX, _startY;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	static int _from;

	C2_Scene_02();
	~C2_Scene_02();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);

	//map
	bool ToSpot0(float dt);
	bool ToSpot2(float dt);

	//觸碰
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 

    // implement the "static create()" method manually
    CREATE_FUNC(C2_Scene_02);
	
};

#endif // __HELLOWORLD_SCENE_H__
