#pragma once
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

#include "CTrigger.h"


using namespace tinyxml2;


class  xmlBag {

private:
	static xmlBag *_pxmlBag;



	std::string _filePath;
	std::string _fileName;
	
	cocos2d::Rect _target[2];

	bool checkBagState(int bagNum); // used for sorting

public:
	static xmlBag* getInstance();
	void destroyInstance(); // 釋放取得資源

	void parseXML(CTrigger *ptrigger);
	int getBagState(); // get the bag no. to put items in 
	void setBagState(int bagNum, bool state, const char* item=nullptr);

	int getTriggerCode(int bagNum); // for load data. get item's trigger code from itemXML
	const char* getItemName(int bagNum);

	void setArrangementXML(const char *cname, int order);
	int getArrangement(int bagNum);

	void reset();

	xmlBag();
	~xmlBag();

	void sortItems();

};