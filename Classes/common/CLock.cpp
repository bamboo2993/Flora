#include "CLock.h"


USING_NS_CC;
using namespace cocos2d;


CLock::CLock() {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/lock.plist");

	_btouch = false;
	_state = false;
	_check = false;
	_solved = false;
	
	_ans[0] = NULL;// the correct password
	_ans[1] = NULL;// the correct password
	_ans[2] = NULL;// the correct password
	_ans[3] = NULL;// the correct password
	_ans[4] = NULL;// the correct password

	for (int i = 0; i < MAX_LOCK_LETTERS*2; i++) {
		_buttonState[i] = false;
	}

	for (size_t i = 0; i <  MAX_LOCK_LETTERS; i++) {

		_now[i] = 1;
	}

}


CLock::~CLock() {

	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}


bool CLock::init(int totDigit, cocos2d::Node *lockNode, int maxNum, bool zero, int keyType) {
	_totDigit = totDigit;
	_rootNode= lockNode;
	_type = keyType;
	_existZero = zero;
	_maxNum = maxNum;
	//setup button rect
	Size size = _rootNode->getChildByName("1")->getContentSize();
	for (size_t i = 0; i < _totDigit *2; i++){
		auto s = std::to_string(i+1);
		Point pos = _rootNode->getChildByName(s)->getPosition();
		_buttonRect[i] = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	}

	//set key
	if (_type == 1) {
		for (int i = 0; i < _totDigit; i++) {
			auto result = "SR_L_0" + std::to_string(i+1) + ".png";
			auto name = std::to_string(i + 101);
			Point pos = _rootNode->getChildByName(name)->getPosition();

			_number[i] = Sprite::createWithSpriteFrameName("SR_L_01.png");
			_number[i]->setPosition(pos);
			_rootNode->addChild(_number[i], 1);
		}
	}

	this->schedule(CC_SCHEDULE_SELECTOR(CLock::doStep));
	return true;
}



void CLock::SetArea(const cocos2d::Rect &lockarea) {
	_lockAreaRect = lockarea;
}

void CLock::SetExitArea(const cocos2d::Rect &lockarea) {
	_exitRect = lockarea; // del button
}

void CLock::SetPassword(int num, int ans) {
	_ansNum = num;
	int x = ans;
	for (int i = _ansNum - 1; i >= 0; i--) {
		_ans[i] = x % 10;
		x = (int)x / 10;
	}

}

bool CLock::CheckAns() {
	for (size_t i = 0; i < _totDigit; i++){
		if(_now[i] != _ans[i])  return false;
	}
	return true;
}

void CLock::doStep(float dt) {
	if (_check) {
		if (CheckAns()) {
			log("correct");
			_rootNode->setVisible(false);
			_check = false;
			_solved = true;
		}
		_check = false;
	}
}

void CLock::reset() {
	for (int i = 0; i < 10; i++){	
		_number[i]->setVisible(false);
		_number[i]->setVisible(false);
		_number[i]->setVisible(false);
	}
	_now[0] = 1;
	_now[1] = 1;
	_now[2] = 1;
	_now[3] = 1;
	_now[4] = 1;
}

bool CLock::GetState(){
	return _state;
}
bool CLock::GetSolved() {
	return _solved;
}

bool CLock::TouchBegan(const cocos2d::Point pt) {
	


	if (_state) {

		if (_buttonRect[0].containsPoint(pt)) {
			_buttonState[0] = true;
		}
		else if (_buttonRect[1].containsPoint(pt)) {
			_buttonState[1] = true;
		}
		else if (_buttonRect[2].containsPoint(pt)) {
			_buttonState[2] = true;
		}
		else if (_buttonRect[3].containsPoint(pt)) {
			_buttonState[3] = true;
		}
		else if (_buttonRect[4].containsPoint(pt)) {
			_buttonState[4] = true;
		}
		else if (_buttonRect[5].containsPoint(pt)) {
			_buttonState[5] = true;
		}
		else if (_buttonRect[6].containsPoint(pt)) {
			_buttonState[6] = true;
		}
		else if (_buttonRect[7].containsPoint(pt)) {
			_buttonState[7] = true;
		}
			

		else if (_exitRect.containsPoint(pt)) {
			_btouch = true;
		}

	}
	else {
		if (_lockAreaRect.containsPoint(pt)) {
			_btouch = true;
		}
	}


	return true;
}



bool CLock::TouchMoved(const cocos2d::Point pt) {
	if (_state) {
		if (_buttonState[0]) {// 只有被按住的時候才處理
			if (!_buttonRect[0].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[0] = false;
				return(false);
			}
			else return(false);
		}

		else if (_buttonState[1]) {// 只有被按住的時候才處理
			if (!_buttonRect[1].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[1] = false;
				return(false);
			}
			else return(false);
		}
		else if (_buttonState[2]) {// 只有被按住的時候才處理
			if (!_buttonRect[2].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[2] = false;
				return(false);
			}
			else return(false);
		}
		else if (_buttonState[3]) {// 只有被按住的時候才處理
			if (!_buttonRect[3].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[3] = false;
				return(false);
			}
			else return(false);
		}
		else if (_buttonState[4]) {// 只有被按住的時候才處理
			if (!_buttonRect[4].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[4] = false;
				return(false);
			}
			else return(false);
		}
		else if (_buttonState[5]) {// 只有被按住的時候才處理
			if (!_buttonRect[5].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[5] = false;
				return(false);
			}
			else return(false);
		}
		else if (_buttonState[6]) {// 只有被按住的時候才處理
			if (!_buttonRect[6].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[6] = false;
				return(false);
			}
			else return(false);
		}
		else if (_buttonState[7]) {// 只有被按住的時候才處理
			if (!_buttonRect[7].containsPoint(pt)) {// 手指頭位置離開按鈕
				_buttonState[7] = false;
				return(false);
			}
			else return(false);
		}
		//else if (_btouch) {// 只有被按住的時候才處理
		//	if (!_exitRect.containsPoint(pt)) {// 手指頭位置離開按鈕
		//		_btouch = false;
		//		return(false);
		//	}
		//	else return(false);
		//}
	}
	else {
		if (_btouch) {
			if (!_lockAreaRect.containsPoint(pt)) {// 手指頭位置離開按鈕
				_btouch = false;
				return(false);
			}
			else return(false);
		}
	}



	return false;// 事後再移到按鈕上將被忽略
}
bool CLock::TouchEnded(const cocos2d::Point pt) {

	if (_state) {

		//close lock
		if (_exitRect.containsPoint(pt) && _btouch) {
			_rootNode->setVisible(false);
			_btouch = false;
			_state = false;
			return false;

		}

		//touch [arrow]
		//number 1
		if (_buttonState[0]) {
			if (_now[0] >= _maxNum) {
				if (_existZero) _now[0] = 0;
				else _now[0] = 1;
			}
			else _now[0]++;

			if (_type) {
				auto result = "SR_L_0" + std::to_string(_now[0]) + ".png";
				_number[0]->setSpriteFrame(result);
			}

			log("a   %d", _now[0]);
			_check = true;			
			_buttonState[0] = false;


		}
		else if (_buttonState[1]) {
			if (_existZero) {
				if (_now[0] == 0) _now[0] = _maxNum;
				else _now[0]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[0]) + ".png";
					_number[0]->setSpriteFrame(result);
				}
			}

			else {
				if (_now[0] == 1) _now[0] = _maxNum;
				else _now[0]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[0]) + ".png";
					_number[0]->setSpriteFrame(result);
				}

			}
			log("a   %d", _now[0]);
			_check = true;
			_buttonState[1] = false;
		}

		//number 2
		else if (_buttonState[2]) {
			if (_now[1] >= _maxNum) {
				if (_existZero) _now[1] = 0;
				else _now[1] = 1;
			}
			else _now[1]++;

			if (_type) {
				auto result = "SR_L_0" + std::to_string(_now[1]) + ".png";
				_number[1]->setSpriteFrame(result);
			}

			log("a   %d", _now[1]);
			_check = true;
			_buttonState[2] = false;
		}
		else if (_buttonState[3]) {
			if (_existZero) {
				if (_now[1] == 0) _now[1] = _maxNum;
				else _now[1]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[1]) + ".png";
					_number[1]->setSpriteFrame(result);
				}
			}

			else {
				if (_now[1] == 1) _now[1] = _maxNum;
				else _now[1]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[1]) + ".png";
					_number[1]->setSpriteFrame(result);
				}

			}
			log("a   %d", _now[1]);
			_check = true;
			_buttonState[3] = false;
		}

		//number 3
		else if (_buttonState[4]) {

			if (_now[2] >= _maxNum) {
				if (_existZero) _now[2] = 0;
				else _now[2] = 1;
			}
			else _now[2]++;

			if (_type) {
				auto result = "SR_L_0" + std::to_string(_now[2]) + ".png";
				_number[2]->setSpriteFrame(result);
			}

			log("a   %d", _now[2]);
			_check = true;
			_buttonState[4] = false;
		}
		else if (_buttonState[5]) {

			if (_existZero) {
				if (_now[2] == 0) _now[2] = _maxNum;
				else _now[2]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[2]) + ".png";
					_number[2]->setSpriteFrame(result);
				}
			}

			else {
				if (_now[2] == 1) _now[2] = _maxNum;
				else _now[2]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[2]) + ".png";
					_number[2]->setSpriteFrame(result);
				}

			}
			log("a   %d", _now[2]);
			_check = true;
			_buttonState[5] = false;
		}

		//number 4
		else if (_buttonState[6]) {
			if (_now[3] >= _maxNum) {
				if (_existZero) _now[3] = 0;
				else _now[3] = 1;
			}
			else _now[3]++;

			if (_type) {
				auto result = "SR_L_0" + std::to_string(_now[3]) + ".png";
				_number[3]->setSpriteFrame(result);
			}

			log("a   %d", _now[3]);
			_check = true;
			_buttonState[6] = false;
		}
		else if (_buttonState[7]) {
			if (_existZero) {
				if (_now[3] == 0) _now[3] = _maxNum;
				else _now[3]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[3]) + ".png";
					_number[3]->setSpriteFrame(result);
				}
			}

			else {
				if (_now[3] == 1) _now[3] = _maxNum;
				else _now[3]--;
				if (_type) {
					auto result = "SR_L_0" + std::to_string(_now[3]) + ".png";
					_number[3]->setSpriteFrame(result);
				}

			}
			log("a   %d", _now[3]);
			_check = true;
			_buttonState[7] = false;
		}

	}
	else {
		if (_lockAreaRect.containsPoint(pt)) { //open lock scene
			_rootNode->setVisible(true);
			_btouch = false;
			_state = true;
			return false;
		}
	}
}