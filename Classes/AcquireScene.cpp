#include "AcquireScene.h"


USING_NS_CC;
#include "Game/board.h"
#include "Game/definitions.h"
#include "Game/PlayerAI.h"
#include "Game/order.h"
#include "ATileSprite.h"
#include <algorithm>

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

	for( UINT i=0; i<5; i++ ){
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


void PlayerLayer::updatePlayerRender(){
	UINT i = 0;
	/*std::vector<ATile> handtiles;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it ){
		handtiles.push_back( *it );
	}
	sort( handtiles.begin(), handtiles.end() );*/
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
    if ( !CCLayerColor::init() ){
        return false;
    }
    CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();

	this->setTouchEnabled(true);
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
	//const ATile& t = *( pplayer->ATiles.begin() );
	UINT i = 0;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		if( i==selected_tile_index){
			PlaceATileOrder od( *it );
			return od;
		}
	}
	return DefaultAI::decidePlaceATile( bs );
}

void PlayerLayer::setGameStatus( GameStatus* gs ){
	pGame = gs;
}

void PlayerLayer::toggleDimmedBackGround( bool toggled ){
	this->setColor( toggled ? ccBLUE:ccBLACK );
	this->setOpacity( toggled ? 175: 0 );
}

void PlayerLayer::askPlayerToPlaceTile(){
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	toggleDimmedBackGround(true);
	pHintLabel->setVisible(true);
	pHintLabel->setString("Please select a Tile");
	pMenu = CCMenu::create();

	UINT i = 0;
	CCLabelTTF* pl;
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it,++i ){
		pl = createImageLabel(ccp(0,-FONT_SIZE/2),CCRectMake(BLOCK_SIZE,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE*1.2, it->getCaption());
		CCMenuItemLabel* mi = CCMenuItemLabel::create(pl);
		mi->initWithTarget(this, menu_selector(PlayerLayer::onPlayerSelectedAnATile) );
		mi->setPosition( i*BLOCK_SIZE*1.5, BLOCK_SIZE );
		mi->setScale(1.5);
		pMenu->addChild(mi,0,i);
	}
	
	pl = createImageLabel(ccp(0,BLOCK_SIZE/3),CCRectMake(0,0,BLOCK_SIZE,BLOCK_SIZE),FONT_SIZE,"OKAY");
	CCMenuItemLabel* mi = CCMenuItemLabel::create( pl, this, menu_selector(PlayerLayer::onPlayerPlacedTile) );
	mi->setPosition( BLOCK_SIZE*7, -2*BLOCK_SIZE );
	pMenu->addChild(mi,0,11);
	pMenu->setPosition( ccp(BLOCK_SIZE*2,BLOCK_SIZE*4) );
	this->addChild( pMenu );
}

void PlayerLayer::onPlayerSelectedAnATile(cocos2d::CCObject *pSender){
	CCMenuItemLabel* mi = ( CCMenuItemLabel* ) pSender ;
	char info[50];
	selected_tile_index = mi->getTag();
	sprintf_s( info, "No.%d ATILE selected", selected_tile_index );
	pHintLabel->setString(info);

}

void PlayerLayer::onPlayerPlacedTile(cocos2d::CCObject *pSender){
	pGame->updateGameStage();
	this->setTouchEnabled(true);
	pMenu->removeFromParentAndCleanup( true );
	pHintLabel->setVisible(false);
	toggleDimmedBackGround(false);
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
