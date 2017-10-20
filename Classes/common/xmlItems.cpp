#include "xmlItems.h"


using namespace cocos2d;

USING_NS_CC;

xmlItem *xmlItem::_pXMLItem = nullptr;

xmlItem* xmlItem::getInstance() {

	if (_pXMLItem == nullptr) _pXMLItem = new(std::nothrow)xmlItem();
	return _pXMLItem;

}


void xmlItem::destroyInstance() { CC_SAFE_DELETE(_pXMLItem); }



xmlItem::xmlItem(){

	_fileName = "./res/xml/xmlfile_item.xml";
	_filePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(_fileName, "");
	//std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(_fileName);


}

xmlItem::~xmlItem(){

}


const char* xmlItem::getItemNameXML(int itemNum) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if (item->IntAttribute("no.") == itemNum) {

			const char *name = item->Attribute("name");
			return name;
		}

	}
	//[6] ?放?存
	delete pDoc;
}

bool xmlItem::getItemStateXML(const char * cname)
{
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {

			auto state = item->BoolAttribute("state");
			return state;
		}

	}
	//[6] ?放?存
	delete pDoc;
}





bool xmlItem::getStagnantXML(const char *cname) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {

			auto bstagnant = item->BoolAttribute("stagnant");
			return bstagnant;
		}

	}
	//[6] ?放?存
	delete pDoc;
}

int xmlItem::getTargetNumXML(const char *cname) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {

			auto itargetnum = item->IntAttribute("targetnum");
			return itargetnum;
		}

	}
	//[6] ?放?存
	delete pDoc;
}

cocos2d::Rect xmlItem::getTargetRectXML(const char *cname, const int targetno) {
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	//	解析xml文件
	XMLError errorId = pDoc->LoadFile(_filePath.c_str());

	//判?是否解析??
	if (errorId != 0) {
		//xml格式??
		CCLOG("Parse Error!");
		return cocos2d::Rect();
	}

	//?取根元素
	XMLElement *root = pDoc->RootElement();
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {
			XMLElement *target1 = item->FirstChildElement();
			XMLElement *target2 = target1->NextSiblingElement();

			if (targetno == 1) {
				auto x = target1->FloatAttribute("x");
				auto y = target1->FloatAttribute("y");
				auto h = target1->FloatAttribute("height");
				auto w = target1->FloatAttribute("width");
				auto rect= cocos2d::Rect(x , y , w, h);
				return rect;
			}
			else {
				auto x = target2->FloatAttribute("x");
				auto y = target2->FloatAttribute("y");
				auto h = target2->FloatAttribute("height");
				auto w = target2->FloatAttribute("width");
				auto rect = cocos2d::Rect(x, y , w, h);
				return rect;
			}
		}

	}
	//[6] ?放?存
	delete pDoc;
}

bool xmlItem::getRetakeXML(const char *cname) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {
			XMLElement *retake = item->LastChildElement();

			bool ret = (!strcmp(retake->GetText(), "true"));

			if (true == ret) return true;
			else false;
		}

	}
	//[6] ?放?存
	delete pDoc;
}



const char* xmlItem::getTargetSceneXML(const char *cname, bool num) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {
			if (num == 0) {
				XMLElement* e = item->FirstChildElement();
				const char *scene = e->Attribute("scene");
				return scene;
			}
			else {
				XMLElement* e = item->FirstChildElement()->NextSiblingElement();
				const char *scene = e->Attribute("scene");
				return scene;
			}
		}

	}
	//[6] ?放?存
	delete pDoc;
}


const char* xmlItem::getTriggerSceneXML(const char *cname) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {

			const char *scene = item->Attribute("scene");
			return scene;
		}

	}
	//[6] ?放?存
	delete pDoc;
}


int xmlItem::getTriggerCodeXML(const char *cname) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {
			XMLElement *retake = item->LastChildElement();
			auto itriggercode = retake->IntAttribute("triggercode");
			return itriggercode;
		}

	}
	//[6] ?放?存
	delete pDoc;
}


void xmlItem::setTargetRectXML(int itemNum, cocos2d::Rect area) {
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {

		int n = std::atoi(item->Attribute("no.")); //char 轉  int

		if (itemNum == n) {

			auto x = area.getMinX();
			auto y = area.getMinY();

			auto width = area.getMaxX() - area.getMinX();
			auto height = area.getMaxY() - area.getMinY();



			XMLElement* e = item->FirstChildElement();

			e->SetAttribute("x", x);
			e->SetAttribute("y", y);
			e->SetAttribute("width", width);
			e->SetAttribute("height", height);

		}
	}

	pDoc->SaveFile(_filePath.c_str());
	//[6] ?放?存
	delete pDoc;
}



int xmlItem::checkMixingXML(const char * cname, const char * mixname)
{
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
	XMLElement *List = root->FirstChildElement();

	//?取子元素信息

	for (XMLElement* item = List->FirstChildElement(); item; item = item->NextSiblingElement()) {
		if ((!strcmp(item->Attribute("name"), cname))) {
			XMLElement *mix = item->FirstChildElement();

			if ((!strcmp(mix->Attribute("name"), mixname))) {

				auto r = mix->IntAttribute("ToBe");
				return r;
			}


			else return -1;
		}

	}
	//[6] ?放?存
	delete pDoc;
}

