#pragma once
#include "cocostudio/CocoStudio.h"

#include "xmlBag .h"
#include "xmlItems.h"
#include "xmlScene.h"

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;


class  CMix {

private:
	const char *_targetItem1[4];
	const char *_targetItem2[4];
	const char *_targetItem3[4];
	const char *_targetItem4[4];
	const char *_targetItem5[4];

	int _target1[4];
	int _target2[4];
	int _target3[4];
	int _target4[4];
	int _target5[4];

	int _itargetNum[5];
	int _result1[2];
	int _result2[2];
	int _result3[2];
	int _result4[2];
	int _result5[2];
	int ir[5];
	int _step;
	int _currentShown;
	int _currentSteps;


	bool _bstate;
	int _ioriginalItem;
	int _ifailureItem;

	const char *_targetItemB[2];
	int _resultB;

	int _combiNum[10];

	//int _itargetNum;
	//const char *_targetItem[4];
	int _count;
	//xmlBag *_xmlbag;
	//xmlItem *_xmlitem;
	xmlScene *_xmlscene;
	const char *_mixItem[7];
/*	int _result[4];*/



	std::string _filePath;
	std::string _fileName;

	
	cocos2d::Rect _target[2];

	cocos2d::Node *_Node;

	bool _btype;

	void setFailure();
public:
	void init(int original, int failure, bool type=true);
	void SetCombination(int combiA, bool typeA, int combiB, bool typeB, int result, int step);// type: true->read item name; false->read item Bname



	void SetCombination(int combi, int result);

	void SetCombinationB(int combiA, int combiB, int result);



	void doStep(float dt);
	void mixing(int item, bool type=1);

	

	int getCurrentObj();
	bool getCompleteStep();
	

	void setEmpty(int item, int start, int end);





	void Clear();
	void reset();

	CMix(cocos2d::Node *currentNode);
	~CMix();


};