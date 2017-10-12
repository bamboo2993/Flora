#pragma once
#include "cocostudio/CocoStudio.h"

#include "common\CTrigger.h"

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
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

	void setTriggerStateXML(const char *scene, const int code, bool pick, bool addtobag);
	bool getChangeState(const char *scene);
	void updateTrigger(const char *scene, CTrigger *ptrigger);


	void resetXML(const char *scene);

	xmlTrigger(const char *fileName);
	~xmlTrigger();


};