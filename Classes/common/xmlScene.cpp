#include "xmlScene.h"

using namespace cocos2d;



#define DATA_ROOT_NAME    "DataRoot"

#define XML_FILE_NAME "Data.xml"


xmlScene::xmlScene(const char *fileName){

	_fileName = fileName;
	_filePath = FileUtils::getInstance()->fullPathFromRelativeFile(_fileName, "");
	//std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(_fileName);

	//log("file name = %s", _fileName);



}

xmlScene::~xmlScene(){


}



void xmlScene::parseXML(cocos2d::Node *currentNode, const char *scene, CTrigger *ptrigger)
{

	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	log("parse");
	//	解析xml文件 parse sceneXML====================================================
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
		if (!strcmp(item->Attribute("state"), "scene")) {
			bool ret = (!strcmp(item->GetText(), "false"));

			if (true == ret) {
				const char *name = item->Attribute("name");

				auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
				obj->setVisible(false);
			}
			else {
				const char *name = item->Attribute("name");

				auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
				obj->setVisible(true);
			}
		}



	} 

	xmlTrigger::getInstance()->parseXML(scene, ptrigger);
	//[6] ?放?存
	delete pDoc;
}



void xmlScene::parseNodeXML(cocos2d::Node *currentNode, const char *node)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	log("parse");
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
		if (!strcmp(item->Attribute("state"), node)) {
			bool ret = (!strcmp(item->GetText(), "false"));

			if (true == ret) {
				const char *name = item->Attribute("name");

				auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
				obj->setVisible(false);
			}
			else {
				const char *name = item->Attribute("name");

				auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
				obj->setVisible(true);
			}
		}



	}
	//[6] ?放?存
	delete pDoc;
}



void xmlScene::editItemState(const int itemNo, bool state, cocos2d::Node *currentNode) {

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
		if (item->IntAttribute("no.") == itemNo) {
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

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;

}


void xmlScene::editItemState(const char *name, bool state, cocos2d::Node *currentNode, int start, int end) {
	
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
		if (item->IntAttribute("no.") >= start && item->IntAttribute("no.") <= end) {
			if ((!strcmp(item->Attribute("B_name"), name)) || (!strcmp(item->Attribute("name"), name))) {
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

		}
		

	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;

}



void xmlScene::editRangeState(const int itemNo, bool state, cocos2d::Node *currentNode, int start, int end) {

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
		if (item->IntAttribute("no.") >= start && item->IntAttribute("no.") <= end) {
			if (item->IntAttribute("no.") == itemNo) {

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


			else {
				if (state) {
					item->FirstChild()->SetValue("false");
					const char *name = item->Attribute("name");

					auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
					obj->setVisible(false);

				}
				else {
					item->FirstChild()->SetValue("true");

					const char *name = item->Attribute("name");

					auto obj = (cocos2d::Sprite*)currentNode->getChildByName(name);
					obj->setVisible(true);
				}
			}


		}


	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}

const char* xmlScene::getItemNameXML(int itemNum) {
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
	XMLElement *Scene = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = Scene->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if (item->IntAttribute("no.") == itemNum) {

			const char *name = item->Attribute("name");
			return name;
		}

	}
	//[6] ?放?存
	delete pDoc;
}


const char* xmlScene::getBItemNameXML(int itemNum) {
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
	XMLElement *Scene = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = Scene->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if (item->IntAttribute("no.") == itemNum) {

			const char *name = item->Attribute("B_name");
			return name;
		}

	}
	//[6] ?放?存
	delete pDoc;
}

int xmlScene::getItemNumXML(const char* itemName,bool type) {
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
	XMLElement *Scene = root->FirstChildElement();

	//?取子元素信息

	if (type == 0) {

		for (XMLElement* item = Scene->FirstChildElement(); item; item = item->NextSiblingElement()) {
			if (!strcmp(item->Attribute("name"), itemName)) {

				return (item->IntAttribute("no."));
			}

		}

	}


	else {

		for (XMLElement* item = Scene->FirstChildElement(); item; item = item->NextSiblingElement()) {
			if (!strcmp(item->Attribute("B_name"), itemName)) {

				return (item->IntAttribute("no."));
			}

		}

	}




	//[6] ?放?存
	delete pDoc;
}

void xmlScene::reset() {

}


