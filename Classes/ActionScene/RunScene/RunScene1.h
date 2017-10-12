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

#include "math.h"//�Ψ�??��

class RunScene1 : public cocos2d::Layer
{
private:

	cocos2d::Sprite *_reset;
	cocos2d::Rect   _resetRect[2];


	cocos2d::Sprite *_blur[3];
	cocos2d::Sprite *_win;


	cocos2d::Point _fg1Loc[2];	// _fgnode1 ���e�����󲣥ͪ���m
	cocos2d::Point _fg2Loc[2];	// _fgnode2 ���e�����󲣥ͪ���m
	cocos2d::Node *_fgnode1, *_fgnode2;	// �e������q�D��

							
	CObject *_fg1obj[2], *_fg2obj[2];      // �e���ʺA�ͦ������� (house)
	CObstacle *_obstacle1[5];    // �e���ʺA�ͦ������� (obstacle)
	CObstacle *_obstacle2[3];    // �e���ʺA�ͦ������� (obstacle)
	
	
	CFall *_trash[10]; //�Ȳy��ê��


	cocos2d::Sprite *_bike; //�}�񨮻�ê��
	cocos2d::Rect   _bikeRect;
	cocos2d::Point  _bikePt;
	bool _bcolBike; //�}�񨮸I��
	float _t;
	bool _bwin;
	bool _brestart;
	bool _bStart;
	bool _bpause;
	// �P�_�ư�========================================================
	float startX, startY, endX, endY;//??�D�N?�l��?�O?����?
	cocos2d::LabelTTF *label;//��r 
	cocos2d::LabelTTF *label000;//��r 
	cocos2d::LabelTTF *label5;//��r 

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

	
	void Setbg(); //�]�w�I�� + ��ê��
	void restart();// ���s���ͫe��1 �����󪺦�m�P���A


	//Ĳ�I
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 

																	   // implement the "static create()" method manually
	CREATE_FUNC(RunScene1);
};

#endif // __HELLOWORLD_SCENE_H__
