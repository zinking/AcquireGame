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
ccColor4B COMPANYCOLOR4[MAXNUMBER_OF_STOCKS]={ 
	ccc4(255,0,0,100),   ccc4(0,255,0,100),   ccc4(0,0,255,100),
	ccc4(255,255,0,100), ccc4(255,0,255,100), ccc4(0,255,255,100),
	ccc4(255,128,255,100)
};

void AcquireScene::setGameStatus( GameStatus* gs ){
	pGame = gs;
}
CCScene* AcquireScene::scene()
{
    CCScene *scene = CCScene::create();
    return scene;
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

    CCMenuItemImage *pci = CCMenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        this,
        menu_selector(AcquireScene::menuClickCallBack)
	);
    
	pci->setPosition(ccp(op.x + sz.width - pci->getContentSize().width/2 ,op.y + pci->getContentSize().height/2));
    CCMenu* pMenu = CCMenu::create(pci, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	this->setTouchEnabled(true);
    
    return true;
}

void AcquireScene::menuClickCallBack(CCObject* pSender)
{
	showStatusPopup();

}

void AcquireScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}

void AcquireScene::showStatusPopup()
{
	Popup *popup = Popup::node();
	this->addChild(popup);
	this->setTouchEnabled(false);
}

//////////////////////////////////////////////////////////////////////
CCLabelTTF* PlayerLayer::createImageLabel( CCPoint& ipos, CCRect& rect,  int fontsize , string text){
	CCLabelTTF* pl = CCLabelTTF::create( text.c_str(), "Arial", fontsize, 
		CCSize( BLOCK_SIZE,BLOCK_SIZE*2), kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom); 
	CCSprite* avtar  = CCSprite::createWithTexture(pTexture,rect);
	avtar->setPosition(ipos);
	avtar->setAnchorPoint(ccp(0,0));
	pl->addChild(avtar);
	return pl;
}

void PlayerLayer::initPlayerUI()
{
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + sz.height - BLOCK_SIZE * HEIGH - BLOCK_SIZE/4 ;
	pTexture = CCTextureCache::sharedTextureCache()->addImage("r.png");
	CCLabelTTF* pl = createImageLabel(ccp(0,BLOCK_SIZE/3),CCRectMake(0,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE,"zhenw");
	pl->setPosition(  ccp(BLOCK_SIZE/2,BLOCK_SIZE) );
	this->addChild( pl, 0, 0 );
	for( UINT i=0; i<NUMBER_OF_ATileS_EACH; i++ ){
		pl = createImageLabel(ccp(0,-FONT_SIZE/2),CCRectMake(BLOCK_SIZE,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE*1.2);
		pl->setPosition(  ccp(BLOCK_SIZE*(2+i),BLOCK_SIZE*1.25) );
		this->addChild( pl, 0, 1+i );
	}

	pl = createImageLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),CCRectMake(BLOCK_SIZE*2,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE*1.2);
	pl->setPosition(  ccp(BLOCK_SIZE*9.5,BLOCK_SIZE*1.25) );
	this->addChild( pl, 0, 8 );

	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		pl = createImageLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),CCRectMake(0,BLOCK_SIZE*(1+i),BLOCK_SIZE*2,BLOCK_SIZE),FONT_SIZE*1.5);
		pl->setPosition(  ccp(BLOCK_SIZE*11.5f,dy+(1+i)*BLOCK_SIZE) );
		this->addChild( pl, 0, 9+i );
	}

	dx = op.x + BLOCK_SIZE;
	dy = op.y + sz.height-BLOCK_SIZE*2  ;

	pl = CCLabelTTF::create( "", "Arial", FONT_SIZE*1.5, 
		CCSize( BLOCK_SIZE*WIDTH,BLOCK_SIZE), kCCTextAlignmentLeft,kCCVerticalTextAlignmentBottom); 
	pl->setPosition(  ccp(dx,dy) );
	pl->setAnchorPoint(ccp(0,0));
	
	pl->setVisible( false );
	this->addChild( pl, 1, 111 );
	pHintLabel = pl;

	updatePlayerRender();
}

void PlayerLayer::createMenuContent(){
	pMenu = CCMenu::create();
	CCLabelTTF* pl = createImageLabel(ccp(0,BLOCK_SIZE/3),CCRectMake(0,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE,"OKAY");
	pMenuOK = CCMenuItemLabel::create( pl);
	pMenuOK->setPosition( BLOCK_SIZE*7, -2*BLOCK_SIZE );
	pMenu->setPosition( ccp(BLOCK_SIZE*2,BLOCK_SIZE*4) );
	pMenu->setVisible(true);
	pMenu->addChild( pMenuOK, 0, 111 );
	addChild( pMenu );
}

void PlayerLayer::clearMenuContent(){
	pMenu->removeFromParentAndCleanup(true );
}


void PlayerLayer::updatePlayerRender(){
	UINT i = 0;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		CCLabelTTF* pl = (CCLabelTTF*)this->getChildByTag( 1+i );
		pl->setString( it->getCaption().c_str() );
	}

	CCLabelTTF* pl = (CCLabelTTF*)this->getChildByTag( 8 );
	char info[10];
	sprintf_s( info, "%d$", pplayer->getCash() );
	pl->setString( info );

	for( i=0; i<5; i++ ){
		CCLabelTTF* pl = (CCLabelTTF*)this->getChildByTag( 9+i );
		sprintf_s( info, "%c*%d", COMPANYNAME[i][0], pplayer->stocks[i] );
		pl->setString( info );
	}

	pl = (CCLabelTTF*)this->getChildByTag( 0 );
	//pl->setVisible( pGame->isStupidHumanSTurn() );//TBD
	
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

const PlaceATileOrder PlayerLayer::decidePlaceATile( const GameStatus& bs ){
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
const BuyStockOrder PlayerLayer::decideBuyStocks( const GameStatus& bs){
	BuyStockOrder od( selected_company , amount );
	return od;
}
const SellStockOrder PlayerLayer::decideSellStock( const GameStatus& bs){
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
	this->setColor( toggled ? ccBLUE:ccBLACK );
	this->setOpacity( toggled ? 175: 0 );
}

void PlayerLayer::askPlayerToPlaceTile(){
	toggleDimmedBackGround(true);
	pHintLabel->setVisible(true);
	pHintLabel->setString("Please select a Tile");
	createMenuContent();
	UINT i = 0;
	CCLabelTTF* pl;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		pl = createImageLabel(ccp(0,-FONT_SIZE/2),CCRectMake(BLOCK_SIZE,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE*1.2, it->getCaption());
		CCMenuItemLabel* mi = CCMenuItemLabel::create(pl);
		mi->initWithTarget(this, menu_selector(PlayerLayer::onPlayerPlaceATile) );
		mi->setPosition( i*BLOCK_SIZE*1.5, BLOCK_SIZE );
		mi->setScale(1.5);
		pMenu->addChild(mi,0,i);
	}
	
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
	this->setTouchEnabled(true);
	clearMenuContent();
	pHintLabel->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToBuyStock(){
	toggleDimmedBackGround(true);
	pHintLabel->setVisible(true);
	pHintLabel->setString("Please select stock to buy:");
	createMenuContent();
	pMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	CCLabelTTF* pl;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		pl = createImageLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),CCRectMake(0,BLOCK_SIZE*(1+i),BLOCK_SIZE*2,BLOCK_SIZE),FONT_SIZE*1.5, COMPANYNAME[i]);
		//pl->setPosition(  ccp(BLOCK_SIZE*11.5f,(1+i)*BLOCK_SIZE) );
		CCMenuItemLabel* mi = CCMenuItemLabel::create(pl);
		mi->initWithTarget(this, menu_selector(PlayerLayer::onPlayerBuyStock) );
		mi->setPosition( 2*i*BLOCK_SIZE, BLOCK_SIZE );
		pMenu->addChild(mi,0,i);
	}
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
	this->setTouchEnabled(true);
	clearMenuContent();
	pHintLabel->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToSetupCompany(){
	toggleDimmedBackGround(true);
	pHintLabel->setVisible(true);
	pHintLabel->setString("Please select a company to setup:");
	createMenuContent();
	pMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	CCLabelTTF* pl;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		pl = createImageLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),CCRectMake(0,BLOCK_SIZE*(1+i),BLOCK_SIZE*2,BLOCK_SIZE),FONT_SIZE*1.5, COMPANYNAME[i]);
		//pl->setPosition(  ccp(BLOCK_SIZE*11.5f,(1+i)*BLOCK_SIZE) );
		CCMenuItemLabel* mi = CCMenuItemLabel::create(pl);
		mi->initWithTarget(this, menu_selector(PlayerLayer::onPlayerSetupCompany) );
		mi->setPosition( 2*i*BLOCK_SIZE, BLOCK_SIZE );
		pMenu->addChild(mi,0,i);
	}
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
	this->setTouchEnabled(true);
	clearMenuContent();
	pHintLabel->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToSellStock(){
	toggleDimmedBackGround(true);
	pHintLabel->setVisible(true);
	pHintLabel->setString("Please select stock to sell:");
	createMenuContent();
	pMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	CCLabelTTF* pl;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		pl = createImageLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),CCRectMake(0,BLOCK_SIZE*(1+i),BLOCK_SIZE*2,BLOCK_SIZE),FONT_SIZE*1.5, COMPANYNAME[i]);
		//pl->setPosition(  ccp(BLOCK_SIZE*11.5f,(1+i)*BLOCK_SIZE) );
		CCMenuItemLabel* mi = CCMenuItemLabel::create(pl);
		mi->initWithTarget(this, menu_selector(PlayerLayer::onPlayerSellStock) );
		mi->setPosition( 2*i*BLOCK_SIZE, BLOCK_SIZE );
		pMenu->addChild(mi,0,i);
	}
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
	this->setTouchEnabled(true);
	clearMenuContent();
	pHintLabel->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}

void PlayerLayer::askPlayerToConvertStock(){
	toggleDimmedBackGround(true);
	pHintLabel->setVisible(true);
	pHintLabel->setString("Please select stock to convert:");
	createMenuContent();
	pMenu->setVisible(true);
	selected_company = WORLDWIDE;
	amount = 1;
	UINT i = 0;
	CCLabelTTF* pl;
	for( UINT i=0; i<NUMBER_OF_STOCKS; i++ ){
		pl = createImageLabel(ccp(-BLOCK_SIZE,-BLOCK_SIZE/4),CCRectMake(0,BLOCK_SIZE*(1+i),BLOCK_SIZE*2,BLOCK_SIZE),FONT_SIZE*1.5, COMPANYNAME[i]);
		//pl->setPosition(  ccp(BLOCK_SIZE*11.5f,(1+i)*BLOCK_SIZE) );
		CCMenuItemLabel* mi = CCMenuItemLabel::create(pl);
		mi->initWithTarget(this, menu_selector(PlayerLayer::onPlayerConvertStock) );
		mi->setPosition( 2*i*BLOCK_SIZE, BLOCK_SIZE );
		pMenu->addChild(mi,0,i);
	}
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
	this->setTouchEnabled(true);
	clearMenuContent();
	pHintLabel->setVisible(false);
	toggleDimmedBackGround(false);
	ready = true;
}


////////////////////////////////////////////////////////////////////////

bool AcquireGameScene::init()
{
	if( CCScene::init() )
	{
		pAcquireLayer = AcquireScene::create();
		pAcquireLayer->retain();
		addChild( pAcquireLayer,0 );

		pPlayerLayer = AutomaticPlayerLayer::create();
		pPlayerLayer->setPlayerName("zhenw");
		pPlayerLayer->retain();
		addChild( pPlayerLayer,1 );

		pGame = new Game;
		pAcquireLayer->setGameStatus( &pGame->gs );
		pPlayerLayer->setGameStatus( &pGame->gs );

		DefaultAI* pai2 = new DefaultAI("N2");

		Player* pa = new Player(pPlayerLayer);
		pPlayerLayer->setPlayer( pa );
		Player* pb = new Player(pai2);
		pai2->setPlayer( pb );


		pGame->addPlayer( pa );
		pGame->addPlayer( pb );

		pGame->initPlayerWithATiles();

		pAcquireLayer->initGameUI();
		pPlayerLayer->initPlayerUI();

		pAcquireLayer->updateGameRender();
		pPlayerLayer->updatePlayerRender();
		
		schedule( schedule_selector( AcquireGameScene::updateGame ), 1.0f	);
		//scheduleOnce( schedule_selector( AcquireGameScene::updateGame ), 2.0f	);
		return true;
	}
	else
	{
		return false;
	}
}

void AcquireGameScene::updateGame( float dt){
	if( !pGame->isEndOfGame()  ){
		//pGame->runTheGameOneRound();
		pGame->runTheGameOneLoop();
		pAcquireLayer->updateGameRender();
		pPlayerLayer->updatePlayerRender();
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
