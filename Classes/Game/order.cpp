#include <iostream>
using namespace std;

#include "Board.h"
#include "Player.h"
#include "order.h"
#include "definitions.h"
extern char COMPANYNAME[NUMBER_OF_STOCKS][20];
extern char info[200];


/*
HoldStockOrder::HoldStockOrder( COMPANY cc ):c(cc){}
string HoldStockOrder::toString(){
	sprintf_s( info, "HOLD ORDER %s", COMPANYNAME[c]);
	return info;
}
*/

BuyStockOrder::BuyStockOrder( COMPANY cc, int amt ):c(cc),count(amt){}
string BuyStockOrder::toString(){
	sprintf_s( info, "BUY[%s]*[%d]", COMPANYNAME[c], count);
	return info;
}

SellStockOrder::SellStockOrder( COMPANY cc, int amt ):c(cc),count(amt){}
string SellStockOrder::toString(){
	sprintf_s( info, "SELL[%s]*[%d]", COMPANYNAME[c], count);
	return info;
}



PlaceATileOrder::PlaceATileOrder( const ATile& tt ):t(tt){}
string PlaceATileOrder::toString(){
	sprintf_s( info, "TILE:%s", t.toString().c_str() );
	return info;
}

bool PlaceATileOrder::isValid( const GameStatus& gs, const Player& from )const{
	return from.hasATile( t );
}

bool PlaceATileOrder::execute( Game* g, Player* p )const{
	if( isValid( g->gs, *p )){
		sprintf_s( info, "PLAYER[%s] PLACED AN ATile[%s]", p->getID(),t.toString().c_str() );
		cout << info << endl;
		p->removeATile( t );
		g->messages.push_back( info );
		return true;
	}
	else{
		sprintf_s( info, "PLAYER[%s] PLACED AN INVALID ATile[%s] IGNORED", p->getID(),t.toString().c_str() );
		cout << info << endl;
		g->messages.push_back( info );
		return false;
	}
}

bool SellStockOrder::isValid( const GameStatus& gs, const Player& from )const{
	if( c>=0 && c<=NUMBER_OF_STOCKS )
		return count > 0 && from.getStockAmt( c ) > count;
	else
		return false;
}

bool SellStockOrder::execute( Game* g, Player* p )const{
	if( isValid( g->gs, *p )){
		int sharevalue = g->stocktable.stockprice[c] * count ;
		p->addStock( c, count * -1 );
		p->debitCash( sharevalue );

		sprintf_s( info, "PLAYER[%s] selled %d shares of [%s]", p->getID(), count, COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return true;
	}
	else{
		sprintf_s( info, "PLAYER[%s] invalid sell [ %d * %s]", p->getID(),count, COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return false;
	}
}

ConvertStockOrder::ConvertStockOrder(  COMPANY cc, int amt ):c(cc),count(amt){}
string ConvertStockOrder::toString(){
	sprintf_s( info, "CONVERT STOCK ORDER %d shares of [%s] ", count ,COMPANYNAME[c]);
	return info;
}

bool ConvertStockOrder::isValid( const GameStatus& gs, const Player& from )const{
	if( c>=0 && c<=NUMBER_OF_STOCKS )
		return c==gs.AcquiredBlock.c && count > 0 && from.getStockAmt( c ) > count;
	else return false;
}

bool ConvertStockOrder::execute( Game* g, Player* p )const{
	if( isValid( g->gs, *p )){
		int stocks_converted  = count/2;
		int stocks_to_convert = stocks_converted*2;
		p->addStock( g->gs.AcquiredBlock.c, stocks_to_convert * -1 );
		p->addStock( g->gs.AcquiringBlock.c, stocks_converted );

		sprintf_s( info, "PLAYER[%s] converted %d shares of [%s]", p->getID(), count, COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return true;
	}
	else{
		sprintf_s( info, "PLAYER[%s] invalid convert [ %d * %s]", p->getID(),count, COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return false;
	}
}

bool BuyStockOrder::isValid( const GameStatus& gs, const Player& from )const{
	if( c>=0 && c<=NUMBER_OF_STOCKS && !gs.isCompanyAvailable( c ) ){
		int cost = gs.getStockPrice( c ) * count ;
		return from.getCash() > cost;
	}
	return false;
}

bool BuyStockOrder::execute( Game* g, Player* p )const{
	if( isValid( g->gs, *p )){
		int cost = g->gs.getStockPrice( c ) * count ;
		p->debitCash( cost * -1 );
		p->addStock( c, count );

		sprintf_s( info, "PLAYER[%s] buys %d shares of [%s]]", p->getID(), count, COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return true;
	}
	else{
		sprintf_s( info, "PLAYER[%s] invalid buy [ %d * %s]", p->getID(),count, COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return false;
	}
}
SetupCompanyOrder::SetupCompanyOrder( COMPANY cc ):c(cc){}
bool SetupCompanyOrder::isValid( const GameStatus& gs, const Player& from )const{
	if( c>=0 && c<=NUMBER_OF_STOCKS )
		return gs.isCompanyAvailable( c );
	else return false;
}

bool SetupCompanyOrder::execute( Game* g, Player* p )const{
	if( isValid( g->gs, *p )){
		p->addStock( c, 1 );
		g->stocktable.markCompanyAvailable(c,0);
		sprintf_s( info, "PLAYER[%s] setup COMPANY[%s] and get a share", p->getID(),  COMPANYNAME[c] );
		cout << info << endl;
		g->messages.push_back( info );
		return true;
	}
	else{
		sprintf_s( info, "PLAYER[%s] placed an invalid setup COMPANY order", p->getID() );
		cout << info << endl;
		g->messages.push_back( info );
		return false;
	}
}
string SetupCompanyOrder::toString(){
	sprintf_s( info, "SETUP COMPANY ORDER of [%s] " ,COMPANYNAME[c]);
	return info;
}









