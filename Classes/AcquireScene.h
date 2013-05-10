#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"
#include "Game\definitions.h"
#include "Game\PlayerAI.h"
#include "Popup.h"
#include <vector>
#include <string>

struct GameStatus;
struct Game;
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
	void initGameUI();

	void updateGameRender();
    static cocos2d::CCScene* scene();
	void showStatusPopup();

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
	GameStatus* pGame;
	Popup *popup;
	
public:
	bool inoperation;
	//GAMESTAGE stage;
    virtual bool init();  
	void updatePlayerRender();
	void updatePlayerLogic();
	void initPlayerUI();
	void setPlayerName( string name );

	void setGameStatus( GameStatus* gs );
   
    CREATE_FUNC(PlayerLayer);
	//LAYER_NODE_FUNC(TouchableLayer);  
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){ return true; }
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}

	const PlaceATileOrder decidePlaceATile( const GameStatus& bs );


	void askPlayerToPlaceTile();
	void onPlayerPlacedTile(cocos2d::CCObject *pSender);
	
};

class AcquireGameScene : public cocos2d::CCScene
{
	Game* pGame;
	AcquireScene* pAcquireLayer;
	PlayerLayer* pPlayerLayer;
public:
    AcquireGameScene() {};
    ~AcquireGameScene();
    bool init();
    CREATE_FUNC(AcquireGameScene);

	void updateGame( float dt);
  
    //CC_SYNTHESIZE_READONLY(GameOverLayer*, _layer, Layer);
};

#endif // __HELLOWORLD_SCENE_H__
