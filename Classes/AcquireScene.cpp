#include "AcquireScene.h"


USING_NS_CC;
#include "Game/board.h"
#include "Game/definitions.h"
#include "Game/PlayerAI.h"
#include "ATileSprite.h"



CCScene* AcquireScene::scene()
{
    CCScene *scene = CCScene::create();
    AcquireScene *layer = AcquireScene::create();
    scene->addChild(layer);
    return scene;
}

void AcquireScene::initGameUI()
{
	CCSize  sz = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint op = CCDirector::sharedDirector()->getVisibleOrigin();
	int dx = op.x + BLOCK_SIZE;
	int dy = op.y + sz.height - BLOCK_SIZE * HEIGH + BLOCK_SIZE/2;
	for( int i=0; i<HEIGH; i++ ){//row
		for( int j=0; j<WIDTH; j++ ){//col
			ATileLabel* at = ATileLabel::create();
			at->setPosition( ccp(  j*BLOCK_SIZE+dx , i*BLOCK_SIZE+dy ) );
			at->initTile(i,j);
			ats[i][j] = at;
			this->addChild(at);
		}
	}
	for ( unsigned int i=0; i<pGame->players.size(); i++ ){
		CCNode* player_node  = new CCNode;
		player_node->setPosition(  op.x + BLOCK_SIZE/2, op.y + i*BLOCK_SIZE + BLOCK_SIZE/2 );
		pcon.push_back( player_node );
		this->addChild( player_node );
	}
	updateGameRender();
}

void AcquireScene::initGameLogic(){
	pGame = new Game;
	DefaultAI* pai1 = new DefaultAI("N1");
	DefaultAI* pai2 = new DefaultAI("N2");

	Player* pa = new Player(pai1);
	pai1->setPlayer( pa );
	Player* pb = new Player(pai2);
	pai2->setPlayer( pb );

	pGame->addPlayer( pa );
	pGame->addPlayer( pb );

	pGame->initPlayerWithATiles();
}

extern char COMPANYNAME[NUMBER_OF_STOCKS][20];

void AcquireScene::updateGameRender(){
	for( int i=0; i<pGame->players.size(); i++ ){
		Player* p  = pGame->players[i];
		CCNode* pn = pcon[i];
		pn->removeAllChildren();
		char caption[20];
		int index = 0;
		CCLabelTTF* pl;
		for( auto it=p->ATiles.begin(); it!=p->ATiles.end(); ++it){
			pl = CCLabelTTF::create( it->getCaption().c_str(), "Arial", 7, CCSize( BLOCK_SIZE,BLOCK_SIZE), kCCTextAlignmentCenter);
			pl->setPositionX( index * BLOCK_SIZE );
			pn->addChild( pl );
			index++;
		}
		for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
			sprintf_s( caption, "%c:%d", COMPANYNAME[i][0], p->stocks[i] );
			pl = CCLabelTTF::create( caption, "Arial", 7, CCSize( BLOCK_SIZE,BLOCK_SIZE), kCCTextAlignmentCenter);
			pl->setPositionX( index * BLOCK_SIZE );
			pn->addChild( pl );
			index++;
		}
		sprintf(caption,"[%s] CASH:%d",p->getID(),p->getCash() );
		pl = CCLabelTTF::create( caption, "Arial", 7, CCSize( BLOCK_SIZE,BLOCK_SIZE), kCCTextAlignmentCenter);
		pl->setPositionX( index * BLOCK_SIZE );
		pn->addChild( pl );
	}

	for( auto it=pGame->allblocks.begin(); it!=pGame->allblocks.end(); ++it ){
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
    
	initGameLogic();
	
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

	initGameUI();
    
    return true;
}

void AcquireScene::menuClickCallBack(CCObject* pSender)
{
	if( !pGame->isEndOfGame() ){
		pGame->runTheGameOneRound();
		updateGameRender();
	}
}

void AcquireScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}

