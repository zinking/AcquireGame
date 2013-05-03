#pragma once
#include <vector>
#include <string>
using namespace std;
struct GameStatus;
struct ATile;
struct Player;

struct PlaceATileOrder;
struct SellStockOrder;
struct HoldStockOrder;
struct ConvertStockOrder;
struct BuyStockOrder;
struct SetupCompanyOrder;

struct PlayerAI{
	const Player* pplayer;
	string id;
	virtual const string getID(){ return id;}
	//AI cannot change PLAYER status
	virtual void setPlayer( const Player* p ){ pplayer = p; }
	virtual const ConvertStockOrder decideDoStockConversion(const GameStatus& bs )=0;
	virtual const PlaceATileOrder decidePlaceATile( const GameStatus& bs )=0;
	virtual const BuyStockOrder decideBuyStocks( const GameStatus& bs)=0;
	virtual const SellStockOrder decideSellStock( const GameStatus& bs)=0;
	virtual const SetupCompanyOrder decideSetupCompany( const GameStatus& bs )=0;
	
	virtual void fyiPlayerInfos( const vector<string> ids)=0 ;
	virtual void fyiPlayerBuyStockOrder( const string& player, const BuyStockOrder& ods )=0;
	virtual void fyiPlayerConvertStockOrder( const string& player, const ConvertStockOrder& ods )=0;
	virtual void fyiPlayerPlaceATileOrder( const string& player, const PlaceATileOrder& ods )=0;
	virtual void fyiPlayerSellStockOrder( const string& player, const SellStockOrder& ods )=0;
	virtual void fyiPlayerSetupCompanyOrder( const string& player, const SetupCompanyOrder& ods )=0;
};

struct DefaultAI:PlayerAI{
	DefaultAI( string iid ){ id = iid; }
	//const string getID()const;
	//AI cannot change PLAYER status
	const ConvertStockOrder decideDoStockConversion(const GameStatus& bs );
	const PlaceATileOrder decidePlaceATile( const GameStatus& bs );
	const BuyStockOrder decideBuyStocks( const GameStatus& bs);
	const SellStockOrder decideSellStock( const GameStatus& bs);
	const SetupCompanyOrder decideSetupCompany( const GameStatus& bs );
	
	void fyiPlayerInfos( const vector<string> ids){}
	void fyiPlayerBuyStockOrder( const string& player, const BuyStockOrder& ods ){}
	void fyiPlayerConvertStockOrder( const string& player, const ConvertStockOrder& ods ){}
	void fyiPlayerPlaceATileOrder( const string& player, const PlaceATileOrder& ods ){}
	void fyiPlayerSellStockOrder( const string& player, const SellStockOrder& ods ){}
	void fyiPlayerSetupCompanyOrder( const string& player, const SetupCompanyOrder& ods ){}
};
