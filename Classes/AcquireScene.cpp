#include "AcquireScene.h"


USING_NS_CC;
#include "Game/board.h"
#include "Game/definitions.h"
#include "Game/PlayerAI.h"
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

	updateGameRender();
}

void AcquireScene::initGameLogic(){
	/*
	pGame = new Game;
	DefaultAI* pai1 = new DefaultAI("N1");
	DefaultAI* pai2 = new DefaultAI("N2");

	Player* pa = new Player(pai1);
	pai1->setPlayer( pa );
	Player* pb = new Player(pai2);
	pai2->setPlayer( pb );

	pCur = pa;

	pGame->addPlayer( pa );
	pGame->addPlayer( pb );

	pGame->initPlayerWithATiles();
	*/
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
    return true;
}

void PlayerLayer::setPlayerName( string name ){ 
	id = name; 
}
