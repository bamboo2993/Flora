#include "xmlTrigger.h"

using namespace cocos2d;


xmlTrigger *xmlTrigger::_pXMLTrigger = nullptr;


xmlTrigger* xmlTrigger::getInstance() {

	if (_pXMLTrigger == nullptr) _pXMLTrigger = new(std::nothrow)xmlTrigger("./res/xml/xmlfile_trigger.xml");
	return _pXMLTrigger;

}


void xmlTrigger::destroyInstance() { CC_SAFE_DELETE(_pXMLTrigger); }




xmlTrigger::xmlTrigger(const char *fileName){

	_fileName = fileName;
	_filePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(_fileName, "");
	//std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(_fileName);


}

xmlTrigger::~xmlTrigger(){


}



void xmlTrigger::parseXML(const char *scene, CTrigger *ptrigger)
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
	XMLElement *TriggerList = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* e = TriggerList->FirstChildElement(); e; e = e->NextSiblingElement()) {

		bool ret = (!strcmp(e->Attribute("scene"), scene)); // select the correct scene

		if (true == ret){ 
			for (XMLElement* trigger = e->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {

				auto code = trigger->IntAttribute("code");
				auto pick = trigger->BoolAttribute("bPicked");
				auto addtobag = trigger->BoolAttribute("bAddToBag");


				ptrigger[code].SetPicked(pick);
				ptrigger[code].SetAddToBag(addtobag);

			}


		}

	} 
	//[6] ?放?存
	delete pDoc;
}


void xmlTrigger::setTriggerStateXML(const char *scene, const int code, bool pick, bool addtobag){

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
	XMLElement *TriggerList = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* e = TriggerList->FirstChildElement(); e; e = e->NextSiblingElement()) {

		bool ret = (!strcmp(e->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			for (XMLElement* trigger = e->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {
				if (!strcmp(trigger->Name(), "trigger")) {
					if (code == trigger->IntAttribute("code")) {
						if (pick) trigger->SetAttribute("bPicked", "true");
						else trigger->SetAttribute("bPicked", "false");

						if (addtobag) trigger->SetAttribute("bAddToBag", "true");
						else trigger->SetAttribute("bAddToBag", "false");

					}
				}

			}
			XMLElement* triggerChange = e->LastChildElement();
			triggerChange->FirstChild()->SetValue("true");
		}

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}



bool xmlTrigger::getChangeState(const char *scene) {

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
	XMLElement *TriggerList = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* e = TriggerList->FirstChildElement(); e; e = e->NextSiblingElement()) {

		bool ret = (!strcmp(e->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			XMLElement* triggerChange = e->LastChildElement();

			bool ret = (!strcmp(triggerChange->GetText(), "true"));

			if (true == ret)  return true;
			else return false;
		}

	}
	//[6] ?放?存
	delete pDoc;
}

void xmlTrigger::updateTrigger(const char *scene, CTrigger *ptrigger) {

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
	XMLElement *TriggerList = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* e = TriggerList->FirstChildElement(); e; e = e->NextSiblingElement()) {

		bool ret = (!strcmp(e->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			for (XMLElement* trigger = e->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {
				int icode = trigger->IntAttribute("code");
				auto pick = trigger->BoolAttribute("bPicked");
				auto addtobag = trigger->BoolAttribute("bAddToBag");

				ptrigger[icode].SetPicked(pick);
				ptrigger[icode].SetAddToBag(addtobag);

			}
			XMLElement* triggerChange = e->LastChildElement();
			triggerChange->FirstChild()->SetValue("false");
		}

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}



void xmlTrigger::resetXML(const char *scene) {

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
	XMLElement *TriggerList = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* e = TriggerList->FirstChildElement(); e; e = e->NextSiblingElement()) {

		bool ret = (!strcmp(e->Attribute("scene"), scene)); // select the correct scene

		if (true == ret) {
			for (XMLElement* trigger = e->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {
				if (!strcmp(trigger->Name(), "trigger")) {

					trigger->SetAttribute("bPicked", "false");

					trigger->SetAttribute("bAddToBag", "false");

				}

			}
			XMLElement* triggerChange = e->LastChildElement();
			triggerChange->FirstChild()->SetValue("false");
		}

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}
