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

	const char* getTargetSceneXML(const char *cname);
	const char* getItemNameXML(int itemNum);
	bool getStagnantXML(const char *cname);
	int getTargetNumXML(const char *cname);
	cocos2d::Rect getTargetRectXML(const char *cname, const int targetno=1);
	bool getRetakeXML(const char *cname);
	const char* getTriggerSceneXML(const char *cname);
	int getTriggerCodeXML(const char *cname);

	xmlItem();
	~xmlItem();


};