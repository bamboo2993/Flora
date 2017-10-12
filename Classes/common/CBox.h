#pragma once


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"


class CBox : public cocos2d::Sprite
{
private:


	bool state = false;

	//item
	cocos2d::Point _itemLoc;
	cocos2d::Rect    _itemRect;

	//set region
	struct reg {
		float fm; //gradient
		bool bgradient;
		int iparts =0;

		cocos2d::Rect    ARect;
		cocos2d::Rect    BRect;
		cocos2d::Rect    CRect;
		cocos2d::Rect    DRect;
	};

	reg _close;


	cocos2d::Point _openLoc;
	cocos2d::Point _openOrigin;

	//card
	cocos2d::Sprite *_card;

	cocos2d::Spawn *_In;
	cocos2d::Spawn *_Out;
	cocos2d::DelayTime *_delay;

	cocos2d::CallFunc *_callback;
	bool _bpress = false;

	cocos2d::LayerColor *_bgLayer;
	cocos2d::LayerColor *_contentLayer;

public:
	cocos2d::Sprite *_open;
	cocos2d::Sprite *_item;

	CBox();
	~CBox();

	virtual bool init();
	void doStep(float dt);




	void SetInfo(const cocos2d::Point a, const cocos2d::Point b, const cocos2d::Point c, const cocos2d::Point d, const cocos2d::Point openLoc, const std::string open, cocos2d::Layer &parent);
	
	void SetCloseReg(const cocos2d::Point a, const cocos2d::Point b, const cocos2d::Point c, const cocos2d::Point d);
	void setItem(const cocos2d::Point itemLoc, const std::string item);
	void setItemCard(const std::string card, cocos2d::Layer &parent);

	void itemPressed();
	void Status(bool state);

	bool TouchBegan(const cocos2d::Point pt);
	bool TouchMoved(const cocos2d::Point pt);
	bool TouchEnded(const cocos2d::Point pt);


	// implement the "static create()" method manually
	CREATE_FUNC(CBox);
};


