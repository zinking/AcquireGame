#include "AcquireScene.h"


USING_NS_CC;
#include "Game/board.h"
#include "Game/definitions.h"
#include "Game/PlayerAI.h"
#include "Game/order.h"
#include "ATileSprite.h"

ccColor3B COMPANYCOLOR[NUMBER_OF_STOCKS]={ 
	ccc3(255,0,0),   ccc3(0,255,0),   ccc3(0,0,255),
	ccc3(255,255,0), ccc3(255,0,255), ccc3(0,255,255),
	ccc3(255,255,255)
};
ccColor4B COMPANYCOLOR4[NUMBER_OF_STOCKS]={ 
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
    //AcquireScene *layer = AcquireScene::create();

    //scene->addChild(layer);
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

	dx = op.x + BLOCK_SIZE*WIDTH/2;
	dy = op.y + sz.height  ;

	CCLabelTTF* pl = CCLabelTTF::create( "", "Arial", FONT_SIZE, 
		CCSize( BLOCK_SIZE*WIDTH,BLOCK_SIZE), kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom); 
	pl->setPosition(  ccp(dx,dy) );
	this->addChild( pl, 1, 11 );

	updateGameRender();
}


extern char COMPANYNAME[NUMBER_OF_STOCKS][20];

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
    
	//initGameLogic();
	
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
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);

	//initGameUI();
    
    return true;
}

void AcquireScene::menuClickCallBack(CCObject* pSender)
{
	showStatusPopup();
	/*
	if( !pGame->isEndOfGame() ){
		pGame->runTheGameOneRound();
		updateGameRender();
	}
	*/
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

void PlayerLayer::initPlayerUI()
{
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + sz.height - BLOCK_SIZE * HEIGH - BLOCK_SIZE/4 ;

	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("r.png");

	CCLabelTTF* pl = CCLabelTTF::create( pplayer->getID(), "Arial", FONT_SIZE, 
		CCSize( BLOCK_SIZE,BLOCK_SIZE*2), kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom); 
	CCSprite* avtar  = CCSprite::createWithTexture(pTexture,CCRectMake(0,0,BLOCK_SIZE,BLOCK_SIZE));
	avtar->setPosition(ccp(0,BLOCK_SIZE/3));
	avtar->setAnchorPoint(ccp(0,0));
	pl->addChild(avtar);
	pl->setPosition(  ccp(BLOCK_SIZE/2,BLOCK_SIZE) );
	this->addChild( pl, 0, 0 );

	for( UINT i=0; i<NUMBER_OF_ATileS_EACH; i++ ){
		pl = CCLabelTTF::create( "A1", "Arial", FONT_SIZE, 
			CCSize( BLOCK_SIZE,BLOCK_SIZE), kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom); 
		avtar  = CCSprite::createWithTexture(pTexture,CCRectMake(BLOCK_SIZE,0,BLOCK_SIZE,BLOCK_SIZE));
		avtar->setPosition(ccp(0,-FONT_SIZE/2));
		avtar->setAnchorPoint(ccp(0,0));
		pl->addChild(avtar);
		pl->setPosition(  ccp(BLOCK_SIZE*(2+i),BLOCK_SIZE/1.5) );
		this->addChild( pl, 0, 1+i );
	}

	pl = CCLabelTTF::create( "1500$", "Arial", FONT_SIZE*1.2, 
			CCSize( BLOCK_SIZE*2,BLOCK_SIZE), kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter); 
	avtar  = CCSprite::createWithTexture(pTexture,CCRectMake(BLOCK_SIZE*2,0,BLOCK_SIZE,BLOCK_SIZE));
	avtar->setPosition(ccp(0,0));
	avtar->setAnchorPoint(ccp(0,0));
	pl->addChild(avtar);
	pl->setPosition(  ccp(BLOCK_SIZE*9,BLOCK_SIZE/2) );
	this->addChild( pl, 0, 8 );

	for( UINT i=0; i<5; i++ ){
		pl = CCLabelTTF::create( "W:()", "Arial", FONT_SIZE*1.2, 
				CCSize( BLOCK_SIZE*2,BLOCK_SIZE), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter); 
		avtar  = CCSprite::createWithTexture(pTexture,CCRectMake(0,BLOCK_SIZE*(1+i),BLOCK_SIZE*2,BLOCK_SIZE));
		avtar->setPosition(ccp(0,0));
		avtar->setAnchorPoint(ccp(0,0));
		pl->addChild(avtar);
		pl->setPosition(  ccp(BLOCK_SIZE*11.25f,dy+i*BLOCK_SIZE) );
		this->addChild( pl, 0, 9+i );
	}

	updatePlayerRender();
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
	
}

bool PlayerLayer::init(){
    if ( !CCLayer::init() ){
        return false;
    }
    CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	this->setTouchEnabled(true);
	popup = Popup::node();
	this->addChild(popup);
	popup->setVisible(false );
	inoperation = false;
    return true;
}

void PlayerLayer::setPlayerName( string name ){ 
	id = name; 
}

void PlayerLayer::updatePlayerLogic(){
	if( inoperation ) return;
	GAMESTAGE stage = pGame->getGameState();
	if( stage == TO_PLACE_TILE ){
		askPlayerToPlaceTile();
		inoperation = true;
	}
	else if( stage == TO_BUY_STOCK ){
		pGame->updateGameStage();
	}
	else if( stage == TO_SETUP_COMPANY ){
		pGame->updateGameStage();
	}
	else if( stage == TO_SELL_STOCK ){
		pGame->updateGameStage();
	}
	else if ( stage == TO_CONVERT_STOCK ){
		pGame->updateGameStage();
	}
}

const PlaceATileOrder PlayerLayer::decidePlaceATile( const GameStatus& bs ){
	//Popup *popup = Popup::node();
	//this->addChild(popup);
	
	//this->setTouchEnabled(false);
	//while( true ) Sleep( 5 );
	//const ATile& t = *( pplayer->ATiles.begin() );
	//PlaceATileOrder od( t );
	//return od;
	return DefaultAI::decidePlaceATile( bs );
}

void PlayerLayer::setGameStatus( GameStatus* gs ){
	pGame = gs;
}

void PlayerLayer::askPlayerToPlaceTile(){
	popup->setVisible(true);
	this->setTouchEnabled(false);
	popup->okMenuItem->setTarget( this, menu_selector(PlayerLayer::onPlayerPlacedTile));
}

void PlayerLayer::onPlayerPlacedTile(cocos2d::CCObject *pSender){
	pGame->updateGameStage();
	this->setTouchEnabled(true);
	popup->setVisible(false);
	inoperation = false;
}


////////////////////////////////////////////////////////////////////////

bool AcquireGameScene::init()
{
	if( CCScene::init() )
	{
		pAcquireLayer = AcquireScene::create();
		pAcquireLayer->retain();
		addChild( pAcquireLayer,0 );

		pPlayerLayer = PlayerLayer::create();
		pPlayerLayer->setPlayerName("zhenw");
		pPlayerLayer->retain();
		addChild( pPlayerLayer,1 );

		pGame = new Game;
		//DefaultAI* pai1 = new DefaultAI("N1");
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
	if( !pGame->isEndOfGame() && pPlayerLayer->inoperation == false ){
		pGame->runTheGameOneRound();
		pAcquireLayer->updateGameRender();
		pPlayerLayer->updatePlayerRender();
		if( pGame->isStupidHumanSTurn() ){
			pPlayerLayer->updatePlayerLogic();
		}
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
