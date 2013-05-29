#include "AcquireScene.h"


USING_NS_CC;
#include "Game/board.h"
#include "Game/definitions.h"
#include "Game/PlayerAI.h"
#include "Game/order.h"
#include "ATileSprite.h"
#include <algorithm>


ccColor3B COMPANYCOLOR[MAXNUMBER_OF_STOCKS]={ 
	ccc3(255,0,0),   ccc3(0,255,0),   ccc3(0,0,255),
	ccc3(255,255,0), ccc3(255,0,255), ccc3(0,255,255),
	ccc3(255,255,255)
};
ccColor4B COMPANYCOLOR4[MAXNUMBER_OF_STOCKS+1]={ 
	ccc4(255,0,0,100),   ccc4(0,255,0,100),   ccc4(0,0,255,100),
	ccc4(255,255,0,100), ccc4(255,0,255,100), ccc4(0,255,255,100),
	ccc4(255,128,255,100), ccc4( 125,125,125,100)
};

static const CCSize lblsize = CCSize( BLOCK_SIZE*4,BLOCK_SIZE*1.5);


CCLabelTTF* createTextureLabel( const CCPoint& ipos, const char* pTextureName, int fontsize , string text){
	CCLabelTTF* pl = CCLabelTTF::create( text.c_str(), "Arial", fontsize, 
		CCSize( BLOCK_SIZE,BLOCK_SIZE*2), kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom); 
	CCSprite* avtar  = CCSprite::createWithSpriteFrameName(pTextureName);
	avtar->setPosition(ipos);
	avtar->setAnchorPoint(ccp(0,0));
	pl->addChild(avtar);
	return pl;
}


void AcquireScene::setGameStatus( GameStatus* gs ){
	pGame = gs;
}

void AcquireScene::initGameUI()
{
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + sz.height - BLOCK_SIZE * HEIGH - BLOCK_SIZE/4 ;
	for( int i=0; i<HEIGH; i++ ){//row
		for( int j=0; j<WIDTH; j++ ){//col
			ATileLabel* at = ATileLabel::create();
			at->setPosition( ccp(  (j+0.5)*BLOCK_SIZE+dx , i*BLOCK_SIZE+dy ) );
			at->initTile(i,j);
			ats[i][j] = at;
			this->addChild(at);
		}
	}

	dx = op.x + BLOCK_SIZE;
	dy = op.y + sz.height -BLOCK_SIZE/2  ;

	CCLabelTTF* pl = CCLabelTTF::create( "", "Arial", FONT_SIZE*1.2, 
		CCSize( BLOCK_SIZE*WIDTH,BLOCK_SIZE), kCCTextAlignmentLeft,kCCVerticalTextAlignmentBottom); 
	pl->setPosition(  ccp(dx,dy) );
	pl->setAnchorPoint(ccp(0,0));
	this->addChild( pl, 1, 11 );

	updateGameRender();
}

extern char COMPANYNAME[NUMBER_OF_STOCKS][20];
extern char COMPANYSHORTNAME[NUMBER_OF_STOCKS][2];
extern char info[2000];
void AcquireScene::updateGameRender(){
	const vector<Block> currentblocks = pGame->getAllBlocks();
	for( auto it=currentblocks.begin(); it!=currentblocks.end(); ++it ){
		for( auto it2=it->ATiles.begin(); it2 != it->ATiles.end(); ++it2 ){
			ATileLabel* pal = ats[ it2->row ][ it2->col ];
			pal->updateCaption( it->c );
		}
	}

	CCLabelTTF* pl = (CCLabelTTF*)this->getChildByTag( 11 );
	pl->setString( pGame->getLastestMessage().c_str() );
}

bool AcquireScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* sp_m   = CCSprite::createWithSpriteFrameName( "flower" );
	CCSprite* sp_ms  = CCSprite::createWithSpriteFrameName( "flower" );
	sp_m->setScale( 0.5f );
	sp_ms->setScale( 0.6f );
	//sp_ms->setRotation( 60 );
    CCMenuItemSprite *pci = CCMenuItemSprite::create(
       sp_m,
       sp_ms,
        this,
        menu_selector(AcquireScene::menuClickCallBack)
	);
    
	pci->setPosition(ccp(op.x + sz.width - pci->getContentSize().width/2 ,op.y + pci->getContentSize().height/2));
    CCMenu* pMenu = CCMenu::create(pci, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
	this->setTouchEnabled(true);

	pmenulayer = AcquireScene::create_pa_layer();
	pmenulayer->setVisible( false );
	this->addChild( pmenulayer,5 );
    pmenulayer->setPosition( ccp(op.x, op.y ));
    return true;
}

CCLayerColor* AcquireScene::create_pa_layer(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLayerColor* palayer = AcquireGameScene::create_bg_layer();
	palayer->setColor( ccWHITE );
	palayer->setOpacity( 200 );
	CCMenu* pmenupause = CCMenu::create();
	CCMenuItemSprite *pci;
	CCSprite* sp_exit  = CCSprite::createWithSpriteFrameName( "exit" );
	CCSprite* sp_canc  = CCSprite::createWithSpriteFrameName( "cancel" );
	pci = CCMenuItemSprite::create(sp_exit,sp_exit,palayer,menu_selector(AcquireSelectScene::onExitGMSelected));
	pci->setPosition( ccp(0,0) );
	pmenupause->addChild( pci );
	pci = CCMenuItemSprite::create(sp_canc,sp_canc,palayer,menu_selector(AcquireScene::onCancelGMSelected));
	pci->setPosition( ccp(0,BLOCK_SIZE*1.5) );
	pmenupause->addChild( pci );
	pmenupause->setPosition( sz.width*0.5f + op.x , sz.height*0.25f + op.y );
	palayer->addChild( pmenupause );

	CCSprite* spstockfrmae  = CCSprite::createWithSpriteFrameName( "stockframe" );
	palayer->addChild( spstockfrmae );
	spstockfrmae->setPosition(ccp(BLOCK_SIZE*4,BLOCK_SIZE*4));
	CCLabelTTF* pl  = CCLabelTTF::create( "", "Arial", FONT_SIZE*1, 
			CCSize( BLOCK_SIZE*6,BLOCK_SIZE*6), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
	pl->setPosition( ccp(BLOCK_SIZE*4.75,BLOCK_SIZE*3.5) );
	pl->setColor(ccBLUE);
	palayer->addChild( pl, 0, 111 );

	return palayer;
}

string AcquireScene::getGameStatusInfo( GameStatus* gs ){
	string result;
	//sprintf_s(info, "%12s", "COMPANY" );
	//result += info;
	/*for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		sprintf_s(info, "%7s", COMPANYSHORTNAME[i] );
		result += info;
	}
	result += "\n";*/

	//sprintf_s(info, "%12s", "STOCK PRICE" );
	//result += info;
	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		sprintf_s(info, "%7d", gs->getStockPrice( (COMPANY)i ) );
		result += info;
	}
	result += "\n";

	//sprintf_s(info, "%12s", "MAJOR BONUS" );
	//result += info;
	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		sprintf_s(info, "%7d", gs->getStockMajor( (COMPANY)i ) );
		result += info;
	}
	result += "\n";

	//sprintf_s(info, "%12s", "MINOR BONUS" );
	//result += info;
	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		sprintf_s(info, "%7d", gs->getStockMinor( (COMPANY)i ) );
		result += info;
	}
	result += "\n";

	return result;
}

void AcquireScene::onCancelGMSelected(CCObject* pSender)
{
	CCMenuItemSprite *pci = (CCMenuItemSprite*)pSender;
	pci->getParent()->getParent()->setVisible(false);
}

void AcquireScene::menuClickCallBack(CCObject* pSender)
{
	CCLabelTTF* pl = (CCLabelTTF*)pmenulayer->getChildByTag(111);
	pl->setString( getGameStatusInfo(pGame).c_str() );
	pmenulayer->setVisible( true );
}


//////////////////////////////////////////////////////////////////////
void PlayerLayer::initPlayerUI(){
	pinfolayer = createInfoLayer();
	addChild( pinfolayer );
	updateLayerInfo();

	poperlayer = createOperLayer();
	addChild( poperlayer );
	poperlayer->setVisible(false );

	pHintLabel = (CCLabelTTF* )poperlayer->getChildByTag(111);

	pTileMenu = createTileOPMenu();
	poperlayer->addChild(pTileMenu);
	pTileMenu->setVisible(false);
	
	pCompMenu = createCompOPMenu();
	poperlayer->addChild(pCompMenu);
	pCompMenu->setVisible(false);
}

CCLayerColor* PlayerLayer::createOperLayer()
{
	CCLayerColor* poperlayer = CCLayerColor::create();
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + BLOCK_SIZE*1.5f;
	CCLabelTTF* pl = CCLabelTTF::create( "", "Arial", FONT_SIZE*1.5, 
		CCSize( BLOCK_SIZE*WIDTH,BLOCK_SIZE), kCCTextAlignmentLeft,kCCVerticalTextAlignmentBottom); 
	pl->setPosition(  ccp(dx,dy) );
	pl->setAnchorPoint(ccp(0,0));
	
	pl->setVisible( false );
	poperlayer->addChild( pl, 1, 111 );
	return poperlayer;
}

CCLayerColor* PlayerLayer::createInfoLayer()
{
	CCLayerColor* pinfolayer = CCLayerColor::create();
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + sz.height - BLOCK_SIZE * HEIGH - BLOCK_SIZE/4 ;

	CCLabelTTF* pl = createTextureLabel(ccp(0,BLOCK_SIZE/3),"avtar",FONT_SIZE,"PLAYER");
	pl->setPosition(  ccp(BLOCK_SIZE/2,BLOCK_SIZE) );
	pinfolayer->addChild( pl, 0, 0 );
	for( UINT i=0; i<NUMBER_OF_ATileS_EACH; i++ ){
		pl = createTextureLabel(ccp(0,-FONT_SIZE/2),"tile",FONT_SIZE*1.2,"");
		pl->setPosition(  ccp(BLOCK_SIZE*(2+i),BLOCK_SIZE*1.25) );
		pinfolayer->addChild( pl, 0, 1+i );
	}

	pl = createTextureLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),"money",FONT_SIZE*1.2,"");
	pl->setPosition(  ccp(BLOCK_SIZE*9.5,BLOCK_SIZE*1.25) );
	pinfolayer->addChild( pl, 0, 8 );

	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		sprintf_s( info, "company%d",i );
		pl = createTextureLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),info,FONT_SIZE*1.5, "");
		pl->setPosition(  ccp(BLOCK_SIZE*11.5f,dy+(1+i)*BLOCK_SIZE) );
		pinfolayer->addChild( pl, 0, 9+i );
	}
	return pinfolayer;
}


void PlayerLayer::updateLayerInfo(){
	UINT i = 0;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		CCLabelTTF* pl = (CCLabelTTF*)pinfolayer->getChildByTag( 1+i );
		pl->setString( it->getCaption().c_str() );
	}

	CCLabelTTF* pl = (CCLabelTTF*)pinfolayer->getChildByTag( 8 );
	char info[10];
	sprintf_s( info, "%d$", pplayer->getCash() );
	pl->setString( info );

	for( i=0; i<5; i++ ){
		CCLabelTTF* pl = (CCLabelTTF*)pinfolayer->getChildByTag( 9+i );
		sprintf_s( info, "%c*%d", COMPANYNAME[i][0], pplayer->stocks[i] );
		pl->setString( info );
	}

	pl = (CCLabelTTF*)this->getChildByTag( 0 );
}

bool PlayerLayer::init(){
    if ( !CCLayerColor::init() ){
        return false;
    }
    CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	setTouchEnabled(true);
	selected_company = WORLDWIDE;
	amount = 1;

    return true;
}

void PlayerLayer::setPlayerName( string name ){ 
	id = name; 
}

const PlaceATileOrder	PlayerLayer::decidePlaceATile( const GameStatus& bs ){
	UINT i = 0;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		if( i==selected_tile_index){
			PlaceATileOrder od( *it );
			return od;
		}
	}
	return DefaultAI::decidePlaceATile( bs );
}
const ConvertStockOrder PlayerLayer::decideDoStockConversion(const GameStatus& bs ){
	ConvertStockOrder od( selected_company , amount );
	return od;
}
const BuyStockOrder		PlayerLayer::decideBuyStocks( const GameStatus& bs){
	BuyStockOrder od( selected_company , amount );
	return od;
}
const SellStockOrder	PlayerLayer::decideSellStock( const GameStatus& bs){
	SellStockOrder od( selected_company , amount );
	return od;
}
const SetupCompanyOrder PlayerLayer::decideSetupCompany( const GameStatus& bs ){
	SetupCompanyOrder od(selected_company);
	return od;
}

void PlayerLayer::setGameStatus( GameStatus* gs ){
	pGame = gs;
}

void PlayerLayer::toggleDimmedBackGround( bool toggled ){
	poperlayer->setVisible( toggled );
	pinfolayer->setVisible( !toggled );
	poperlayer->setColor( toggled ? ccBLUE:ccBLACK );
	poperlayer->setOpacity( toggled ? 50: 0 );
	pHintLabel->setVisible(toggled);
}

CCMenu* PlayerLayer::createTileOPMenu(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	CCMenu* pMenu = CCMenu::create();
	CCLabelTTF* pl = createTextureLabel(ccp(0,BLOCK_SIZE/3),"avtar",FONT_SIZE,"OKAY");
	CCMenuItemLabel* pMenuOK = CCMenuItemLabel::create( pl);
	pMenuOK->setPosition( BLOCK_SIZE*9.5f, 1*BLOCK_SIZE );
	pMenu->setPosition( op);
	pMenu->addChild( pMenuOK, 0, 111 );
	for( int i=0; i<NUMBER_OF_ATileS_EACH; i++ ){
		CCMenuItemSprite* mi;
		CCSprite* pbutton  = CCSprite::createWithSpriteFrameName( "tile" );
		CCSprite* pbuttons  = CCSprite::createWithSpriteFrameName( "tile" );
		pbuttons->setScale(1.2f );
		mi = CCMenuItemSprite::create( pbutton,pbuttons );
		pl  = CCLabelTTF::create( "", "Arial", FONT_SIZE*1.5, 
			lblsize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
		mi->addChild( pl, 1, 0 );
		mi->setPosition(  ccp((i+2)*BLOCK_SIZE, BLOCK_SIZE*0.6f) );
		pl->setPosition(  ccp(BLOCK_SIZE*0.5f, BLOCK_SIZE*0.25f) );
		pMenu->addChild(mi,0,i);
	}
	return pMenu;
}
CCMenu* PlayerLayer::createCompOPMenu(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + sz.height - BLOCK_SIZE * HEIGH - BLOCK_SIZE/4 ;
	CCMenuItemSprite* mi;
	CCMenu* pMenu = CCMenu::create();
	CCLabelTTF* pl = createTextureLabel(ccp(0,BLOCK_SIZE/3),"avtar",FONT_SIZE,"OKAY");
	CCMenuItemLabel* pMenuOK = CCMenuItemLabel::create( pl);
	pMenuOK->setPosition( BLOCK_SIZE*9.5f, 1*BLOCK_SIZE );
	pMenu->setPosition( ccp(0,0) );
	pMenu->addChild( pMenuOK, 0, 111 );
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		sprintf_s( info, "company%d",i );
		CCSprite* pbutton  = CCSprite::createWithSpriteFrameName( info );
		CCSprite* pbuttons  = CCSprite::createWithSpriteFrameName( info );
		pbuttons->setScale(1.2f );
		mi = CCMenuItemSprite::create( pbutton,pbuttons );
		pl  = CCLabelTTF::create( COMPANYNAME[i], "Arial", FONT_SIZE*1.5, 
			lblsize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		mi->addChild( pl, 1, 0 );
		mi->setPosition(  ccp(BLOCK_SIZE*11,(3+i)*BLOCK_SIZE) );
		pl->setPosition(  ccp(BLOCK_SIZE,BLOCK_SIZE/2) );
		pMenu->addChild(mi,0,i);
	}
	return pMenu;
}

void PlayerLayer::askPlayerToPlaceTile(){
	toggleDimmedBackGround(true);
	pHintLabel->setString("Please select a Tile");
	pTileMenu->setVisible(true);
	UINT i = 0;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		CCMenuItemSprite* mi = (CCMenuItemSprite*)pTileMenu->getChildByTag(i);
		mi->setTarget(this, menu_selector(PlayerLayer::onPlayerPlaceATile) );
		CCLabelTTF* pl = (CCLabelTTF*)mi->getChildByTag(0);
		pl->setString( it->getCaption().c_str() );
	}
	CCMenuItemLabel* pMenuOK = (CCMenuItemLabel*)pTileMenu->getChildByTag(111);
	pMenuOK->setTarget(this,menu_selector(PlayerLayer::onPlayerTilePlaced));
	ready = false;

}
void PlayerLayer::onPlayerPlaceATile(cocos2d::CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	selected_tile_index = mi->getTag();
	sprintf_s( info, "No.%d ATILE selected", selected_tile_index );
	pHintLabel->setString(info);

}
void PlayerLayer::onPlayerTilePlaced(cocos2d::CCObject *pSender){
	pTileMenu->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToBuyStock(){
	toggleDimmedBackGround(true);
	pHintLabel->setString("Please select stock to buy:");
	pCompMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		CCMenuItemSprite* mi = (CCMenuItemSprite*)pCompMenu->getChildByTag(i);
		mi->setTarget(this, menu_selector(PlayerLayer::onPlayerBuyStock) );
	}
	CCMenuItemLabel* pMenuOK = (CCMenuItemLabel*)pCompMenu->getChildByTag(111);
	pMenuOK->setTarget(this,menu_selector(PlayerLayer::onPlayerStockBought));
	ready = false;

}
void PlayerLayer::onPlayerBuyStock(cocos2d::CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	COMPANY newselection = (COMPANY)mi->getTag();
	if( selected_company == newselection ){
		amount ++ ;
	}
	else{
		selected_company = newselection;
		amount = 1;
	}
	sprintf_s( info, "Buy [%s]*%d", COMPANYNAME[selected_company], amount );
	pHintLabel->setString(info);

}
void PlayerLayer::onPlayerStockBought(cocos2d::CCObject *pSender){
	pCompMenu->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToSetupCompany(){
	toggleDimmedBackGround(true);
	pHintLabel->setString("Please select a company to setup:");
	pCompMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		CCMenuItemSprite* mi = (CCMenuItemSprite*)pCompMenu->getChildByTag(i);
		mi->setTarget(this, menu_selector(PlayerLayer::onPlayerSetupCompany) );
	}
	CCMenuItemLabel* pMenuOK = (CCMenuItemLabel*)pCompMenu->getChildByTag(111);
	pMenuOK->setTarget(this,menu_selector(PlayerLayer::onPlayerCompanySetup));
	ready = false;
}
void PlayerLayer::onPlayerSetupCompany(cocos2d::CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	selected_company = (COMPANY)mi->getTag();
	sprintf_s( info, "setup [%s]", COMPANYNAME[selected_company] );
	pHintLabel->setString(info);

}
void PlayerLayer::onPlayerCompanySetup(cocos2d::CCObject *pSender){
	pCompMenu->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToSellStock(){
	toggleDimmedBackGround(true);
	pHintLabel->setString("Please select stock to sell:");
	pCompMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		CCMenuItemSprite* mi = (CCMenuItemSprite*)pCompMenu->getChildByTag(i);
		mi->setTarget(this, menu_selector(PlayerLayer::onPlayerSellStock) );
	}
	CCMenuItemLabel* pMenuOK = (CCMenuItemLabel*)pCompMenu->getChildByTag(111);
	pMenuOK->setTarget(this,menu_selector(PlayerLayer::onPlayerStockSold));
	ready = false;

}
void PlayerLayer::onPlayerSellStock(cocos2d::CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	COMPANY newselection = (COMPANY)mi->getTag();
	if( selected_company == newselection ){
		amount ++ ;
	}
	else{
		selected_company = newselection;
		amount = 1;
	}
	sprintf_s( info, "Sell [%s]*%d", COMPANYNAME[selected_company], amount );
	pHintLabel->setString(info);

}
void PlayerLayer::onPlayerStockSold(cocos2d::CCObject *pSender){
	pCompMenu->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToConvertStock(){
	toggleDimmedBackGround(true);
	pHintLabel->setString("Please select stock to convert:");
	pCompMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		CCMenuItemSprite* mi = (CCMenuItemSprite*)pCompMenu->getChildByTag(i);
		mi->setTarget(this, menu_selector(PlayerLayer::onPlayerConvertStock) );
	}
	CCMenuItemLabel* pMenuOK = (CCMenuItemLabel*)pCompMenu->getChildByTag(111);
	pMenuOK->setTarget(this,menu_selector(PlayerLayer::onPlayerStockConverted));
	ready = false;

}
void PlayerLayer::onPlayerConvertStock(cocos2d::CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	COMPANY newselection = (COMPANY)mi->getTag();
	if( selected_company == newselection ){
		amount ++ ;
	}
	else{
		selected_company = newselection;
		amount = 1;
	}
	sprintf_s( info, "[%s]*%d", COMPANYNAME[selected_company], amount );
	pHintLabel->setString(info);

}
void PlayerLayer::onPlayerStockConverted(cocos2d::CCObject *pSender){
	pCompMenu->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}


////////////////////////////////////////////////////////////////////////
bool AcquireGameScene::init()
{
	if( CCScene::init() )
	{
		
		return true;
	}
	else
	{
		return false;
	}
}
void AcquireGameScene::initGameWithSelection( AcquireSelectScene*  sscene ){
	//KNOWN MEMORY LEAKS HERE
		pGame = new Game;
		pAcquireLayer = AcquireScene::create();
		pAcquireLayer->retain();
		addChild( pAcquireLayer,10 );
		pAcquireLayer->setGameStatus( &pGame->gs );

		if( sscene->mode == AcquireSelectScene::AIVSAI ){
			pPlayerLayer = AutomaticPlayerLayer::create();
			pPlayerLayer->setPlayerName("AI1");
			pPlayerLayer->retain();
			addChild( pPlayerLayer,1 );
			pPlayerLayer->setGameStatus( &pGame->gs );
			Player* pa = new Player(pPlayerLayer);
			pGame->addPlayer( pa );

			DefaultAI* pai2 = new DefaultAI("AI2");
			pPlayerLayer->setPlayer( pa );
			Player* pb = new Player(pai2);
			pai2->setPlayer( pb );
			pGame->addPlayer( pb );
		}
		else if( sscene->mode = AcquireSelectScene::HUVSAI ){
			pPlayerLayer = PlayerLayer::create();
			pPlayerLayer->setPlayerName("Player");
			pPlayerLayer->retain();
			addChild( pPlayerLayer,1 );
			pPlayerLayer->setGameStatus( &pGame->gs );
			Player* pa = new Player(pPlayerLayer);
			pGame->addPlayer( pa );

			for( int i=1; i<=sscene->ai_num; i++ ){
				sprintf_s( info, "AI%d", i );
				//DefaultAI* pai2 = new DefaultAI(info);
				PlayerAI* pai2 = AIFactory::getAI( sscene->seleted_ais[i], info );
				pPlayerLayer->setPlayer( pa );
				Player* pb = new Player(pai2);
				pai2->setPlayer( pb );
				pGame->addPlayer( pb );
			}
		}
		
		pGame->initPlayerWithATiles();

		pAcquireLayer->initGameUI();
		pPlayerLayer->initPlayerUI();

		pAcquireLayer->updateGameRender();
		//pPlayerLayer->updatePlayerRender();
		
		schedule( schedule_selector( AcquireGameScene::updateGame ), 1.0f	);
}
void AcquireGameScene::updateGame( float dt){
	if( !pGame->isEndOfGame()  ){
		pGame->runTheGameOneLoop();
		pAcquireLayer->updateGameRender();
		pPlayerLayer->updateLayerInfo();
	}
	else{
		switchToEndScene(this);
	}
}
AcquireGameScene::~AcquireGameScene()
{
	if (pAcquireLayer)
	{
		pAcquireLayer->release();
		pAcquireLayer = NULL;
	}

	if (pAcquireLayer)
	{
		pAcquireLayer->release();
		pAcquireLayer = NULL;
	}

	delete pGame;
}

CCLayerColor* AcquireGameScene::create_bg_layer(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLayerColor* bglayer = CCLayerColor::create();
	CCSprite* bgsprite = CCSprite::createWithSpriteFrameName("menuhead");
	bgsprite->setPosition(ccp(sz.width / 2 + op.x , sz.height * 0.75f + op.y ));
	bglayer->addChild(bgsprite);
	return bglayer;
}

CCLayerColor* AcquireGameScene::create_an_layer(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLayerColor* anlayer = CCLayerColor::create();
	CCTexture2D* pdm  =  CCTextureCache::sharedTextureCache()->textureForKey("demor.png");
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("demo.plist",pdm);
	CCSprite* anisprite = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("frame1"));
	anisprite->setPosition(ccp(sz.width *0.75 + op.x , sz.height * 0.25f  + op.y ));

	CCArray* animFrames = CCArray::createWithCapacity(8);

	char str[100] = {0};
	for(int i = 1; i <= 8; ++i)
	{
		sprintf(str, "frame%d", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );
		animFrames->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames,2.0f);
	animation->setLoops(1000);
	
	anlayer->addChild( anisprite,1 );
	anisprite->runAction(CCAnimate::create(animation) );

	return anlayer;
}
CCScene* AcquireGameScene::create_gameend_scene()
{
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	CCScene *scene = CCScene::create();
	CCLayerColor* player = CCLayerColor::create();
	CCSprite* pLogo = CCSprite::createWithSpriteFrameName( "roundend" );
	pLogo->setPosition(ccp(sz.width / 2 + op.x , sz.height *0.75f + op.y ));
	player->addChild( pLogo,0 );
	scene->addChild( player );

	CCMenu* pbtn = CCMenu::create();
	pbtn->setPosition( ccp( BLOCK_SIZE*9,BLOCK_SIZE*1 ));
	player->addChild( pbtn,1 );

	CCLabelTTF* pl = CCLabelTTF::create( "Play Again", "Arial", FONT_SIZE*1.5, 
		CCSize( BLOCK_SIZE*4,BLOCK_SIZE*1.5), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
	CCSprite* pbutton;
	CCSprite* pbuttons;
	pbutton  = CCSprite::createWithSpriteFrameName( "blank" );
	pbuttons  = CCSprite::createWithSpriteFrameName( "blanks" );
	CCMenuItemSprite* pii = CCMenuItemSprite::create( pbutton,pbuttons,player,menu_selector(AcquireGameScene::switchToMainScene) );
	pii->addChild( pl, 1, 0 );
	pl->setPosition( ccp(BLOCK_SIZE,BLOCK_SIZE*0.5));
	pbtn->addChild( pii,0,12 );

    return scene;
}
CCLayerColor* AcquireGameScene::create_static_layer(){
	CCLayerColor* dashboard_layer = CCLayerColor::create();
	string winner="NONE";
	int max_position = 0;
	CCLabelTTF* pl ;
	for( UINT i=0; i<pGame->players.size(); i++ ){
		Player* p = pGame->players[i];
		int pos = p->getTotalBalance( & pGame->gs );
		if( pos > max_position ) winner = p->getID();
		sprintf_s(info,"%s TOTAL:%10d", p->toString().c_str(), pos );
		pl = CCLabelTTF::create( info, "Arial", FONT_SIZE*1, 
			CCSize( BLOCK_SIZE*8,BLOCK_SIZE), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		pl->setPosition( ccp(BLOCK_SIZE*4,BLOCK_SIZE*(1+i*0.5)));
		dashboard_layer->addChild( pl,i );
	}

	pl = CCLabelTTF::create( info, "Arial", FONT_SIZE*1.5, 
			CCSize( BLOCK_SIZE*8,BLOCK_SIZE), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
	pl->setPosition( ccp(BLOCK_SIZE*4,BLOCK_SIZE*(1+2*0.5)));
	sprintf_s(info, "WINNER %s", winner.c_str() );
	pl->setString( info );
	dashboard_layer->addChild( pl,11 );

	CCSprite* sp_rabbit  = CCSprite::createWithSpriteFrameName( "rabbit" );
	sp_rabbit->setPosition( ccp( BLOCK_SIZE*2, BLOCK_SIZE*2.5 ) );
	sp_rabbit->setScale(0.75f);
	dashboard_layer->addChild( sp_rabbit );

	CCSprite* spstockfrmae  = CCSprite::createWithSpriteFrameName( "stockframe" );
	dashboard_layer->addChild( spstockfrmae );
	spstockfrmae->setPosition(ccp(BLOCK_SIZE*4,BLOCK_SIZE*4));
	pl  = CCLabelTTF::create( "", "Arial", FONT_SIZE*1, 
			CCSize( BLOCK_SIZE*6,BLOCK_SIZE*6), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
	pl->setPosition( ccp(BLOCK_SIZE*4.75,BLOCK_SIZE*3.5) );
	pl->setColor(ccBLUE);
	pl->setString( AcquireScene::getGameStatusInfo( &(pGame->gs) ).c_str() );
	dashboard_layer->addChild( pl, 0, 111 );
	return dashboard_layer;
}

void AcquireGameScene::switchToMainScene(CCObject *pSender){
	CCScene *s = AcquireSelectScene::create();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionPageTurn::create(1.0f, s, false);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void AcquireGameScene::switchToEndScene(CCObject *pSender){
	CCScene *s = create_gameend_scene();
	s->addChild( this->create_static_layer() );
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionPageTurn::create(1.0f, s, false);
	CCDirector::sharedDirector()->replaceScene(transition);
}



void AcquireGameScene::loadTexture(){
	CCTextureCache::sharedTextureCache()->addImage("rr2.png");
	CCTextureCache::sharedTextureCache()->addImage("rr3.png");
	CCTextureCache::sharedTextureCache()->addImage("rr.png");
	CCTextureCache::sharedTextureCache()->addImage("demor.png");
	CCTexture2D* pdm  =  CCTextureCache::sharedTextureCache()->textureForKey("rr.png");
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("ui.plist",pdm);
	pdm  =  CCTextureCache::sharedTextureCache()->textureForKey("rr3.png");
	cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("ui2.plist",pdm);
	
}
CCScene* AcquireGameScene::create_splash_scene()
{
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
    CCScene *scene = CCScene::create();
	scene->scheduleOnce( schedule_selector( AcquireGameScene::switchToMainScene ), 4.0f	);
	CCLayerColor* player = CCLayerColor::create();
	loadTexture();
	CCTexture2D* psplash_texture = CCTextureCache::sharedTextureCache()->textureForKey("rr2.png");
	CCSprite* pLogo = CCSprite::createWithTexture(psplash_texture);
	
	pLogo->setPosition(ccp(sz.width / 2 + op.x , sz.height / 2 + op.y ));

	player->addChild( pLogo,0 );
	scene->addChild( player );
    return scene;
}
////////////////////////////////////////////////////////////////////////
bool AcquireSelectScene::init()
{
	if( CCScene::init() )
	{
		ai_num = 1;
		mode = HUVSAI;
		pAISelectionLayer = createAISelectionLayer();
		pMISelectionLayer = createMISelectionLayer();
		addChild( AcquireGameScene::create_bg_layer() );
		addChild( AcquireGameScene::create_an_layer() );

		addChild(pAISelectionLayer);
		addChild(pMISelectionLayer);

		updateAISelectionsUI();
		return true;
	}
	else
	{
		return false;
	}
}

extern char ainame[3][10];

CCLayerColor* AcquireSelectScene::createMISelectionLayer(){
	CCLayerColor* selecton_layer = CCLayerColor::create();
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	p_aimainmenu = CCMenu::create();
	selecton_layer->addChild(p_aimainmenu, 0);
	p_aimainmenu->setPosition(CCPointZero);
	CCSprite* sp_vsai;
	CCSprite* sp_vshu;
	CCSprite* sp_exit;
	sp_vsai  = CCSprite::createWithSpriteFrameName( "vsai" );
	sp_vshu  = CCSprite::createWithSpriteFrameName( "vshu" );
	sp_exit  = CCSprite::createWithSpriteFrameName( "exit" );
	CCMenuItemSprite *pci;
	pci = CCMenuItemSprite::create( sp_vshu,sp_vshu,this,menu_selector(AcquireSelectScene::onHUVSAISelected));
	pci->setPosition( sz.width * 0.25f + op.x, sz.height*0.25f + op.y );
	p_aimainmenu->addChild( pci );
   
	pci = CCMenuItemSprite::create(sp_vsai,sp_vsai,this,menu_selector(AcquireSelectScene::onAIVSAISelected));
	pci->setPosition( sz.width * 0.55f + op.x, sz.height*0.25f + op.y );
	p_aimainmenu->addChild( pci );

	pci = CCMenuItemSprite::create(sp_exit,sp_exit,this,menu_selector(AcquireSelectScene::onExitGMSelected));
	pci->setPosition( sz.width * 0.55f + op.x, BLOCK_SIZE+op.y );
	p_aimainmenu->addChild( pci );

	selecton_layer->setTouchEnabled(true);
	return selecton_layer;

}
CCLayerColor* AcquireSelectScene::createAISelectionLayer(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	CCLayerColor* selecton_layer = CCLayerColor::create();
	selecton_layer->setVisible(false);
	p_aimainmenu = CCMenu::create();
	selecton_layer->addChild(p_aimainmenu, 0);
	p_aimainmenu->setPosition(ccp(BLOCK_SIZE*2+op.x, op.y ));
	for( UINT i=0; i<MAXAI; i++ ){
		seleted_ais[i] = NAIVE_AI;
	}
	CCSprite* pbutton;
	CCSprite* pbuttons;
	CCLabelTTF* pl;
	for( UINT i=0; i<MAXAI; i++ ){
		AITYPE t = seleted_ais[i];
		pl  = CCLabelTTF::create( ainame[t], "Arial", FONT_SIZE*1.5, 
			lblsize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
		pbutton  = CCSprite::createWithSpriteFrameName( "blank" );
		pbuttons  = CCSprite::createWithSpriteFrameName( "blanks" );
		paitypelabel[i] = CCMenuItemSprite::create( pbutton,pbuttons,this,menu_selector(AcquireSelectScene::onAITYPEChanged) );
		pl->setPosition( ccp(lblsize.width/4, lblsize.height/4 ));
		paitypelabel[i]->addChild( pl, 1, 0 );
		paitypelabel[i]->setPosition(ccp(BLOCK_SIZE*3,BLOCK_SIZE*(4-i*1)));
		p_aimainmenu->addChild( paitypelabel[i],0,i );
	}

	pl = CCLabelTTF::create( "", "Arial", FONT_SIZE*1.5, 
		lblsize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
		pbutton  = CCSprite::createWithSpriteFrameName( "blank" );
		pbuttons  = CCSprite::createWithSpriteFrameName( "blanks" );
	painumlabel = CCMenuItemSprite::create( pbutton,pbuttons,this,menu_selector(AcquireSelectScene::onAICountChanged) );
	painumlabel->setPosition(ccp(BLOCK_SIZE*6,BLOCK_SIZE*4));
	painumlabel->addChild( pl, 1, 0 );
	pl->setPosition( ccp(lblsize.width/4, lblsize.height/4 ));
	p_aimainmenu->addChild( painumlabel,0,11 );

	pl = CCLabelTTF::create( "OKAY", "Arial", FONT_SIZE*1.5, 
		lblsize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
		pbutton  = CCSprite::createWithSpriteFrameName( "blank" );
		pbuttons  = CCSprite::createWithSpriteFrameName( "blanks" );
	CCMenuItemSprite* pii = CCMenuItemSprite::create( pbutton,pbuttons,this,menu_selector(AcquireSelectScene::switchToGameScene) );
	pii->setPosition(ccp(BLOCK_SIZE*6,BLOCK_SIZE*3));
	pii->addChild( pl, 1, 0 );
	pl->setPosition( ccp(lblsize.width/4, lblsize.height/4 ));
	p_aimainmenu->addChild( pii,0,12 );

	selecton_layer->setTouchEnabled(true);
	return selecton_layer;
}
void AcquireSelectScene::onAIVSAISelected(CCObject *pSender){
	mode = AIVSAI;
	switchToGameScene(this);
}
void AcquireSelectScene::onHUVSAISelected(CCObject *pSender){
	pMISelectionLayer->setVisible(false);
	pAISelectionLayer->setVisible(true);
	mode = HUVSAI;
}
void AcquireSelectScene::onExitGMSelected(CCObject *pSender){
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
void AcquireSelectScene::onAICountChanged(CCObject *pSender){
	ai_num = (ai_num + 1) % (MAXAI+1) ;
	if( 0 == ai_num ) ai_num = 1;
	updateAISelectionsUI();
}
void AcquireSelectScene::onAITYPEChanged(CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	int index = mi->getTag();
	seleted_ais[index] =(AITYPE)( ( seleted_ais[index] + 1 ) % 3 );
	updateAISelectionsUI();
}
void AcquireSelectScene::updateAISelectionsUI( ){
	sprintf_s( info, "FIGHT %d", ai_num );
	CCLabelTTF* pl = (CCLabelTTF*)(painumlabel->getChildByTag(0));
	pl->setString( info );
	for( int i=0; i<MAXAI; i++ ){
		CCLabelTTF* pl = (CCLabelTTF*)(paitypelabel[i]->getChildByTag(0));
		pl->setString(ainame[seleted_ais[i]] );
		paitypelabel[i]->setVisible( i<ai_num );
	}

}

void AcquireSelectScene::switchToGameScene(CCObject *pSender){
	AcquireGameScene *s = AcquireGameScene::create();
	s->initGameWithSelection( this );
	//CCScene* ss = s->create_gameend_scene();
	//CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionMoveInT  *transition = CCTransitionMoveInT::create(1.0f, s);
	CCDirector::sharedDirector()->replaceScene(transition);
}
