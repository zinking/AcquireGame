#pragma once
#include <set>
#include <vector>
#include <ctime>
using namespace std;
#include "definitions.h"
#include "Player.h"

struct ATile{
	int row;
	int col;
	ATile( int r, int c );
	ATile();
	//int hashCode(){ return row*31+col; }
	bool operator==( const ATile& rhs ) const;
	bool operator<(  const ATile& rhs ) const;
	string toString() const ;
};

struct Block{
	enum COMPANY  c;
	set<ATile> ATiles;
	Block( enum COMPANY cc );
	Block();
	bool hasATile( const ATile& t );
	void addATile( const ATile& t );
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
	void getAllocatedATiles(const set<ATile>& t );
	void addStock( enum COMPANY c, int count);
	void removeATile( const ATile& t );
	void addATile( const ATile& t );
	bool hasATile( const ATile& t )const ;
	bool hasStock( const enum COMPANY c ) const;
	int getStockAmt( const enum COMPANY c ) const;
	int ATileCount()const;
	string toString();
	set<ATile> ATiles;
	time_t time_used;
	int  cash;
	int  stocks[NUMBER_OF_STOCKS];
};

struct MergeEvent{
	Block* adjblocks[4];
	vector<Block*> sorted_blocks;
	//int largest_block_index;
	//set<COMPANY> adjcompanies;

	MergeEvent( vector<Block>& allBlocks, const ATile& newATile );
	Block* getBlockWithATile( vector<Block>& allblocks, ATile& newATile);
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
	const vector<ATile>  getAvailableATiles() const;
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
	vector<ATile> allATiles;
	vector<Block> allblocks;
	vector<Player*> players;
	GameStatus gs;
	StockTable stocktable;

	int round;
	ATile current_ATile;
	Game( );
	void initGame();
	void addPlayer( Player* player );
	void initPlayerWithATiles();
	bool isEndOfGame();
	const ATile askPlayerToPlaceATile( Player* p );
	void allocatePlayerOneATile( Player* p );
	void askPlayerToBuyStock(  Player* p );
	void askPlayerToSetupCompany(  Player* p , const ATile& t  );
	void askPlayersToSellStock( const vector<Player*> shareholders );
	void askPlayersToConvertStock( const vector<Player*> shareholders );
	void runTheGame();
	vector<int> getAvailableNewCompanies();
	void doAcquire( Block& AcquiringBlock, Block& AcquiredBlock, ATile& via );
	void allocateBonusFor( enum COMPANY c , const vector<Player*> shs );
	void statistics();
};







