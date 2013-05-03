#pragma once
#include <set>
#include <vector>
#include <ctime>
using namespace std;
#include "definitions.h"
#include "Player.h"

struct Tile{
	int row;
	int col;
	Tile( int r, int c );
	Tile();
	//int hashCode(){ return row*31+col; }
	bool operator==( const Tile& rhs ) const;
	bool operator<(  const Tile& rhs ) const;
	string toString() const ;
};

struct Block{
	enum COMPANY  c;
	set<Tile> tiles;
	Block( enum COMPANY cc );
	Block();
	bool hasTile( const Tile& t );
	void addTile( const Tile& t );
	//why block would have no company setup at all?
	bool hasCompany();
	bool operator==( const Block& rhs ) const;
	void mergeWith( Block other );
	string toString()const;
};

struct PlayerAI;
struct  Player{
	string id;
	PlayerAI* pai;//by const I mean this pointer cannot be changed
	Player( PlayerAI* pai );
	void initialize();
	time_t getTimeUsed() const ;
	const char* getID() const;
	int getCash() const ;
	void debitCash( int amount );
	void getAllocatedTiles(const set<Tile>& t );
	void addStock( enum COMPANY c, int count);
	void removeTile( const Tile& t );
	void addTile( const Tile& t );
	bool hasTile( const Tile& t )const ;
	bool hasStock( const enum COMPANY c ) const;
	int getStockAmt( const enum COMPANY c ) const;
	int tileCount()const;
	string toString();
	set<Tile> tiles;
	time_t time_used;
	int  cash;
	int  stocks[NUMBER_OF_STOCKS];
};

struct MergeEvent{
	Block* adjblocks[4];
	vector<Block*> sorted_blocks;
	//int largest_block_index;
	//set<COMPANY> adjcompanies;

	MergeEvent( vector<Block>& allBlocks, const Tile& newTile );
	Block* getBlockWithTile( vector<Block>& allblocks, Tile& newTile);
	bool isValidMerger()const;
	bool isNewBlock()const;
	bool isAdjToOneBlock()const;
};

struct StockTable{
	int stockprice[NUMBER_OF_STOCKS];
	int majorbonus[NUMBER_OF_STOCKS];
	int minorbonus[NUMBER_OF_STOCKS];
	int stockamont[NUMBER_OF_STOCKS];
	int available[NUMBER_OF_STOCKS];
	//int stockavail[NUMBER_OF_STOCKS];

	StockTable();

	void markCompanyAvailable( const COMPANY& c, int flag ){
		available[c] = flag;
	}

	const vector<COMPANY> getAvailableCompanies() const{
		vector<COMPANY> r;
		for( unsigned int i=0; i<NUMBER_OF_STOCKS; i++ ){
			if ( available[i] != 0 ){
				r.push_back( (COMPANY)i );
			}
		}
		return r;
	}

};

struct Order;
struct Game;
/*this class intends to provide trimmed down access to the game class*/
struct GameStatus{
	const Game* pbd;
	Block AcquiringBlock;
	Block AcquiredBlock;
public:
	GameStatus( );
	/*this method intends to tell player what blocks are available now*/
	const vector<Tile>  getAvailableTiles() const;
	const vector<Block> getAllBlocks()const;
	const vector<COMPANY> getAllAvailableCompanies()const;
	const string toString()const;
	int getStockPrice( const COMPANY& c )const;
	bool isCompanyAvailable( const COMPANY& c )const;

};

struct Game{
	//int board[WIDTH][HEIGH];
	//static const int STOCK_SIZE=25;
	int stocks[NUMBER_OF_STOCKS];
	vector<Tile> alltiles;
	vector<Block> allblocks;
	vector<Player*> players;
	GameStatus gs;
	StockTable stocktable;

	int round;
	Tile current_tile;
	Game( );
	void initGame();
	void addPlayer( Player* player );
	void initPlayerWithTiles();
	bool isEndOfGame();
	const Tile askPlayerToPlaceTile( Player* p );
	void allocatePlayerOneTile( Player* p );
	void askPlayerToBuyStock(  Player* p );
	void askPlayerToSetupCompany(  Player* p , const Tile& t  );
	void askPlayersToSellStock( const vector<Player*> shareholders );
	void askPlayersToConvertStock( const vector<Player*> shareholders );
	void runTheGame();
	vector<int> getAvailableNewCompanies();
	void doAcquire( Block& AcquiringBlock, Block& AcquiredBlock, Tile& via );
	void allocateBonusFor( enum COMPANY c , const vector<Player*> shs );
	void statistics();
};







