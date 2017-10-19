#pragma once
#pragma once
#include "cocostudio/CocoStudio.h"

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

#include "CTrigger.h"
using namespace tinyxml2;


class  xmlTrigger {

private:
	static xmlTrigger *_pXMLTrigger;



	std::string _filePath;
	std::string _fileName;

public:
	static xmlTrigger* getInstance();
	void destroyInstance(); // 釋放取得資源

	void parseXML(const char *scene, CTrigger *ptrigger);
	void updateTriggerXML(const char *scene, CTrigger *ptrigger);
	void setTriggerStateXML(const char *scene, int code, bool state);


	xmlTrigger();
	~xmlTrigger();
};