#ifndef __BR_SCENE_H__
#define __BR_SCENE_H__

#include "cocos2d.h"
#include "common\CPlayer.h"
#include "common\CLightbox.h"
#include "SimpleAudioEngine.h"

#include "common\CTrigger.h"
#include "common\CBag.h"

using namespace cocos2d;

class BR_Scene : public cocos2d::Layer
{
private:
	Sprite *_bg_front, *_bg_back;

	//boy
	CPlayer *_boy;

	//spots
	Sprite *_spot[6];
	Rect _spotRect[6];

	//map
	bool _toSpot[6];
	bool _isWalking;

	//talk
	Sprite *_talkArea[5];
	Rect _talkAreaRect[5];
	Sprite *_dialog[50];


	CTrigger *_pTrigger; // items that can be taken: ÃÄ¤ô¥H red, green, blue, yellow ±Æ¦C¡B»]ÃH¤ô¡B¬Á¼þ´Î¡B¯óÃÄ
	//bag=============================================
	int _ibagState;

	//Ä²±±-----------
	cocos2d::Point _touchLoc;
	cocos2d::Point _itempos;
	float _startX, _startY;

	//bag
	CLightbox *_newspaper;

	cocos2d::Node *_zNode; // zoomin node
	bool _btouchNode;//for z_node
	bool _bopenNode;//for z_node
	cocos2d::Rect	_detectRect;


	cocos2d::Rect	_resetRect; // reset button
	cocos2d::Sprite *_close;
	cocos2d::Rect	_closeRect; // area to close node (znode)

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	BR_Scene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);


	//map
	bool ToMainPath(float dt);
	bool ToSpot0(float dt);
	bool ToSpot1(float dt);
	bool ToSpot2(float dt);
	bool ToSpot3(float dt); 
	bool ToSpot4(float dt);
	bool ToSpot5(float dt);

	bool GoSpot3(float dt, int n); // for passing by
	bool GoSpot1(float dt, int n);
	
	void ClearToSpot();

	//touch
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); 

    // implement the "static create()" method manually
    CREATE_FUNC(BR_Scene);
	
};

#endif // __HELLOWORLD_SCENE_H__
