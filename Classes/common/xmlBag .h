#pragma once
#include "cocostudio/CocoStudio.h"


#include "common\CTrigger.h"



#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;


class  xmlBag {

private:
	static xmlBag *_pxmlBag;



	std::string _filePath;
	std::string _fileName;
	
	cocos2d::Rect _target[2];

public:
	static xmlBag* getInstance();
	void destroyInstance(); // ������o�귽

	void parseXML(CTrigger *ptrigger);
	int getBagState(); // get the bag no. to put items in 
	void setBagState(int bagNum, bool state, const char* item=nullptr);

	int getTriggerCode(int bagNum); // for load data. get item's trigger code from itemXML
	const char* getItemName(int bagNum);
//	void setTriggerChange(const char *scene, int code, bool bPick, bool bAddtobag);

	void reset();

	xmlBag();
	~xmlBag();


};