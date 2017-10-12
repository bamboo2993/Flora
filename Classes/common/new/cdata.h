#pragma once
#include "cocostudio/CocoStudio.h"

#include "common\CBag.h"
#include "common\CTrigger.h"

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;


class  cData {

private:
	std::string _filePath;
	std::string _fileName;
	tinyxml2::XMLDocument *_pDoc;
	XMLElement *_plistElement;

	



	cocos2d::Sprite *_normal;
	cocos2d::Sprite *_touched;
	cocos2d::Sprite *_disabled;



	cocos2d::Rect    _btnRect;

	bool _bPress =false;


public:
	void addRoot(const char *root, const char *key, const char *attributeA, const char *attributeB);
	void  makeXML();
	void parseXML(cocos2d::Node *currentNode,CTrigger *ptrigger);
	void parseBagXML(CBag *bag);
	int getBagState(); // get the bag no. to put items in 
	void setBagState(int bagNum, bool state, const char* item=nullptr);


	void loadplayerInfo();
	void editItemState(const int itemNo, bool state, cocos2d::Node *currentNode);
	void updatePos(float x, float y);
	void updateScene(const char *currentScene);




	int getTriggerCode(int bagNum);

	cData(const char *fileName);
	~cData();


};