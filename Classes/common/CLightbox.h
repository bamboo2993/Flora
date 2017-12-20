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


	bool _bflip;
	int _ipageNum;
	int _icurrentPage;
public:
	static CLightbox* create();
	CLightbox();


	bool init(Node *rootNode, const std::string& item, const std::string& enlarge);
	bool init(cocos2d::Rect area, const std::string & enlarge);
	bool init();

	void setPic(const std::string& enlarge);

	void doStep(float dt);
	bool GetOpen();

	void setFlip(bool flip,int pageNum);


	//Ä²¸I
	bool TouchBegan(const cocos2d::Point pt);
	bool TouchBegan(const cocos2d::Point pt, int bagstate, const std::string& enlarge=NULL);
};


