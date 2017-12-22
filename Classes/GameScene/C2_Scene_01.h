#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class BR_Scene : public cocos2d::Layer
{
private:
	Sprite *_bg_front, *_bg_back;

	//boy
	Node *_boyRoot;
	Sprite *_boyBody;
	Sprite *_walkAni[4];

	//spots
	Sprite *_spot[5];
	Rect _spotRect[5];

	//map
	bool _toSpot[5];
	bool _reachSpot[5];
	bool _reachMainpath;
	bool _isWalking;

	//talk
	Sprite *_talkArea[6];
	Rect _talkAreaRect[6];
	Sprite *_dialog[50];

	//bag
	Sprite *_bagArea;
	Rect _bagRect;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	BR_Scene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);
	void ClearReachSpot();


	//map
	bool ToMainPath(float dt);
	bool ToSpot0(float dt);
	bool ToSpot1(float dt);
	bool ToSpot2(float dt);
	bool ToSpot3(float dt); 
	bool ToSpot4(float dt);

	//touch
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 

    // implement the "static create()" method manually
    CREATE_FUNC(BR_Scene);
	
};

#endif // __HELLOWORLD_SCENE_H__
