#include "PlayerAI.h"
#include "order.h"
#include "Board.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>

//AI cannot change PLAYER status
const ConvertStockOrder DefaultAI::decideDoStockConversion(const GameStatus& bs ){
	COMPANY c = bs.AcquiredBlock.c;
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
	
