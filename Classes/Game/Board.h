#pragma once
#include <set>
#include <vector>
#include <queue>
#include <ctime>
#include <string>
using namespace std;
#include "definitions.h"

struct ATile{
	int row;
	int col;
	ATile( int r, int c );
	ATile();
	bool operator==( const ATile& rhs ) const;
	bool operator<(  const ATile& rhs ) const;
	string toString() const;
	string getCaption() const;
};

struct Block{
	COMPANY  c;
	set<ATile> ATiles;
	Block( COMPANY cc );
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
struct Player{
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
	vector<Block>& allblocks;
	vector<Block*> sorted_blocks; //all adj blocks with compay setup in sorted order
	vector<Block*> isolat_blocks; //all adj blocks with no company setup
	vector<string> companies_to_be_removed;
	Block* acquiring;
	Block* acquired;
	ATile newATile;
	MergeEvent( vector<Block>& allBlocks, const ATile& newATile );
	Block* getBlockWithATile( vector<Block>& allblocks, const ATile& newATile);
	bool isValidMerger()const;
	bool isNewBlock()const;
	bool isSetupBlock() const;
	bool isAdjToOneBlock()const;
	void setupCompany( const COMPANY& c );
	//void removedTheMergedCompanyFromAllBlocks(  vector<Block>& allBlocks );
	void acquireIsolatedBlocks( Block& acquiring );
	void mergeNewTileWithExistingCompany();
	void mergeCompanyWithCompany();
	void cleanUp();
};

struct StockTable{
	int stockprice[NUM_STOCK_PRICE];
	int majorbonus[NUM_STOCK_PRICE];
	int minorbonus[NUM_STOCK_PRICE];


	int stockamont[NUMBER_OF_STOCKS];
	int available[NUMBER_OF_STOCKS];
	int stockseed[NUMBER_OF_STOCKS];

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

	const int getStockPrice( const COMPANY& c, int size ) const;
	const int getStockMajor( const COMPANY& c, int size ) const;
	const int getStockMinor( const COMPANY& c, int size ) const;

};

struct Order;
struct Game;
/*this class intends to provide trimmed down access to the game class*/
struct GameStatus{
	const Game* pbd;
public:
	GameStatus( );
	/*this method intends to tell player what blocks are available now*/
	const vector<ATile>   getAvailableATiles() const;
	const vector<Block>   getAllBlocks()const;
	const vector<COMPANY> getAllAvailableCompanies()const;
	const string toString()const;
	
	bool isCompanyAvailable( const COMPANY& c )const;

	const Block* getAcquiringBlock()const;
	const Block* getAcquiredBlock()const;
	const string getLastestMessage() const;
	const int getRound() const;

	int getStockPrice( const COMPANY& c )const;
	int getStockMajor( const COMPANY& c )const;
	int getStockMinor( const COMPANY& c )const;
};

class GameCommand;
struct Game{
	queue<GameCommand*> commandqueue;
	int stocks[NUMBER_OF_STOCKS];
	vector<ATile> allATiles;
	vector<Block> allblocks;
	vector<Player*> players;
	vector<string> messages;
	GameStatus gs;
	StockTable stocktable;
	MergeEvent* pme;
	int round;
	ATile current_ATile;
	Game( );
	void addCommand( GameCommand* gs ){ commandqueue.push( gs ); }
	void initGame();
	void addPlayer( Player* player );

	//void addNewTile();
	void initPlayerWithATiles();
	bool isEndOfGame();
	void allocatePlayerOneATile( Player* p );
	void runTheGameOneLoop();
	void runTheGameLoop();
	vector<int> getAvailableNewCompanies();
	void allocateBonusFor( enum COMPANY c , const vector<Player*> shs );
	void statistics();

	int getStockPrice( const COMPANY& c )const;
	int getStockMajor( const COMPANY& c )const;
	int getStockMinor( const COMPANY& c )const;

	int getCompanySize( const COMPANY& c ) const;

};







