#pragma once
#include "cocostudio/CocoStudio.h"

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;


class  xmlItem {

private:
	static xmlItem *_pXMLItem;



	std::string _filePath;
	std::string _fileName;

public:
	static xmlItem* getInstance();
	void destroyInstance(); // 釋放取得資源

	const char* getItemNameXML(int itemNum);
	bool getItemStateXML(const char *cname);//if state == 1 : item used in scene,   if state == 0 : item used in bag

	bool getRetakeXML(const char *cname);
	bool getStagnantXML(const char *cname);
	int getTargetNumXML(const char *cname);
	cocos2d::Rect getTargetRectXML(const char *cname, const int targetno = 1);


	const char* getTargetSceneXML(const char *cname, bool num=0);
	const char* getTriggerSceneXML(const char *cname);

	int getTriggerCodeXML(const char *cname);

	const char * getItemDescriptionXML(const char * cname);


	int checkMixingXML(const char *cname, const char *mixname);


	xmlItem();
	~xmlItem();


	void setTargetRectXML(int itemNum, cocos2d::Rect area); //for setting target rect in xml


};