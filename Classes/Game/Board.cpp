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
#include "order.h"
#include "PlayerAI.h"
#include "definitions.h"
#include "GameStage.h"

#define MAX_PLAYER_SIZE 6
extern char info[200];
extern char COMPANYNAME[NUMBER_OF_STOCKS][20];

////////////////////////////////////////////////////////////////////////////////
ATile::ATile( int r, int c ):row(r),col(c){}
ATile::ATile():row(0),col(0){}
bool ATile::operator==( const ATile& rhs )const{
	return row == rhs.row && col == rhs.col;
}

bool ATile::operator<(  const ATile& rhs )const{
	return (row  == rhs.row ? col < rhs.col : row < rhs.row );
}
string ATile::toString() const {
	sprintf_s(info,"T[%d,%d]",row,col);
	return info;
}

string ATile::getCaption() const {
	sprintf_s(info,"%c%d",'A'+row,col+1);
	return info;
}
////////////////////////////////////////////////////////////////////////////////
Block::Block( COMPANY cc ):c(cc){}
Block::Block(){}
bool Block::hasATile( const ATile&  t){
	return ATiles.find(t) != ATiles.end();
}

void Block::addATile( const ATile&  t){
	ATiles.insert( t );
}

bool Block::hasCompany(){
	return c != EMPTY;
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
	if( c == EMPTY )
		sprintf_s(info,"BLOCK COMPANY[%s] WITH ATiles[%s]", "EMPTY", ATilesinfo.c_str() );
	else
		sprintf_s(info,"BLOCK COMPANY[%s] WITH ATiles[%s]", COMPANYNAME[c], ATilesinfo.c_str() );
	return info;
}
////////////////////////////////////////////////////////////////////////////////
StockTable::StockTable(){
	for( int i=0; i<NUM_STOCK_PRICE; i++ ){
		stockprice[i] = (i+2)*100;
		majorbonus[i] = (i+2)*1000;
		minorbonus[i] = (i+2)*500;
	}

	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		available[i]  = 1; 
		stockseed[i]  = rand() % NUM_STOCK_PRICE;
	}
}

const int StockTable::getStockPrice( const COMPANY& c, int size )const {
	int index = ( size / (( WIDTH*HEIGH)/NUM_STOCK_PRICE) + stockseed[c] ) % NUM_STOCK_PRICE;
	return stockprice[index];
}

const int StockTable::getStockMajor( const COMPANY& c, int size )const {
	int index = ( size / (( WIDTH*HEIGH)/NUM_STOCK_PRICE) + stockseed[c] ) % NUM_STOCK_PRICE;
	return majorbonus[index];
}

const int StockTable::getStockMinor( const COMPANY& c, int size )const {
	int index = ( size / (( WIDTH*HEIGH)/NUM_STOCK_PRICE) + stockseed[c] ) % NUM_STOCK_PRICE;
	return minorbonus[index];
}
////////////////////////////////////////////////////////////////////////////////
Player::Player( PlayerAI* pai ){
	this->id  = pai->getID();
	this->pai = pai;
	this->pai->setPlayer( this );
	initialize();
}

void Player::initialize(){
	this->cash = 6000;
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
		if( stocks[i] >= 0 ){
			sprintf_s(info,"%c(%d) ", COMPANYNAME[i][0],stocks[i] );
			shstr += info;
		}
	}
	sprintf_s( info, "%s(%d$) %s ", id.c_str(), cash,shstr.c_str() );
	return info;
}

////////////////////////////////////////////////////////////////////////////////
int GameStatus::getStockPrice( const COMPANY& c )const{
	return pbd->getStockPrice(c);
}
bool GameStatus::isCompanyAvailable( const COMPANY& c )const{
	if(c>=0 && c<=NUMBER_OF_STOCKS )return pbd->stocktable.available[c] != 0;
	return false;
}
const Block* GameStatus::getAcquiringBlock()const{
	return pbd->pme->acquiring;
}
const Block* GameStatus::getAcquiredBlock()const{
	return pbd->pme->acquired;
}


const string GameStatus::getLastestMessage() const{
	int len = pbd->messages.size();	
	return len > 0 ? pbd->messages[len-1] : "" ;
}

const int GameStatus::getRound() const{
	return pbd->round;
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

////////////////////////////////////////////////////////////////////////////////
MergeEvent::MergeEvent( vector<Block>& ab, const ATile& na ):allblocks(ab),newATile(na){
	int y = newATile.row;
	int x = newATile.col;
	adjblocks[0]=0,adjblocks[1]=0,adjblocks[2]=0,adjblocks[3]=0;
	if( x-1>=0 )	adjblocks[0] = getBlockWithATile( allblocks, ATile(y,x-1));
	if( y-1>=0 )	adjblocks[1] = getBlockWithATile( allblocks, ATile(y-1,x));
	if( x+1<WIDTH ) adjblocks[2] = getBlockWithATile( allblocks, ATile(y,x+1));
	if( y+1<HEIGH ) adjblocks[3] = getBlockWithATile( allblocks, ATile(y+1,x));

	for( int j=0; j<4; j++ ){
		Block* ccb = adjblocks[j];
		if( ccb != 0 && ccb->hasCompany() ){
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
		else if( ccb != 0 ){//isolated blocks are blocks with size 
			isolat_blocks.push_back( ccb );
		}
	}
}

Block* MergeEvent::getBlockWithATile( vector<Block>& allblocks, const ATile& newATile){
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
	return isolat_blocks.size() == 0 && sorted_blocks.size() == 0;
}
bool MergeEvent::isAdjToOneBlock()const{
	return sorted_blocks.size() == 1;
}
bool MergeEvent::isSetupBlock()const{
	return isolat_blocks.size() > 0 && sorted_blocks.size() == 0;
}

void MergeEvent::mergeNewTileWithExistingCompany(){
	Block* adjBlock = sorted_blocks[0];
	adjBlock->addATile( newATile );
	acquireIsolatedBlocks( *adjBlock );
	cleanUp();
}

void MergeEvent::mergeCompanyWithCompany(){
	//vector<Block*>& blocks_will_be_merged = pGame->pme->sorted_blocks;
	//Block& AcquiringBlock = *blocks_will_be_merged[0];
	//Block& AcquiredBlock  = *blocks_will_be_merged[1];
	acquiring = sorted_blocks[0];
	acquired  = sorted_blocks[1];
	acquiring->mergeWith( *acquired );
	acquiring->addATile( newATile );
	
	acquireIsolatedBlocks(*acquiring);
	sorted_blocks.erase( sorted_blocks.begin()+1 );
	companies_to_be_removed.push_back( acquired->toString() );
}
void MergeEvent::cleanUp(   ){
	for( auto it2=companies_to_be_removed.begin(); it2 != companies_to_be_removed.end(); ++it2 ){
		for( auto it=allblocks.begin(); it != allblocks.end(); ++it ){//clear the acquired block from the table
			if( it->toString() == *it2 ){
				allblocks.erase(it);
				break;
			}
		}
	}
}

void MergeEvent::acquireIsolatedBlocks( Block& acquiring ){
	//vector<string> blocks_to_removed;
	for( auto it=isolat_blocks.begin(); it != isolat_blocks.end(); ++it){
		acquiring.mergeWith( **it );
		companies_to_be_removed.push_back( (*it)->toString() );
	}
	isolat_blocks.clear();

	/*
	for( auto it2=blocks_to_removed.begin(); it2 != blocks_to_removed.end(); ++it2 ){
		for( auto it=allblocks.begin(); it != allblocks.end(); ++it ){//clear the acquired block from the table
			if( it->toString() == *it2 ){
				allblocks.erase(it);
				break;
			}
		}
	}
	*/
}

void MergeEvent::setupCompany(const COMPANY& c ){
	Block nb( c );
	nb.addATile( newATile );
	
	acquireIsolatedBlocks( nb );
	//the sequence here is important because the array stores addresses which will be destroyed after things get inserted into the table
	allblocks.push_back( nb );
	cleanUp();
	
}
////////////////////////////////////////////////////////////////////////////////
	
Game::Game( ){
	initGame();
	gs.pbd = this;
	round = 0;
}

void Game::initGame(){
	int i=0,j=0;
	for( i=0; i<HEIGH; i++ )
		for( j=0; j<WIDTH; j++ )
			allATiles.push_back( ATile(i,j) );
	random_shuffle ( allATiles.begin(), allATiles.end() );
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

	for_each( allblocks.begin(), allblocks.end(), [&allBlockGT11,&oneBlockGE41,&companyNumber](Block& b){
		if( b.hasCompany() && b.ATiles.size() < 11 ) allBlockGT11 = false;
		else if( b.ATiles.size() >= 41 ) oneBlockGE41 = true;
		else if ( b.hasCompany() ) companyNumber++;
	});

	if( companyNumber == 0 ) allBlockGT11 = false;

	bool oneplayerhasnoATile = true;
	for_each( players.begin(), players.end(), [&oneplayerhasnoATile]( Player* p ){
		if( p->ATileCount() > 0 ){
			oneplayerhasnoATile=false;
		}
	});

	return oneBlockGE41 || allBlockGT11 || oneplayerhasnoATile || hasnoATile;
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
		//int cashshare = stocktable.majorbonus[ c ] / sh1.size() ;
		int cashshare = getStockMajor(c) / sh1.size() ;
		for_each( sh1.begin(), sh1.end(), [&cashshare]( Player* sh ){
			sh->debitCash( cashshare );
			sprintf_s( info, "PLAYER[%s] shared [%d] major share bonus", sh->toString().c_str(), cashshare);
			cout << info << endl;
		});
	}
	else if( sh1.size() == 1 ){
		//sh1[0]->debitCash( stocktable.majorbonus[ c ] );
		int cashshare =  getStockMajor(c);
		sh1[0]->debitCash( cashshare );
		sprintf_s( info, "PLAYER[%s] get [%d] major share bonus]", sh1[0]->toString().c_str(), cashshare);
		cout << info << endl;
		if( sh2.size() > 1 ){//there will be no minor bonus allocated
			int cashshare = getStockMinor(c) / sh2.size() ;
			for_each( sh1.begin(), sh1.end(), [&cashshare]( Player* sh ){
				sh->debitCash( cashshare );
				sprintf_s( info, "PLAYER[%s] shared [%d] minor share bonus]", sh->toString().c_str(), cashshare);
				cout << info << endl;
			});
		}
	}

}

void Game::runTheGameLoop(){
	initPlayerWithATiles();
	while( !isEndOfGame() ){
		runTheGameOneLoop();
	}
}

void Game::runTheGameOneLoop(){
	if( commandqueue.size() > 0 ){
		GameCommand* pcommand = commandqueue.front();
		if( pcommand->run() ){
			delete pcommand;
			commandqueue.pop();
		}
	}
	else{
		statistics();
		round++;
		int plen = players.size();
		int pi = round%plen;
		Player* p = players[pi];
		addCommand( new PlaceTileCommand(p,this) );
	}
}

int Game::getStockPrice( const COMPANY& c )const{
	return stocktable.getStockPrice( c, getCompanySize(c) );
}
int Game::getStockMajor( const COMPANY& c )const{
	return stocktable.getStockMajor( c, getCompanySize(c) );
}
int Game::getStockMinor( const COMPANY& c )const{
	return stocktable.getStockMinor( c, getCompanySize(c) );
}

int Game::getCompanySize( const COMPANY& c ) const{
	for( unsigned int  i=0; i<allblocks.size(); i++ ){
		if( allblocks[i].c == c ) return allblocks[i].ATiles.size();
	}
	return 0;
}

void Game::statistics(){
	char board[HEIGH][WIDTH];
	memset( board, '.', sizeof( board ) );
	for( int i=0; i<allblocks.size(); i++ ){
		Block& b = allblocks[i];
		char symbol = b.hasCompany() ? COMPANYNAME[b.c][0]:'?';
		for( auto it=b.ATiles.begin(); it!=b.ATiles.end(); ++it ){
			board[ it->row ][ it->col ] = symbol;
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
////////////////////////////////////////////////////////////////////////////////