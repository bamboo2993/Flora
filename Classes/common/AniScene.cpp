#include "AniScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;


AniScene::AniScene() {
	idx = 0;
	_tot = 0;
	_skiptot = 0;
}
AniScene::~AniScene(){

}

void AniScene::init(Point &pt, Layer &parent, int num, char* picname) {

	 idx = 0;
	_tot = 0;
	picnum = num;
	char tmp[50];
	for (int j = 1; j <= num; j++) {
		char picnames[50];
		strcpy_s(picnames, picname);
		sprintf(tmp,"%02d.png",j);
		strcat(picnames,tmp);
		_SpriteCD[j-1] = Sprite::createWithSpriteFrameName(picnames);
	}

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("../music/memories.mp3", true);
	//_SpriteCD[0] = Sprite::createWithSpriteFrameName("010.png");
	//_SpriteCD[1] = Sprite::createWithSpriteFrameName("009.png");
	//_SpriteCD[2] = Sprite::createWithSpriteFrameName("008.png");
	//_SpriteCD[3] = Sprite::createWithSpriteFrameName("007.png");
	//_SpriteCD[4] = Sprite::createWithSpriteFrameName("006.png");
	//_SpriteCD[5] = Sprite::createWithSpriteFrameName("005.png");
	//_SpriteCD[6] = Sprite::createWithSpriteFrameName("004.png");
	//_SpriteCD[7] = Sprite::createWithSpriteFrameName("003.png");
	//_SpriteCD[8] = Sprite::createWithSpriteFrameName("002.png");
	//_SpriteCD[9] = Sprite::createWithSpriteFrameName("001.png");


	for (int i = num-1; i >= 0; i--) {
		_SpriteCD[i]->setPosition(pt);
		parent.addChild(_SpriteCD[i],20001);
		//_SpriteCD[i]->setVisible(false);
		_SpriteCD[i]->setCascadeOpacityEnabled(true);
	}
	_SpriteCD[idx]->setVisible(true);
}

bool AniScene::doStep(float dt)
{
	if (_tot >= 6.0f) {
		_tot -= 6.0f;
		idx++;
		if (idx >= picnum) {
			return(true);
		}
	}
	else {
		if (_tot > 4.0f) {
			// weird
			float t = sinf(_tot * 45 * 3.1415926 / 180.0f);
			_SpriteCD[idx]->setOpacity(255 * t);
			if (idx == picnum - 1) {
				SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(t);
			}
		}
	}
	_tot += dt;
	
	return(false);
}

bool AniScene::skip(float dt) {
	_skiptot += dt;
	float t = cosf(_skiptot * 45 * 3.1415926 / 180.0f);
	for (int i = idx; i < picnum; i++) {
		_SpriteCD[i]->setOpacity(255 * t);
	}
//	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(t);
	if (_skiptot > 2.0f) { 
		for (int i = idx; i < picnum; i++) {
			_SpriteCD[i]->setVisible(false);
		}
		return true; }
	else return false;
}

void AniScene::setVis(bool b) {
	for (int i = 0; i < picnum; i++) {
		_SpriteCD[i]->setVisible(b);
	}
}