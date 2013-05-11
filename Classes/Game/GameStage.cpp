#include "GameStage.h"

#include "Board.h"
#include "PlayerAI.h"
#include "order.h"

#include <algorithm>
using namespace std;


bool PlaceTileCommand::run(){
	static bool notified = false;
	if( !notified ){
		pPlayer->pai->askPlayerToPlaceTile();
		notified = true;
	}
	PlayerAI* pai = pPlayer->pai;
	if( pai->ready ){
		const PlaceATileOrder od = pai->decidePlaceATile( pGame->gs );
		pGame->current_ATile = od.t;
		if( od.execute( pGame, pPlayer ) ){
			for_each( pGame->players.begin(), pGame->players.end(), [this,&od](Player* pp ){
				pp->pai->fyiPlayerPlaceATileOrder( pPlayer->id , od );
			});
		}
		
		pGame->pme = new MergeEvent( pGame->allblocks, pGame->current_ATile ); 
		if( pGame->pme->isValidMerger() ){
			pGame->addCommand( new MergeBlockCommand( pPlayer, pGame ) );
		}
		else if( pGame->pme->isNewBlock() ){
			pGame->addCommand( new SetupCommpanyCommand( pPlayer, pGame ) );
			pGame->addCommand( new BuyStockCommand( pPlayer, pGame ) );
		}
		else if( pGame->pme->isAdjToOneBlock() ){
			Block* adjBlock = pGame->pme->sorted_blocks[0];
			adjBlock->addATile( pGame->current_ATile );
			pGame->addCommand( new BuyStockCommand( pPlayer, pGame ) );
		}
		notified = false;
		pai->ready = false;
		return true;
	}
	
	return false;
}

bool MergeBlockCommand::run(){
	PlayerAI* pai = pPlayer->pai;

	vector<Block*>& blocks_will_be_merged = pGame->pme->sorted_blocks;
	Block& AcquiringBlock = *blocks_will_be_merged[0];
	Block& AcquiredBlock  = *blocks_will_be_merged[1];
	pGame->pme->acquiring = AcquiringBlock;
	pGame->pme->acquired  = AcquiredBlock;
	COMPANY stakecompany = AcquiredBlock.c;
	vector<Player* > shareholders;
			
	for( unsigned int i=0; i<pGame->players.size(); i++ ){
		Player& pp = *pGame->players[i];
		if( pp.hasStock( stakecompany ) ){
			shareholders.push_back( &pp );
		}
	}

	pGame->allocateBonusFor(  stakecompany, shareholders );
	for( unsigned int i=0; i<shareholders.size(); i++ ){
		Player* ppp = shareholders[i];
		pGame->addCommand( new SellStockCommand( pPlayer, pGame ) );
		pGame->addCommand( new ConvertStockCommand( pPlayer, pGame ) );
	}
	
	pGame->stocktable.markCompanyAvailable( stakecompany, 1 );

	AcquiringBlock.mergeWith( AcquiredBlock );
	AcquiringBlock.addATile( pGame->current_ATile );

	blocks_will_be_merged.erase( blocks_will_be_merged.begin()+1 );
	pGame->pme->companies_to_be_removed.push_back( stakecompany );

	if( pGame->pme->isValidMerger() ){
		pGame->addCommand( new MergeBlockCommand( pPlayer, pGame ) );
	}
	else{
		pGame->pme->removedTheMergedCompanyFromAllBlocks(pGame->allblocks);
	}	

	return true;
}

bool SellStockCommand::run(){
	static bool notified = false;
	PlayerAI* pai = pPlayer->pai;
	if( !notified ){
		pPlayer->pai->askPlayerToSellStock();
		notified = true;
	}
	
	if( pai->ready ){
		const SellStockOrder od = pai->decideSellStock( pGame->gs );
		if( od.execute( pGame, pPlayer ) ){
			for_each( pGame->players.begin(), pGame->players.end(), [this,&od](Player* pp ){
				pp->pai->fyiPlayerSellStockOrder( pPlayer->id , od );
			});
		}
		notified = false;
		pai->ready = false;
		return true;
	}
	return false;
}

bool ConvertStockCommand::run(){
	static bool notified = false;
	PlayerAI* pai = pPlayer->pai;
	if( !notified ){
		pai->askPlayerToConvertStock();
		notified = true;
	}
	if( pai->ready ){
		const ConvertStockOrder od = pai->decideDoStockConversion( pGame->gs );
		if( od.execute( pGame, pPlayer ) ){
			for_each( pGame->players.begin(), pGame->players.end(), [this,&od](Player* pp ){
				pp->pai->fyiPlayerConvertStockOrder( pPlayer->id , od );
			});
		}
		pai->ready = false;
		notified = false;
		return true;
	}
	return false;
}

bool SetupCommpanyCommand::run(){
	static bool notified = false;
	PlayerAI* pai = pPlayer->pai;
	if( !notified ){
		pai->askPlayerToSetupCompany();
		notified = true;
	}
	
	if( pai->ready ){
		SetupCompanyOrder od = pai->decideSetupCompany( pGame->gs );
		if( od.execute( pGame, pPlayer ) ){
			for_each( pGame->players.begin(), pGame->players.end(), [this,&od](Player* pp ){
				pp->pai->fyiPlayerSetupCompanyOrder( pPlayer->id , od );
			});
			Block nb(od.c);
			nb.addATile( pGame->current_ATile );
			pGame->allblocks.push_back( nb );
		}
		
		notified = false;
		pai->ready = false;
		return true;
	}
	return false;
}

bool BuyStockCommand::run(){//using this as the ending round command
	static bool notified = false;
	PlayerAI* pai = pPlayer->pai;
	if( !notified ){
		pai->askPlayerToBuyStock();
		notified = true;
	}
	
	if( pai->ready ){
		const BuyStockOrder od = pai->decideBuyStocks( pGame->gs );
		if( od.execute( pGame, pPlayer ) ){
			for_each( pGame->players.begin(), pGame->players.end(), [this,&od](Player* pp ){
				pp->pai->fyiPlayerBuyStockOrder( pPlayer->id , od );
			});
		}
		delete pGame->pme;
		pGame->allocatePlayerOneATile( pPlayer );
		notified = false;
		pai->ready = false;
		return true;
	}
	return false;
}