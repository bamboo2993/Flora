#include "xmlBag .h"
#include "common\CBag.h"
#include "common\xmlItems.h"
#include "common\xmlTrigger.h"

using namespace cocos2d;

#define DATA_ROOT_NAME    "DataRoot"
#define XML_FILE_NAME "Data.xml"


xmlBag *xmlBag::_pxmlBag = nullptr;

xmlBag* xmlBag::getInstance() {

	if (_pxmlBag == nullptr) _pxmlBag = new(std::nothrow)xmlBag();
	return _pxmlBag;

}


void xmlBag::destroyInstance() { CC_SAFE_DELETE(_pxmlBag); }



xmlBag::xmlBag(){

	_fileName = "./res/xml/xmlfile_bag.xml";
	_filePath = FileUtils::getInstance()->fullPathFromRelativeFile(_fileName, "");
	//std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(_fileName);

}

xmlBag::~xmlBag(){

}

void xmlBag::parseXML()
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *sceneInfo = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {

		bool ret = (!strcmp(item->GetText(), "true"));  // when it is being occupied

		if (true == ret) {
			const char *name = item->Attribute("name");
			int inum = item->IntAttribute("no.");
			log("name of item in bag = %s", name);

			auto targetNum = xmlItem::getInstance()->getTargetNumXML(name);

			log("targetNum = %d", targetNum);
			
			_target[0] = xmlItem::getInstance()->getTargetRectXML(name);
			if(targetNum>1) _target[1] = xmlItem::getInstance()->getTargetRectXML(name,2);
			bool isStagnant = xmlItem::getInstance()->getStagnantXML(name);
			auto canRetake = xmlItem::getInstance()->getRetakeXML(name);
			

			CBag::getInstance()->AddObjXML(inum, name, targetNum, _target, isStagnant, canRetake);
		}

	}

	//[6] ?放?存
	delete pDoc;
}


int xmlBag::getBagState() {
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return false;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *sceneInfo = root->FirstChildElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {

		bool ret = (!strcmp(item->GetText(), "false"));

		if (true == ret) {
			/*const char *cnumber = item->Attribute("no.");
			int inum = Value(cnumber).asInt();
			return inum; */


			log("no = %d", item->IntAttribute("no."));

			int inum = item->IntAttribute("no.");
			return inum;

		}
	}

	return -1;
	//[6] ?放?存
	delete pDoc;
}


int xmlBag::getTriggerCode(int bagNum) {
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return false;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *sceneInfo = root->FirstChildElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {

		if (item->IntAttribute("no.") == bagNum) {
			const char *name = item->Attribute("name");
			auto e = xmlItem::getInstance()->getTriggerCodeXML(name);
			return e;
		}
	}

	return false;
	//[6] ?放?存
	delete pDoc;
}

const char* xmlBag::getItemName(int bagNum) {
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return false;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *sceneInfo = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {

		if (item->IntAttribute("no.") == bagNum) {
			const char *name = item->Attribute("name");
			return name;
		}
	}

	return false;
	//[6] ?放?存
	delete pDoc;
}






void xmlBag::setTriggerChange(const char *scene, int code, bool bPick, bool bAddtobag){
	xmlTrigger::getInstance()->setTriggerStateXML(scene, code, bPick, bAddtobag);
}



void xmlBag::setBagState(int bagNum, bool state, const char* obj) {
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *sceneInfo = root->FirstChildElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息
	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if (item->IntAttribute("no.") == bagNum) {
			if (state == true) {
				log("noSave = %d", item->IntAttribute("no."));
				item->FirstChild()->SetValue("true");
				item->SetAttribute("name", obj);
			}
			else item->FirstChild()->SetValue("false");

		}
	}
	pDoc->SaveFile(_filePath.c_str());

	//[6] ?放?存
	delete pDoc;

}



void xmlBag::reset() {
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *sceneInfo = root->FirstChildElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息
	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if (!strcmp(item->GetText(), "true")) {
			item->FirstChild()->SetValue("false");
		}
	}
	pDoc->SaveFile(_filePath.c_str());

	//[6] ?放?存
	delete pDoc;
}