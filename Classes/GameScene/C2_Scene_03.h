#ifndef __C2_Scene_03_SCENE_H__
#define __C2_Scene_03_SCENE_H__

#include "cocos2d.h"
#include "common\CPlayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class C2_Scene_03 : public cocos2d::Layer
{
private:
	Sprite *_bg, *_bgfront;

	//boy
	CPlayer *_boy;

	//spots
	Sprite *_spot[6];
	Rect _spotRect[6];

	//map
	bool _toSpot[6];
	bool _isWalking;

	//Area
	Sprite *_doorArea[2];
	Rect _doorRect[2];

	//bag=============================================
	int _ibagState;

	//Ä²±±-----------
	cocos2d::Point _touchLoc;
	cocos2d::Point _itempos;
	float _startX, _startY;

	int _iObjUsed;


public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	static int _from;

	C2_Scene_03();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);

	//map
	bool ToSpot0(float dt);
	bool ToSpot2(float dt);
	bool ToSpot3(float dt);
	bool ToSpot4(float dt);
	bool ToSpot5(float dt);
	bool ToSpot1_45(float dt);
	bool ToSpot1_023(float dt);
	void ClearToSpot();

	//touch
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 

    // implement the "static create()" method manually
    CREATE_FUNC(C2_Scene_03);
	
};

#endif // __HELLOWORLD_SCENE_H__
