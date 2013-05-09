#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Game\definitions.h"
#include "Game\PlayerAI.h"
#include <vector>
#include <string>
#include "AppMacros.h"
struct GameStatus;
struct Player;
class ATileSprite;
class ATileLabel;

class AcquireScene : public cocos2d::CCLayer
{
	GameStatus* pGame;
	ATileLabel* ats[HEIGH][WIDTH];
	std::vector<cocos2d::CCSprite*> pcon;
public:
    virtual bool init();  
	void initGameLogic();
	void initGameUI();

	void updateGameRender();
    static cocos2d::CCScene* scene();

	void setGameStatus( GameStatus* gs );
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuClickCallBack(CCObject* pSender);
   
    CREATE_FUNC(AcquireScene);
	//LAYER_NODE_FUNC(TouchableLayer);  
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){ return true; }
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	
};

class PlayerLayer : public cocos2d::CCLayer, public DefaultAI
{
public:
    virtual bool init();  
	void updatePlayerRender();
	void initPlayerUI();
	void setPlayerName( string name );
   
    CREATE_FUNC(PlayerLayer);
	//LAYER_NODE_FUNC(TouchableLayer);  
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){ return true; }
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	
};

#endif // __HELLOWORLD_SCENE_H__
