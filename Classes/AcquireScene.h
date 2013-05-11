#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"
#include "Game\definitions.h"
#include "Game\PlayerAI.h"
#include "Game\order.h"
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

class PlayerLayer : public cocos2d::CCLayerColor, public DefaultAI
{
	GameStatus* pGame;
	CCTexture2D * pTexture;
	CCLabelTTF* pHintLabel;
	CCMenu* pMenu;
	CCMenuItemLabel* pMenuOK;

	int selected_tile_index;
	COMPANY selected_company;
	int amount;
	
public:
    virtual bool init();  
	void updatePlayerRender();
	void initPlayerUI();
	void setPlayerName( string name );
	void toggleDimmedBackGround( bool toggled );
	void createMenuContent();
	void clearMenuContent();

	void setGameStatus( GameStatus* gs );

	CCLabelTTF* createImageLabel( CCPoint& ipos, CCRect& rect, int fontsize, string text="" );
   
    CREATE_FUNC(PlayerLayer);
	//LAYER_NODE_FUNC(TouchableLayer);  
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){ return true; }
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}

	const ConvertStockOrder decideDoStockConversion(const GameStatus& bs );
	const PlaceATileOrder decidePlaceATile( const GameStatus& bs );
	const BuyStockOrder decideBuyStocks( const GameStatus& bs);
	const SellStockOrder decideSellStock( const GameStatus& bs);
	const SetupCompanyOrder decideSetupCompany( const GameStatus& bs );

	void askPlayerToPlaceTile();
	void onPlayerPlaceATile(CCObject *pSender);
	void onPlayerTilePlaced(CCObject *pSender);

	void askPlayerToBuyStock();
	void onPlayerBuyStock(CCObject *pSender);
	void onPlayerStockBought(CCObject *pSender);

	void askPlayerToSetupCompany();
	void onPlayerSetupCompany(CCObject *pSender);
	void onPlayerCompanySetup(CCObject *pSender);

	void askPlayerToSellStock();
	void onPlayerSellStock(CCObject *pSender);
	void onPlayerStockSold(CCObject *pSender);

	void askPlayerToConvertStock();
	void onPlayerConvertStock(CCObject *pSender);
	void onPlayerStockConverted(CCObject *pSender);

};

class AutomaticPlayerLayer : public PlayerLayer
{
public:
	CREATE_FUNC(AutomaticPlayerLayer);
	const ConvertStockOrder decideDoStockConversion(const GameStatus& bs ){ 
		return DefaultAI::decideDoStockConversion(bs);
	}
	const PlaceATileOrder decidePlaceATile( const GameStatus& bs ){
		return DefaultAI::decidePlaceATile(bs);
	}
	const BuyStockOrder decideBuyStocks( const GameStatus& bs){
		return DefaultAI::decideBuyStocks(bs);
	}
	const SellStockOrder decideSellStock( const GameStatus& bs){
		return DefaultAI::decideSellStock(bs);
	}
	const SetupCompanyOrder decideSetupCompany( const GameStatus& bs ){
		return DefaultAI::decideSetupCompany(bs);
	}

	void askPlayerToPlaceTile(){
		return DefaultAI::askPlayerToPlaceTile();
	}

	void askPlayerToBuyStock(){
		return DefaultAI::askPlayerToBuyStock();
	}

	void askPlayerToSetupCompany(){
		return DefaultAI::askPlayerToSetupCompany();
	}

	void askPlayerToSellStock(){
		return DefaultAI::askPlayerToSellStock();
	}

	void askPlayerToConvertStock(){
		return DefaultAI::askPlayerToConvertStock();
	}

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
