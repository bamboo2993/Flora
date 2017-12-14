#pragma once
#include "cocostudio/CocoStudio.h"

#include "CBag.h"
#include "xmlTrigger.h"

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;
using namespace cocos2d;


class  xmlScene {

private:
	std::string _filePath;
	std::string _fileName;

	std::string _triggerfilePath;


public:
	void parseXML(cocos2d::Node *currentNode, const char *scene, CTrigger *ptrigger); // load every time switching into  new scene
	void parseNodeXML(cocos2d::Node *currentNode, const char *node); // load every time switching into  new scene


	void editItemState(const int itemNo, bool state, cocos2d::Node *currentNode); // save state of obj when state has changed
	void editItemState(const char *name, bool state, cocos2d::Node *currentNode, int start, int end); // save state of obj when state has changed


	void editRangeState(const int itemNo, bool state, cocos2d::Node *currentNode, int start, int end); // togger state



	const char* getItemNameXML(int itemNum);
	const char* getBItemNameXML(int itemNum);

	int getItemNumXML(const char* itemName, bool type=0); // type 0-> find name; type 1-> find B_name

	void reset();

	xmlScene(const char *fileName);
	~xmlScene();

};