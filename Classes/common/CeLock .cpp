#include "CeLock .h"


USING_NS_CC;
using namespace cocos2d;



CeLock::CeLock(bool black) {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/lock.plist");
	_ansNum = NULL; //number of character for password
	_ans[0] = NULL;// the correct password
	_ans[1] = NULL;// the correct password
	_ans[2] = NULL;// the correct password
	_ans[3] = NULL;// the correct password
	_ans[4] = NULL;// the correct password

	_posX = 0.0f; _posY = 0.0f;

	_btouch = false;
	_state = false;
	_check = false;

	for (int i = 0; i < MAX_LOCK_LETTERS; i++) {
		_number[i] = Sprite::createWithSpriteFrameName("elock00.png");
		//_number[i]->setPosition(900, 200);
		_number[i]->setVisible(false);

		this->addChild(_number[i],1);
	}
	for (int i = 0; i < 12; i++) {
		_buttonState[i] = false;
	}

	_error = false;
	_openState = false;
	_Error= Sprite::createWithSpriteFrameName("elock_ERROR.png");
	
	_Error->setVisible(false);
	this->addChild(_Error, 1);

	if(black) _exit = Sprite::create("common/go back2.png");  // 使用 create 函式,給予檔名即可
	else _exit = Sprite::create("common/go back.png");  // 使用 create 函式,給予檔名即可

	_exit->setPosition(108.49f, 1426.32f);
	Size size = _exit->getContentSize();
	Point pos = _exit->getPosition();
	this->addChild(_exit, 1);
	_exitRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
}


CeLock::~CeLock() {

	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool CeLock::init(const char* bgImage) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	//background image
	auto background = (cocos2d::Sprite *) Sprite::create(bgImage);
	//set position
	background->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	this->addChild(background,0);


	this->setVisible(false);
	return true;
}




void CeLock::SetArea(const cocos2d::Rect &lockarea) {
	_lockAreaRect = lockarea;
}

void CeLock::SetExitArea(cocos2d::Point BLpoint, float width, float height) {
	_exitRect = Rect(BLpoint.x, BLpoint.y, width, height); // del button
}


void CeLock::SetKeyArea(cocos2d::Point BLpoint, float w, float h){	
	//setup button
	float width = w / 3; //width of a button
	float height = h / 3; //height of a button

	//third roll button (from top)
	_buttonRect[1] = Rect(BLpoint.x, BLpoint.y , width, height);
	_buttonRect[2] = Rect(BLpoint.x + width, BLpoint.y , width, height);
	_buttonRect[3] = Rect(BLpoint.x + 2 * width, BLpoint.y , width, height);

	//second roll button (from top)
	_buttonRect[4] = Rect(BLpoint.x, BLpoint.y + height, width, height);
	_buttonRect[5] = Rect(BLpoint.x + width, BLpoint.y + height, width, height);
	_buttonRect[6] = Rect(BLpoint.x + 2 * width, BLpoint.y + height, width, height);

	//top roll button
	_buttonRect[7] = Rect(BLpoint.x, BLpoint.y + 2 * height, width, height);
	_buttonRect[8] = Rect(BLpoint.x + width, BLpoint.y + 2 * height, width, height);
	_buttonRect[9] = Rect(BLpoint.x + 2 * width, BLpoint.y + 2 * height, width, height);

}

void CeLock::SetEnterArea(cocos2d::Point BLpoint, float w, float h) {
	//setup button
	float width = w / 3; //width of a button

	//botom 3 button
	_buttonRect[10] = Rect(BLpoint.x, BLpoint.y, width, h); // del button
	_buttonRect[0] = Rect(BLpoint.x + width, BLpoint.y, width, h);
	_buttonRect[11] = Rect(BLpoint.x + 2 * width, BLpoint.y, width, h); //enter button


}

void CeLock::SetNumAppear(float x, float y){
	_posX = x;
	_posY = y;
	_Error->setPosition(1024+20, _posY);
}

void CeLock::SetPassword(int num, int ans){
	_ansNum = num;
	int x = ans;
	for (int i = _ansNum-1; i >= 0; i--) {
		_ans[i] = x % 10;
		x = (int)x / 10;
	}

}

void CeLock::doStep(float dt) {
	
	if (_openState) {
		this->setVisible(true);
		_state = true;
		_openState = false;
	}

	if (_error) {
		_tot += dt;
		close();
		_state = true;
		_Error->setVisible(true);
		// blink====================
		if (_tot>0.5f){
			_tot -= 0.5f;
			_Error->setVisible(false);
			_error = false;

		}
		else {
			float t = cosf(_tot * 360 * 3.1415926 / 180.0f);
			_Error->setOpacity(255 * t);
		}
		

	}


}
void CeLock::SetReached(bool x) {
	_openState = x;
}



void CeLock::close() {
	_typedNum.clear();
	_itypedNum.clear();

	_btouch = false;
	_state = false;
	_check = false;
	for (int i = 0; i < 12; i++) {
		_buttonState[i] = false;
	}
	//clear answer and number sprite
	for (int i = 0; i < MAX_LOCK_LETTERS; i++) {
		_number[i]->setVisible(false);
	}
}

void CeLock::reset() {
	//remove all from list
	_typedNum.clear();
	_itypedNum.clear();
	_posX = 0.0f; _posY = 0.0f;

	_ansNum = 0; //number of character for password
	//clear answer and number sprite
	for (int i = 0; i < MAX_LOCK_LETTERS; i++) {
		_number[i] = nullptr;
		_ans[i] = NULL;
	}

	_btouch = false;
	_state = false;
	_check = false;
	for (int i = 0; i < 12; i++) {
		_buttonState[i] = false;
	}
}



void CeLock::GetNum(cocos2d::Sprite* x) {
	_typedNum.push_back(x);

}

void CeLock::DeleteNum() {
	(_typedNum.back())->setVisible(false);
	_typedNum.pop_back();

	_itypedNum.pop_back();

	//_typedNum.back()->setSpriteFrame();
}


void CeLock::ArrangeNum() {
	std::list <cocos2d::Sprite*> ::iterator it;
	int i;
	for (it = _typedNum.begin(), i = 1; it != _typedNum.end(), (i - 1) < _typedNum.size(); ++it, i++) {

		(*it)->setPosition((_posX + ((i - 1)*40.0f) - _typedNum.size()* 40.0f), _posY);
		(*it)->setVisible(true); // shows in bag


		auto q=(*it)->getPosition();

	}

}


bool CeLock::CheckAns() {
	if (_typedNum.size() == _ansNum) {
		std::list <int> ::iterator it;
		int i;
		for (it = _itypedNum.begin(), i = 0; it != _itypedNum.end(); ++it, i++) {
			if (*it != _ans[i]) return false;
		}
		return true;
	}
	else return false;
}


bool CeLock::GetState()
{
	return _state;
}

bool CeLock::TouchBegan(const cocos2d::Point pt) {


	if (_state ) {
		if (!_error) {

			if (_typedNum.size() < MAX_LOCK_LETTERS) {
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
				else if (_buttonRect[8].containsPoint(pt)) {
					_buttonState[8] = true;
				}
				else if (_buttonRect[9].containsPoint(pt)) {
					_buttonState[9] = true;
				}
			}

			if (_buttonRect[10].containsPoint(pt)) {
				if (_typedNum.size() > 0) {
					_buttonState[10] = true;
				}
			}


			else if (_buttonRect[11].containsPoint(pt)) {
				_buttonState[11] = true;
			}

			else if (_exitRect.containsPoint(pt)) {
				_btouch = true;
			}
		}


	}
	else {
		if (_lockAreaRect.containsPoint(pt)) {
			_btouch = true;
		}
	}
	

	return true;
}



bool CeLock::TouchMoved(const cocos2d::Point pt) {
	if (_state) {
		if (!_error) {
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
			else if (_buttonState[8]) {// 只有被按住的時候才處理
				if (!_buttonRect[8].containsPoint(pt)) {// 手指頭位置離開按鈕
					_buttonState[8] = false;
					return(false);
				}
				else return(false);
			}
			else if (_buttonState[9]) {// 只有被按住的時候才處理
				if (!_buttonRect[9].containsPoint(pt)) {// 手指頭位置離開按鈕
					_buttonState[9] = false;
					return(false);
				}
				else return(false);
			}
			else if (_buttonState[10]) {// 只有被按住的時候才處理
				if (!_buttonRect[10].containsPoint(pt)) {// 手指頭位置離開按鈕
					_buttonState[10] = false;
					return(false);
				}
				else return(false);
			}
			else if (_buttonState[11]) {// 只有被按住的時候才處理
				if (!_buttonRect[11].containsPoint(pt)) {// 手指頭位置離開按鈕
					_buttonState[11] = false;
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
bool CeLock::TouchEnded(const cocos2d::Point pt) {
	if (_state) {
		if (!_error) {
			//close lock
			if (_exitRect.containsPoint(pt) && _btouch) {

				this->setVisible(false);
				_btouch = false;
				_state = false;
				close();
				return false;

			}

			//touch [number key]

			if (_buttonState[0]) {
				//_number[_typedNum.size()]->stopAllActions();
				_number[_typedNum.size()]->setSpriteFrame("elock00.png");
				//_number[_typedNum.size()]->setTexture("1.png");
				GetNum(_number[_typedNum.size()]); //add sprite to list
				ArrangeNum();
				_itypedNum.push_back(0); //add int to list
				_buttonState[0] = false;
			}
			else if (_buttonState[1]) {
				_number[_typedNum.size()]->setSpriteFrame("elock01.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(1); //add int to list
				_buttonState[1] = false;
			}
			else if (_buttonState[2]) {
				_number[_typedNum.size()]->setSpriteFrame("elock02.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(2); //add int to list
				_buttonState[2] = false;
			}
			else if (_buttonState[3]) {
				_number[_typedNum.size()]->setSpriteFrame("elock03.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(3); //add int to list
				_buttonState[3] = false;
			}
			else if (_buttonState[4]) {
				_number[_typedNum.size()]->setSpriteFrame("elock04.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(4); //add int to list
				_buttonState[4] = false;
			}
			else if (_buttonState[5]) {
				_number[_typedNum.size()]->setSpriteFrame("elock05.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(5); //add int to list
				_buttonState[5] = false;
			}
			else if (_buttonState[6]) {
				_number[_typedNum.size()]->setSpriteFrame("elock06.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(6); //add int to list
				_buttonState[6] = false;
			}
			else if (_buttonState[7]) {
				_number[_typedNum.size()]->setSpriteFrame("elock07.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(7); //add int to list
				_buttonState[7] = false;
			}
			else if (_buttonState[8]) {
				_number[_typedNum.size()]->setSpriteFrame("elock08.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(8); //add int to list
				_buttonState[8] = false;
			}
			else if (_buttonState[9]) {
				_number[_typedNum.size()]->setSpriteFrame("elock09.png");
				GetNum(_number[_typedNum.size()]);
				ArrangeNum();
				_itypedNum.push_back(9); //add int to list
				_buttonState[9] = false;
			}



			//touch [delete]
			else if (_buttonState[10]) {
				DeleteNum();
				ArrangeNum();
				_buttonState[10] = false;
			}

			//touch [ok]
			else if (_buttonState[11]) {
				if (CheckAns()) {
					log("lock open");
					//switch scene
					return true;
				}
				else {
					log("wrong answer");
					_buttonState[11] = false;
					_error = true;
					_Error->setVisible(true);
					return false;
				}
			}

		}
		
	}
	else {
		if (_lockAreaRect.containsPoint(pt)) { //open lock scene
			_btouch = false;
			return false;
		}
		return false;
	}







	return false;


	

}