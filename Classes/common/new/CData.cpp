#include "cData.h"

using namespace cocos2d;



#define DATA_ROOT_NAME    "DataRoot"

#define XML_FILE_NAME "Data.xml"


cData::cData(const char *fileName){

	_fileName = fileName;
	_filePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(_fileName, "");
	//std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(_fileName);






}

cData::~cData(){


}



void cData::parseXML(cocos2d::Node *currentNode, CTrigger *ptrigger)
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
	XMLElement *ItemList = sceneInfo->FirstChildElement();
	XMLElement *TriggerList = ItemList->NextSiblingElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	for (XMLElement* item = ItemList->FirstChildElement(); item; item = item->NextSiblingElement()) {

		bool ret = (!strcmp(item->GetText(), "false"));

		if (true == ret){
			const char *name = item->Attribute("name");

			auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
			obj->setVisible(false);
		}

	} 

	for (XMLElement* trigger = TriggerList->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {
		auto icode = trigger->IntAttribute("code");
		auto bpicked = trigger->BoolAttribute("bPicked");
		auto baddtobag = trigger->BoolAttribute("bAddToBag");

		ptrigger[icode].SetPicked(bpicked);
		ptrigger[icode].SetAddToBag(baddtobag);
	}
	//[6] ?放?存
	delete pDoc;
}



void cData::editItemState(const int itemNo, bool state, cocos2d::Node *currentNode) {

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
	XMLElement *ItemList = sceneInfo->FirstChildElement();
	XMLElement *TriggerList = ItemList->NextSiblingElement();

	//?取子元素信息
	int count = 0;
	for (XMLElement* item = ItemList->FirstChildElement(); item; item = item->NextSiblingElement()) {
		count++;
		if (count == itemNo) {
			if (state) {
				item->FirstChild()->SetValue("true");
				const char *name = item->Attribute("name");

				auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
				obj->setVisible(true);

			}
			else {
				item->FirstChild()->SetValue("false");

				const char *name = item->Attribute("name");

				auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
				obj->setVisible(false);
			}



		}

		log("state = %d", state);
		log("text = %s", item->GetText());

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;

}






void cData::parseBagXML(CBag *bag)
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

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {

		bool ret = (!strcmp(item->GetText(), "true"));

		if (true == ret) {
			const char *cname = item->Attribute("name");
			int inum = item->IntAttribute("no.");
			int iarrangement = item->IntAttribute("arrangement");

			auto bstagnant = item->BoolAttribute("stagnant");

			auto bretake = item->BoolAttribute("retake");
			auto itrigger = item->IntAttribute("trigger");

			auto itargetNum = item->IntAttribute("targetnum");
			Rect *ptargetRect;
			ptargetRect[0]= Rect(item->BoolAttribute("target1_X") - item->BoolAttribute("target1_w") / 2, item->BoolAttribute("target1_Y") - item->BoolAttribute("target1_h") / 2, item->BoolAttribute("target1_w"), item->BoolAttribute("target1_h"));

			if(itargetNum==2) ptargetRect[1] = Rect(item->BoolAttribute("target2_X") - item->BoolAttribute("target2_w") / 2, item->BoolAttribute("target2_Y") - item->BoolAttribute("target2_h") / 2, item->BoolAttribute("target2_w"), item->BoolAttribute("target2_h"));
			
			bag->AddObj(cname, itargetNum, ptargetRect, bstagnant, bretake);
		}

	}
	//[6] ?放?存
	delete pDoc;
}





void cData::parseTriggerXML(CTrigger *ptrigger)
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
	XMLElement *TriggerList = sceneInfo->FirstChildElement()->NextSiblingElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	for (XMLElement* trigger = TriggerList->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {
		auto icode = trigger->IntAttribute("code");
		auto bpicked = trigger->BoolAttribute("bPicked");
		auto baddtobag = trigger->BoolAttribute("bAddToBag");

		ptrigger[icode].SetPicked(bpicked);
		ptrigger[icode].SetAddToBag(baddtobag);
	}
	//[6] ?放?存
	delete pDoc;
}





int cData::getTriggerCode(int bagNum)
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
	XMLElement *TriggerList = sceneInfo->FirstChildElement()->NextSiblingElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	int count = 0;
	for (XMLElement* trigger = TriggerList->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {

		if (count == bagNum) {
			auto icode = trigger->IntAttribute("trigger");
			return icode;
		}


		count++;
	}
	//[6] ?放?存
	delete pDoc;
}





void cData::setTriggerChange(int triggerCode)
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
	XMLElement *TriggerList = sceneInfo->FirstChildElement()->NextSiblingElement();
	XMLElement *TriggerChange = TriggerList->NextSiblingElement();

	//XMLElement *roomInfo = sceneInfo->FirstChildElement();
	//?取子元素信息

	int count = 0;
	for (XMLElement* trigger = TriggerList->FirstChildElement(); trigger; trigger = trigger->NextSiblingElement()) {

		if (count == triggerCode) {

			auto icode = trigger->IntAttribute("trigger");


			if (item != nullptr) trigger->SetAttribute("name", item);
		}


		count++;
	}
	//[6] ?放?存
	delete pDoc;
}


int cData::getBagState() {
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

		bool ret = (!strcmp(item->GetText(), "false"));

		if (true == ret) {
			/*const char *cnumber = item->Attribute("no.");
			int inum = Value(cnumber).asInt();
			return inum; */

			int inum = item->IntAttribute("no.");
			return inum;
		}
	}

	return false;
	//[6] ?放?存
	delete pDoc;
}



void cData::setBagState(int bagNum, bool state, const char* item) {
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
	int count = 0;
	for (XMLElement* item = sceneInfo->FirstChildElement(); item; item = item->NextSiblingElement()) {

		if (count == bagNum) {
			if (state == true) item->FirstChild()->SetValue("true");
			else item->FirstChild()->SetValue("false");

			if (item != nullptr) item->SetAttribute("name", item);

		}

		count++;

	}


	//[6] ?放?存
	delete pDoc;

}








//儲存 current scene
void cData::updateScene(const char *currentScene) {
	std::string filePath = FileUtils::getInstance()->getWritablePath() + _fileName;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *playerInfo = root->FirstChildElement()->NextSiblingElement();
	XMLElement *Info = playerInfo->FirstChildElement();


	//?取子元素信息
	for (XMLElement* ch = Info->FirstChildElement(); ch; ch = ch->NextSiblingElement()) {

		// 可以通??性名?，??取?性值

		log("pos = %s", ch->Name());
		if ((!strcmp(ch->Name(), "currentMap"))) {
			ch->FirstChild()->SetValue(currentScene);
			
			log("scene = %s", ch->GetText());
		}
	}


	pDoc->SaveFile(filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}


// 讀取 playerInfo
void cData::loadplayerInfo() {
	//讀取 current scene
	std::string filePath = FileUtils::getInstance()->getWritablePath() + _fileName;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return;
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *playerInfo = root->FirstChildElement()->NextSiblingElement();
	XMLElement *Info = playerInfo->FirstChildElement();

	//?取子元素信息
	for (XMLElement* ch = Info->FirstChildElement(); ch ; ch = ch->NextSiblingElement()) {


		/*if ((!strcmp(ch->Name(), "currentMap"))) {

			}
		else {
			
			if(!strcmp(ch->Attribute("position"), "posX")) 
		}*/
	}

	



	//[6] ?放?存
	delete pDoc;
	// 讀取 player 位置

}