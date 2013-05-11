#ifndef __ORDER_H__
#define __ORDER_H__
struct GameStatus;
struct Player;
struct ATile;
struct Game;
#include "definitions.h"

struct Order{
	virtual bool execute( Game* b, Player* p)const{ return false; }
	virtual bool isValid( const GameStatus& bs, const Player& from )const{ return false; }
	virtual string toString()=0;
};


struct PlaceATileOrder:Order{
	const ATile& t;
	PlaceATileOrder( const ATile& tt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};


struct SellStockOrder:Order{
	COMPANY c;
	int  count;
	SellStockOrder( COMPANY cc, int amt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct LiquidateStockOrder:SellStockOrder{
	COMPANY c;
	int  count;
	LiquidateStockOrder( COMPANY cc, int amt ):SellStockOrder(cc,amt){}
	bool isValid( const GameStatus& gs, const Player& from )const;
};

struct ConvertStockOrder:Order{
	COMPANY c;
	int  count;
	ConvertStockOrder( COMPANY cc, int amt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct BuyStockOrder:Order{
	COMPANY c;
	int  count;
	BuyStockOrder( COMPANY cc, int amt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct SetupCompanyOrder:Order{
	const COMPANY c;
	SetupCompanyOrder( const COMPANY c );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

#endif











