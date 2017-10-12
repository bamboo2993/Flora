#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"


class CLightbox :public cocos2d::Node
{
private:

	cocos2d::Sprite *_item;
	cocos2d::Rect    _itemRect;
	

	cocos2d::Sprite *_enlarge;
	cocos2d::Rect    _enlargeRect;

	cocos2d::Point _touchpt;
	bool _isOpen;
	bool _btouch;

public:
	static CLightbox* create();
	CLightbox();


	virtual bool init(Node *rootNode, const std::string& item, const std::string& enlarge);
	void doStep(float dt);
	bool GetOpen();

	//Ä²¸I
	bool TouchBegan(const cocos2d::Point pt);

};


