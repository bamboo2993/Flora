#include "xmlTrigger.h"

using namespace cocos2d;
USING_NS_CC;


xmlTrigger *xmlTrigger::_pXMLTrigger = nullptr;

xmlTrigger* xmlTrigger::getInstance() {

	if (_pXMLTrigger == nullptr) _pXMLTrigger = new(std::nothrow)xmlTrigger();
	return _pXMLTrigger;

}


void xmlTrigger::destroyInstance() { CC_SAFE_DELETE(_pXMLTrigger); }



xmlTrigger::xmlTrigger() {

	_fileName = "./res/xml/xmlfile_trigger.xml";
	_filePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(_fileName, "");
	//std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(_fileName);


}

xmlTrigger::~xmlTrigger() {

}


void xmlTrigger::parseXML(const char *scene, CTrigger *ptrigger)
{

	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	//	解析xml文件 parse triggerXML====================================================
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();


	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	for (XMLElement *sceneInfo = root->FirstChildElement(); sceneInfo; sceneInfo = sceneInfo->NextSiblingElement()) {

		bool ret = (!strcmp(sceneInfo->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			for (XMLElement* trigger = sceneInfo->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {

				auto code = trigger->IntAttribute("code");

				bool ret = (!strcmp(trigger->GetText(), "false"));

				if (true == ret) {
					ptrigger[code].SetPicked(true);
					ptrigger[code].SetAddToBag(false);
				}

			}


		}

	}
	//[6] ?放?存
	delete pDoc;
}


// used when switching scene
void xmlTrigger::updateTriggerXML(const char *scene, CTrigger *ptrigger) {

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

	//?取子元素信息

	for (XMLElement* s = root->FirstChildElement(); s; s = s->NextSiblingElement()) {

		bool ret = (!strcmp(s->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			for (XMLElement* trigger = s->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {
				int icode = trigger->IntAttribute("code");

				if (ptrigger[icode].GetPicked() == true) {
					trigger->FirstChild()->SetValue("false");
				}
				else trigger->FirstChild()->SetValue("true");



			}

		}
	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}

void xmlTrigger::setTriggerStateXML(const char *scene, int code, bool state) {

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

	//?取子元素信息

	for (XMLElement* e = root->FirstChildElement(); e; e = e->NextSiblingElement()) {

		bool ret = (!strcmp(e->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			for (XMLElement* trigger = e->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {

				if (code == trigger->IntAttribute("code")) {
					if (state) trigger->FirstChild()->SetValue("true");
					else trigger->FirstChild()->SetValue("false");
				}

			}

		}

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}
