#include "PlayerAI.h"
#include "order.h"
#include "Board.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <cmath>

//AI cannot change PLAYER status
const ConvertStockOrder DefaultAI::decideDoStockConversion(const GameStatus& bs ){
	COMPANY c = bs.getAcquiredBlock()->c;
	ConvertStockOrder od( c , pplayer->getStockAmt( c ) );
	return od;
}
const PlaceATileOrder DefaultAI::decidePlaceATile( const GameStatus& bs ){
	const ATile& t = *( pplayer->ATiles.begin() );
	PlaceATileOrder od( t );
	return od;
}
const BuyStockOrder DefaultAI::decideBuyStocks( const GameStatus& bs){
	srand (time(NULL));
	COMPANY c = (COMPANY)(rand()%NUMBER_OF_STOCKS);
	BuyStockOrder od( c, 1 );
	return od;
}
const SellStockOrder DefaultAI::decideSellStock( const GameStatus& bs){
	srand (time(NULL));
	COMPANY c = (COMPANY)(rand()%NUMBER_OF_STOCKS);
	SellStockOrder od( c, 1 );
	return od;
}
const SetupCompanyOrder DefaultAI::decideSetupCompany( const GameStatus& bs ){
	vector<COMPANY> cs = bs.getAllAvailableCompanies() ;
	COMPANY c;
	cs.size() > 0 ? c=cs[0]:c=(COMPANY)0;
	SetupCompanyOrder od(c);
	return od;
}

////////////////////////////////////////////////////////////////////////////////////////
//AI cannot change PLAYER status
int ZHENWAI::calculateTile2BlockDistance( const ATile& t, const Block& b ){
	int mindist = INT_MAX;
	for( auto it= b.ATiles.begin(); it!=b.ATiles.end(); ++it ){
		int dist = abs( t.row - it->row ) + abs( t.col -it->col );
		mindist = min( dist, mindist );
	}
	return mindist;
}
int ZHENWAI::calculateBlock2BlockDistance( const Block& b1, const Block& b2 ){
	int mindist = INT_MAX;
	for( auto it = b1.ATiles.begin(); it!=b1.ATiles.end(); ++it ){
		int dist = ZHENWAI::calculateTile2BlockDistance( *it, b2 );
		mindist = min( dist, mindist );
	}
	return mindist;
}

bool ZHENWAI::isTileNext2PlayerBlock( const ATile& t, const GameStatus& bs, const Player& p ){
	vector<Block> ab = bs.getAllBlocks();
	for( auto it = ab.begin(); it!=ab.end(); ++it ){
		if( it->c == EMPTY || !p.hasStock( it->c ) ) continue;
		if( calculateTile2BlockDistance(t,*it) == 1 ) return true;
	}
	return false;
}
const ConvertStockOrder ZHENWAI::decideDoStockConversion(const GameStatus& bs ){
	COMPANY c = bs.getAcquiredBlock()->c;
	ConvertStockOrder od( c , pplayer->getStockAmt( c ) );
	return od;
}
const PlaceATileOrder ZHENWAI::decidePlaceATile( const GameStatus& bs ){
	const ATile& t = *( pplayer->ATiles.begin() );
	PlaceATileOrder od( t );
	return od;
}
const BuyStockOrder ZHENWAI::decideBuyStocks( const GameStatus& bs){
	srand (time(NULL));
	COMPANY c = (COMPANY)(rand()%NUMBER_OF_STOCKS);
	BuyStockOrder od( c, 1 );
	return od;
}
const SellStockOrder ZHENWAI::decideSellStock( const GameStatus& bs){
	srand (time(NULL));
	COMPANY c = (COMPANY)(rand()%NUMBER_OF_STOCKS);
	SellStockOrder od( c, 1 );
	return od;
}
const SetupCompanyOrder ZHENWAI::decideSetupCompany( const GameStatus& bs ){
	COMPANY  ec = WORLDWIDE;
	int max_price = 0;
	vector<COMPANY> cs = bs.getAllAvailableCompanies() ;
	for( auto it=cs.begin(); it!=cs.end(); ++it ){
		int p = bs.getStockPrice( *it );
		if( p > max_price ){
			ec = *it;
			max_price = p;
		}
	}

	SetupCompanyOrder od(ec);
	return od;
}
	
