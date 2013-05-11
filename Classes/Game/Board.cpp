#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

#include "Board.h"
#include "Player.h"
#include "PlayerAI.h"
#include "order.h"
#include "definitions.h"

#define MAX_PLAYER_SIZE 6
extern char info[200];
extern char COMPANYNAME[NUMBER_OF_STOCKS][20];


ATile::ATile( int r, int c ):row(r),col(c){}
ATile::ATile():row(0),col(0){}
bool ATile::operator==( const ATile& rhs )const{
	return row == rhs.row && col == rhs.col;
}

bool ATile::operator<(  const ATile& rhs )const{
	return (row  == rhs.row ? col < rhs.col : row < rhs.row );
}
string ATile::toString() const {
	sprintf_s(info,"ATile[%d,%d]",row,col);
	return info;
}

string ATile::getCaption() const {
	sprintf_s(info,"%c%d",'A'+row,col+1);
	return info;
}

Block::Block( COMPANY cc ):c(cc){}
Block::Block(){}
bool Block::hasATile( const ATile&  t){
	return ATiles.find(t) != ATiles.end();
}

void Block::addATile( const ATile&  t){
	ATiles.insert( t );
}
//why block would have no company setup at all?
bool Block::hasCompany(){
	return c != -1;
}

bool Block::operator==( const Block& rhs )const{
	return false;
};


void Block::mergeWith( Block other ){
	for( auto it=other.ATiles.begin(); it!=other.ATiles.end(); ++it ){
		ATiles.insert(*it);
	}
}

string Block::toString()const{
	string ATilesinfo;
	for( auto it=ATiles.begin(); it!=ATiles.end(); ++it){
		ATilesinfo += it->toString() + " ";
	}
	sprintf_s(info,"BLOCK COMPANY[%s] WITH ATiles[%s]", COMPANYNAME[c], ATilesinfo.c_str() );
	return info;
}

StockTable::StockTable(){
	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		stockprice[i] = 200;
		majorbonus[i] = 2000;
		minorbonus[i] = 1500;
		available[i]  = 1; 
	}
}

Player::Player( PlayerAI* pai ){
	this->id  = pai->getID();
	this->pai = pai;
	this->pai->setPlayer( this );
	initialize();
}

void Player::initialize(){
	this->cash = 1500;
	this->time_used = 0;
	memset(stocks,0,sizeof(stocks));
}

const char* Player::getID() const{
	return id.c_str();
}

time_t Player::getTimeUsed() const {
	return time_used;
}

int Player::getCash() const {
	return cash;
}
void Player::debitCash( int amount ){
	cash += amount;
}
void Player::getAllocatedATiles(const set<ATile>& t ){
	ATiles = t;
}
void Player::addStock( COMPANY c, int count){
	stocks[c]+=count;
}
void Player::removeATile( const ATile& t ){
	ATiles.erase( t );
}
void Player::addATile( const ATile& t ){
	ATiles.insert( t );
}
bool Player::hasATile( const ATile& t )const {
	return ATiles.find( t ) != ATiles.end();
}
int Player::ATileCount()const{
	return ATiles.size();
}
bool Player::hasStock( const  COMPANY c ) const{
	return stocks[c] > 0 ;
}
int Player::getStockAmt( const COMPANY c ) const{
	return stocks[c]  ;
}
string Player::toString(){
	string shstr;
	for( int i=0;i<NUMBER_OF_STOCKS; i++ ){
		if( stocks[i] > 0 ){
			sprintf_s(info,"[%s(%d)] ", COMPANYNAME[i],stocks[i] );
			shstr += info;
		}
	}
	sprintf_s( info, "PLAYER[%s] TOTALCASH[%d] SHARE:%s ", id.c_str(), cash,shstr.c_str() );
	return info;
}

MergeEvent::MergeEvent( vector<Block>& allblocks, const ATile& na ){
	newATile = na;
	int y = newATile.row;
	int x = newATile.col;
	adjblocks[0]=0,adjblocks[1]=0,adjblocks[2]=0,adjblocks[3]=0;
	if( x-1>=0 )	adjblocks[0] = getBlockWithATile( allblocks, ATile(y,x-1));
	if( y-1>=0 )	adjblocks[1] = getBlockWithATile( allblocks, ATile(y-1,x));
	if( x+1<WIDTH ) adjblocks[2] = getBlockWithATile( allblocks, ATile(y,x+1));
	if( y+1<HEIGH ) adjblocks[3] = getBlockWithATile( allblocks, ATile(y+1,x));

	for( int j=0; j<4; j++ ){
		Block* ccb = adjblocks[j];
		if( ccb != 0 ){
			int index = 0;
			for(  ;index<sorted_blocks.size(); index++ ){
				if( ccb == sorted_blocks[index] ){
					index = -1 ;
					break;
				}
				if( ccb->ATiles.size() > sorted_blocks[index]->ATiles.size() ){
					break;
				}
			}
			if ( index >= 0 ) sorted_blocks.insert( sorted_blocks.begin() + index, ccb );
		}
	}
}
int GameStatus::getStockPrice( const COMPANY& c )const{
	return pbd->stocktable.stockprice[c];
}
bool GameStatus::isCompanyAvailable( const COMPANY& c )const{
	if(c>=0 && c<=NUMBER_OF_STOCKS )return pbd->stocktable.available[c] != 0;
}
const Block* GameStatus::getAcquiringBlock()const{
	return pbd->pme->sorted_blocks[0];
}
const Block* GameStatus::getAcquiredBlock()const{
	return pbd->pme->sorted_blocks[1];
}
const GAMESTAGE GameStatus::getGameState() const{
	return pbd->stage;
}

void GameStatus::updateGameStage(  ){
	pbd->updateGameStageForStupidHuman();
}

const string GameStatus::getLastestMessage() const{
	int len = pbd->messages.size();	
	return len > 0 ? pbd->messages[len-1] : "" ;
}

Block* MergeEvent::getBlockWithATile( vector<Block>& allblocks, ATile& newATile){
	int n = allblocks.size(), i=0;
	for( i=0; i<n; i++ ){
		Block& b = allblocks[i];
		if( b.hasATile( newATile ) ) return &b;
	}
	return 0;
}

bool MergeEvent::isValidMerger()const{
	return sorted_blocks.size() > 1;
}
bool MergeEvent::isNewBlock()const{
	return sorted_blocks.size() == 0;
}
bool MergeEvent::isAdjToOneBlock()const{
	return sorted_blocks.size() == 1;
}
void MergeEvent::removedTheMergedCompanyFromAllBlocks(  vector<Block>& allblocks ){
	for( auto it2=companies_to_be_removed.begin(); it2 != companies_to_be_removed.end(); ++it2 ){
		for( auto it=allblocks.begin(); it != allblocks.end(); ++it ){//clear the acquired block from the table
			if( it->c == *it2 ){
				allblocks.erase(it);
				break;
			}
		}
	}
}

	
Game::Game( ){
	initGame();
	gs.pbd = this;
	round = 1;
}

void Game::initGame(){
	int i=0,j=0;
	for( i=0; i<HEIGH; i++ )
		for( j=0; j<WIDTH; j++ )
			allATiles.push_back( ATile(i,j) );
	random_shuffle ( allATiles.begin(), allATiles.end() );
	stage = TO_PLACE_TILE;
}

void Game::addPlayer( Player* player ){
	if( players.size() < MAX_PLAYER_SIZE ) players.push_back( player );
}

void Game::allocatePlayerOneATile( Player* p ){
	if( allATiles.size() > 0 ){
		p->addATile( allATiles.back() );
		allATiles.pop_back();
	}
}

void Game::initPlayerWithATiles(){
	vector<string> pids;
	for_each( players.begin(), players.end(),[&pids]( Player* p){
		pids.push_back(p->id);
	});
	for_each( players.begin(), players.end(),[&pids]( Player* p){
		p->pai->fyiPlayerInfos( pids );
	});

	for_each( players.begin(), players.end(),[this]( Player* p){
		for( int j=0; j<NUMBER_OF_ATileS_EACH; j++ ){
			this->allocatePlayerOneATile(p);
		}
	});
}

bool Game::isEndOfGame() {
	bool allBlockGT11 = true;
	bool oneBlockGE41 = false;
	int companyNumber = 0;
	bool hasnoATile = ( allATiles.size() == 0 );

	for_each( allblocks.begin(), allblocks.end(), [&allBlockGT11,&oneBlockGE41](Block& b){
		if( b.ATiles.size() < 11 ) allBlockGT11 = false;
		else if( b.ATiles.size() >= 41 ) oneBlockGE41 = true;
	});

	if( allblocks.size() == 0 ) allBlockGT11 = false;

	bool oneplayerhasnoATile = true;
	for_each( players.begin(), players.end(), [&oneplayerhasnoATile]( Player* p ){
		if( p->ATileCount() > 0 ){
			oneplayerhasnoATile=false;
		}
	});

	return oneBlockGE41 || allBlockGT11 || oneplayerhasnoATile || hasnoATile;
}

const ATile Game::askPlayerToPlaceATile(  Player* p ){
	const PlaceATileOrder od = p->pai->decidePlaceATile( gs );
	ATile result = od.t;
	if( od.execute( this, p ) ){
		for_each( players.begin(), players.end(), [&p,&od](Player* pp ){
			pp->pai->fyiPlayerPlaceATileOrder( p->id , od );
		});
	}
	return result;
}

void Game::askPlayerToBuyStock(  Player* p ){
	const BuyStockOrder od = p->pai->decideBuyStocks( gs );
	if( od.execute( this, p ) ){
		for_each( players.begin(), players.end(), [&p,&od](Player* pp ){
			pp->pai->fyiPlayerBuyStockOrder( p->id , od );
		});
	}
}

void Game::askPlayerToSetupCompany(  Player* p, const ATile& t ){
	SetupCompanyOrder od = p->pai->decideSetupCompany( gs );
	if( od.execute( this, p ) ){
		for_each( players.begin(), players.end(), [&p,&od](Player* pp ){
			pp->pai->fyiPlayerSetupCompanyOrder( p->id , od );
		});

		Block nb(od.c);
		nb.addATile( t );
		allblocks.push_back( nb );
	}

}

void Game::askPlayersToSellStock( const vector<Player*> shareholders ){
	for( unsigned int i=0; i<shareholders.size(); i++ ){
		Player* ppp = shareholders[i];
		const SellStockOrder od = ppp->pai->decideSellStock( gs );
		if( od.execute( this, ppp ) ){
			for_each( players.begin(), players.end(), [&ppp,&od](Player* pp ){
				pp->pai->fyiPlayerSellStockOrder( ppp->id , od );
			});
		}
	}
	
}

void Game::askPlayersToConvertStock( const vector<Player*> shareholders ){
	for( unsigned int i=0; i<shareholders.size(); i++ ){
		Player* ppp = shareholders[i];
		const ConvertStockOrder od = ppp->pai->decideDoStockConversion( gs );
		if( od.execute( this, ppp ) ){
			for_each( players.begin(), players.end(), [&ppp,&od](Player* pp ){
				pp->pai->fyiPlayerConvertStockOrder( ppp->id , od );
			});
		}
	}
	
}

void Game::doAcquire( MergeEvent& me  ){
	vector<Block*>& blocks_will_be_merged = me.sorted_blocks;
	//vector<COMPANY>  companies_to_be_removed;
	while( blocks_will_be_merged.size() > 1 ){
		Block& AcquiringBlock = *blocks_will_be_merged[0];
		Block& AcquiredBlock  = *blocks_will_be_merged[1];
		doAcquireOnce( AcquiringBlock, AcquiredBlock, me.newATile );
		blocks_will_be_merged.erase( blocks_will_be_merged.begin()+1 );
		me.companies_to_be_removed.push_back(AcquiredBlock.c);
	}

	me.removedTheMergedCompanyFromAllBlocks( allblocks );
	/*
	for( auto it2=me.companies_to_be_removed.begin(); it2 != me.companies_to_be_removed.end(); ++it2 ){
		for( auto it=allblocks.begin(); it != allblocks.end(); ++it ){//clear the acquired block from the table
			if( it->c == *it2 ){
				allblocks.erase(it);
				break;
			}
		}
	}
	*/

}

void Game::doAcquireOnce( Block& AcquiringBlock, Block& AcquiredBlock, ATile& via ){
	vector<Player* > shareholders;
	COMPANY stakecompany = AcquiredBlock.c;
	for( unsigned int i=0; i<players.size(); i++ ){
		Player& pp = *players[i];
		if( pp.hasStock( stakecompany ) ){
			shareholders.push_back( &pp );
		}
	}

	allocateBonusFor(  AcquiredBlock.c, shareholders );
	askPlayersToSellStock( shareholders );
	askPlayersToConvertStock( shareholders );
	stocktable.markCompanyAvailable( stakecompany, 1 );

	AcquiringBlock.mergeWith( AcquiredBlock );
	AcquiringBlock.addATile( via );
	
}

void Game::allocateBonusFor( COMPANY c , const vector<Player*> shs ){
	vector<Player*> sh1;
	vector<Player*> sh2;
	vector<int> stockamts;

	
	for( unsigned int i=0; i<shs.size(); i++ ){
		Player* pp = shs[i];
		int ppsharecount = pp->getStockAmt( c );
		int index = 0;
		for( unsigned int j=0; j<stockamts.size(); j++ ){
			if( stockamts[j] == ppsharecount ){
				index = -1;
				break;
			}
			if( stockamts[j] < ppsharecount ){
				index = j;
				break;
			}
		}
		if( index >= 0 ) stockamts.insert( stockamts.begin() + index , ppsharecount ) ;
	}
	for( unsigned int i=0; i<shs.size(); i++ ){//pretty sure there's better way to do this in just 1 pass TBD
		Player* pp = shs[i];
		int ppstkamt = pp->getStockAmt( c );
		if( ppstkamt == stockamts[0] ) sh1.push_back( pp );
		if( stockamts.size() > 1 && ppstkamt == stockamts[1] ) sh2.push_back( pp );
	}

	if( sh1.size() > 1 ){//there will be no minor bonus allocated
		int cashshare = stocktable.majorbonus[ c ] / sh1.size() ;
		for_each( sh1.begin(), sh1.end(), [&cashshare]( Player* sh ){
			sh->debitCash( cashshare );
			sprintf_s( info, "PLAYER[%s] shared [%d] major share bonus", sh->toString().c_str(), cashshare);
			cout << info << endl;
		});
	}
	else if( sh1.size() == 1 ){
		sh1[0]->debitCash( stocktable.majorbonus[ c ] );
		int cashshare = stocktable.majorbonus[ c ];
		sprintf_s( info, "PLAYER[%s] get [%d] major share bonus]", sh1[0]->toString().c_str(), cashshare);
		cout << info << endl;
		if( sh2.size() > 1 ){//there will be no minor bonus allocated
			int cashshare = stocktable.minorbonus[ c ] / sh2.size() ;
			for_each( sh1.begin(), sh1.end(), [&cashshare]( Player* sh ){
				sh->debitCash( cashshare );
				sprintf_s( info, "PLAYER[%s] shared [%d] minor share bonus]", sh->toString().c_str(), cashshare);
				cout << info << endl;
			});
		}
	}

}

bool Game:: isStupidHumanSTurn() const{
	int plen = players.size();
	return round % plen == 0;
}

void Game::runTheGameOneRound(){
	
	if( isStupidHumanSTurn() ){
		runTheGameOneRoundForStupidHuman();
	}
	else{
		runTheGameOneRoundForSmartAI();
	}
}

void Game::runTheGameOneRoundForSmartAI(){
	int plen = players.size();
	Player* player = players[round % plen] ;
	ATile pt = askPlayerToPlaceATile( player );
	current_ATile = pt;

	MergeEvent me( allblocks, pt );
	pme = &me;
	if( me.isValidMerger() ){
		doAcquire( me );
	}
	else if( me.isNewBlock() ){
		askPlayerToSetupCompany(player, pt);
		askPlayerToBuyStock(player);
	}
	else if( me.isAdjToOneBlock() ){
		Block* adjBlock = me.sorted_blocks[0];
		adjBlock->addATile( pt );
		askPlayerToBuyStock(player);
	}

	allocatePlayerOneATile( player );
	statistics();
	round++;
}

void Game::doAcquireForStupidHumanOnce(){
	vector<Block*>& blocks_will_be_merged = pme->sorted_blocks;
	Block& AcquiringBlock = *blocks_will_be_merged[0];
	Block& AcquiredBlock  = *blocks_will_be_merged[1];
	COMPANY stakecompany = AcquiredBlock.c;
	vector<Player* > shareholders;
			
	for( unsigned int i=0; i<players.size(); i++ ){
		Player& pp = *players[i];
		if( pp.hasStock( stakecompany ) ){
			shareholders.push_back( &pp );
		}
	}

	allocateBonusFor(  stakecompany, shareholders );
	askPlayersToSellStock( shareholders );
	askPlayersToConvertStock( shareholders );
	stocktable.markCompanyAvailable( stakecompany, 1 );

	AcquiringBlock.mergeWith( AcquiredBlock );
	AcquiringBlock.addATile( current_ATile );

	blocks_will_be_merged.erase( blocks_will_be_merged.begin()+1 );
	pme->companies_to_be_removed.push_back( stakecompany );
}

void Game::runTheGameOneRoundForStupidHuman(){
	int plen = players.size();
	Player* player = players[round % plen] ;

	if(  TILE_PLACED == stage ){
		ATile pt = askPlayerToPlaceATile( player );
		current_ATile = pt;
		pme = new MergeEvent( allblocks, pt ); 
		if( pme->isValidMerger() ){
			stage = TO_CONVERT_STOCK;
		}
		else if( pme->isNewBlock() ){
			stage = TO_SETUP_COMPANY;
		}
		else if( pme->isAdjToOneBlock() ){
			Block* adjBlock = pme->sorted_blocks[0];
			adjBlock->addATile( pt );
			stage = TO_BUY_STOCK;
		}
	}
	else if( COMPANY_SETUP == stage ){
		ATile tile = current_ATile;
		askPlayerToSetupCompany(player, tile);
		stage = TO_BUY_STOCK;
	}
	else if( STOCK_SOLD == stage ){
		askPlayersToConvertStock(players);
		stage = TO_BUY_STOCK;
	}
	else if( STOCK_BOUGHT == stage ){
		askPlayerToBuyStock(player);
		stage = ROUND_END;
	}
	else if( STOCK_CONVERTED == stage ){
		if( pme->isValidMerger() ){
			doAcquireForStupidHumanOnce();
			stage = TO_CONVERT_STOCK;
		}
		else{
			pme->removedTheMergedCompanyFromAllBlocks(allblocks);
			stage = ROUND_END;
		}
	}
	else if( ROUND_END == stage ){
		delete pme;
		allocatePlayerOneATile( player );
		statistics();
		round++;
		stage = TO_PLACE_TILE;
	}
}

void Game::runTheGame(){
	initPlayerWithATiles();
	while( !isEndOfGame() ){
		runTheGameOneRound();
	}
}

void Game::updateGameStageForStupidHuman( ){
	if( stage == TO_PLACE_TILE ){
		stage = TILE_PLACED;
		cout << "TO_PLACE_TILE -> TILE_PLACED" << endl;
	}
	else if( stage == TO_BUY_STOCK ){
		stage = STOCK_BOUGHT;
		cout << "TO_BUY_STOCK -> STOCK_BOUGHT" << endl;
	}
	else if( stage == TO_SETUP_COMPANY ){
		stage = COMPANY_SETUP;
		cout << "TO_SETUP_COMPANY -> COMPANY_SETUP" << endl;
	}
	else if( stage == TO_SELL_STOCK ){
		stage = STOCK_SOLD;
		cout << "TO_SELL_STOCK -> STOCK_SOLD" << endl;
	}
	else if ( stage == TO_CONVERT_STOCK ){
		stage = STOCK_CONVERTED;
		cout << "TO_CONVERT_STOCK -> STOCK_CONVERTED" << endl;
	}
}
void Game::runTheGameForAI(){
	initPlayerWithATiles();
	while( !isEndOfGame() ){
		runTheGameOneRoundForSmartAI();
	}
}

void Game::runTheGameForHuman(){
	initPlayerWithATiles();
	while( !isEndOfGame() ){
		runTheGameOneRoundForStupidHuman();
		updateGameStageForStupidHuman();
	}
}

void Game::statistics(){
	char board[HEIGH][WIDTH];
	memset( board, '.', sizeof( board ) );
	for( int i=0; i<allblocks.size(); i++ ){
		Block& b = allblocks[i];
		for( auto it=b.ATiles.begin(); it!=b.ATiles.end(); ++it ){
			board[ it->row ][ it->col ] = COMPANYNAME[b.c][0];
		}
	}
	board[current_ATile.row][current_ATile.col] = 'X';
	cout << "=============================Round:"<< round << "=================================" << endl;
	cout << "=============================Board Begins=================================" << endl;
	for( int i=0; i<HEIGH; i++ ){
		for( int j=0; j<WIDTH; j++ ){
			cout << board[i][j];
		}
		cout << endl;
	}
	cout << "==========================================================================" << endl;
	cout << "=============================Players Begins===============================" << endl;
	for( unsigned int i=0; i<players.size(); i++ ){
		cout << players[i]->toString() << endl;
	}
	cout << "==========================================================================" << endl;
}

GameStatus::GameStatus( ){}
const vector<ATile> GameStatus::getAvailableATiles() const{
	vector<ATile> aATiles = pbd->allATiles;
	return aATiles;
}

const vector<Block> GameStatus::getAllBlocks()const{
	vector<Block> ablks = pbd->allblocks;
	return ablks;
}
const vector<COMPANY> GameStatus::getAllAvailableCompanies()const{
	return pbd->stocktable.getAvailableCompanies();
}

const string GameStatus::toString()const{
	char info[1000]="GAME STATUS";
	return info;
}





