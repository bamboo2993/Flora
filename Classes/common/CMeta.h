#pragma once
#include "cocos2d.h"

#include "xmlScene.h"
#include "xmlBag .h"
#include "xmlItems.h"

USING_NS_CC;

class CMeta :public Node
{
private:
	//set up
	static CMeta *_pMeta;

	const char* _progress;

	bool _bBR_state;
	bool _bSR_state[3];
	bool _bL_state;




public:
	static CMeta* create();
	static CMeta* getInstance();
	void destroyInstance(); // 釋放取得資源

	CMeta();
	~CMeta();


	void Init(Point pos, CTrigger* trigger);

	void reset();

};

