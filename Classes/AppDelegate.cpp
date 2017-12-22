#define SceneTransition 80


#include "AppDelegate.h"

#if SceneTransition == 1 || SceneTransition == 2
#include "GameScene\BMScene.h"

#elif SceneTransition == 3
#include "scene001\kitchenScene.h"

#elif SceneTransition == 10
#include "MainScene\MainScene.h"

#elif SceneTransition == 50
#include "MainScene\lockUsage.h"


#elif SceneTransition == 60
#include "GameScene\GRScene.h"

#elif SceneTransition == 70
#include "GameScene\SRScene.h"

#elif SceneTransition == 80
#include "GameScene\labScene.h"

#elif SceneTransition == 30
#include "ActionScene\RunScene\RunScene1.h"


#endif


USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("CocoStudio101", Rect(0, 0, 2048, 1536), 0.5f);
#else
		glview = GLViewImpl::create("CocoStudio101");
#endif
		director->setOpenGLView(glview);
	}

	director->getOpenGLView()->setDesignResolutionSize(2048, 1536, ResolutionPolicy::SHOW_ALL);

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60.0f);

	FileUtils::getInstance()->addSearchPath("res");


	// create a scene. it's an autorelease object


#if SceneTransition == 1
	auto scene = BMScene::createScene();
	director->runWithScene(scene);
#elif SceneTransition == 2
	auto scene = TransitionMoveInL::create(0.6f, BMScene::createScene());
	director->runWithScene(scene);

#elif SceneTransition == 3
	auto scene = TransitionZoomFlipX::create(0.6f, kitchenScene::createScene());
	director->runWithScene(scene);

#elif SceneTransition == 10
	auto scene = MainScene::createScene();
	director->runWithScene(scene);

#elif SceneTransition == 30
	auto scene = RunScene1::createScene();
	director->runWithScene(scene);


#elif SceneTransition == 50
	auto scene = SelectionScene::createScene();
	director->runWithScene(scene);


#elif SceneTransition == 60
	auto scene = GRScene::createScene();
	director->runWithScene(scene);

#elif SceneTransition == 70
	auto scene = SRScene::createScene();
	director->runWithScene(scene);

#elif SceneTransition == 80
	auto scene = labScene::createScene();
	director->runWithScene(scene);

#endif

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
