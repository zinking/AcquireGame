#include "AppDelegate.h"

#include <vector>
#include <string>

#include "AcquireScene.h"
#include "AppMacros.h"

#include "Game/board.h"
#include "Game/definitions.h"
#include "Game/PlayerAI.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

	CCSize frameSize = pEGLView->getFrameSize();
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    
	initGameScene();
    return true;
}

void AppDelegate::initGameScene(){
	CCScene *pScene = AcquireScene::scene();
	pPlayerLayer = PlayerLayer::create();
	pPlayerLayer->setPlayerName("zhenw");
	pScene->addChild(pPlayerLayer,1);

	pAcquireLayer = AcquireScene::create();
	pScene->addChild(pAcquireLayer,0);

	initGameLogic();

	pAcquireLayer->initGameUI();
	pPlayerLayer->initPlayerUI();
    // run
	CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->runWithScene(pScene);

	//CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(CDataInitNetModule::updateTimer), this, 1.0f/15.0f, false);
}

void AppDelegate::initGameLogic(){
	pGame = new Game;
	//DefaultAI* pai1 = new DefaultAI("N1");
	pAcquireLayer->setGameStatus( &pGame->gs );

	DefaultAI* pai2 = new DefaultAI("N2");

	Player* pa = new Player(pPlayerLayer);
	pPlayerLayer->setPlayer( pa );
	//pai1->setPlayer( pa );
	Player* pb = new Player(pai2);
	pai2->setPlayer( pb );


	pGame->addPlayer( pa );
	pGame->addPlayer( pb );

	pGame->initPlayerWithATiles();
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
