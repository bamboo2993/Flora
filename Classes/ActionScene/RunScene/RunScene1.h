#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ActionScene\RunScene\CObject.h"
#include "ActionScene\RunScene\CRunner.h"
#include "ActionScene\RunScene\CObstacle.h"
#include "ActionScene\RunScene\CFall.h"
#include "Classes\common\CButton.h"

#include "math.h"//用到??值

class RunScene1 : public cocos2d::Layer
{
private:

	cocos2d::Sprite *_reset;
	cocos2d::Rect   _resetRect[2];


	cocos2d::Sprite *_blur[3];
	cocos2d::Sprite *_win;


	cocos2d::Point _fg1Loc[2];	// _fgnode1 的前景物件產生的位置
	cocos2d::Point _fg2Loc[2];	// _fgnode2 的前景物件產生的位置
	cocos2d::Node *_fgnode1, *_fgnode2;	// 前景的兩段道路

							
	CObject *_fg1obj[2], *_fg2obj[2];      // 前景動態生成的物件 (house)
	CObstacle *_obstacle1[5];    // 前景動態生成的物件 (obstacle)
	CObstacle *_obstacle2[3];    // 前景動態生成的物件 (obstacle)
	
	
	CFall *_trash[10]; //紙球障礙物


	cocos2d::Sprite *_bike; //腳踏車障礙物
	cocos2d::Rect   _bikeRect;
	cocos2d::Point  _bikePt;
	bool _bcolBike; //腳踏車碰撞
	float _t;
	bool _bwin;
	bool _brestart;
	bool _bStart;
	bool _bpause;
	// 判斷滑動========================================================
	float startX, startY, endX, endY;//??触摸?始坐?与?束坐?
	cocos2d::LabelTTF *label;//文字 
	cocos2d::LabelTTF *label000;//文字 
	cocos2d::LabelTTF *label5;//文字 

	//runner===========================================
	CRunner *_runner;

	// button===============================
	CButton *_pause;


public:
	~RunScene1();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	
	
	
	
	void doStep(float dt);

	
	void Setbg(); //設定背景 + 障礙物
	void restart();// 重新產生前景1 中物件的位置與狀態


	//觸碰
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 

																	   // implement the "static create()" method manually
	CREATE_FUNC(RunScene1);
};

#endif // __HELLOWORLD_SCENE_H__
