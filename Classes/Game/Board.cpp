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


Tile::Tile( int r, int c ):row(r),col(c){}
Tile::Tile():row(0),col(0){}
bool Tile::operator==( const Tile& rhs )const{
	return row == rhs.row && col == rhs.col;
}

bool Tile::operator<(  const Tile& rhs )const{
	return (row  == rhs.row ? col < rhs.col : row < rhs.row );
}
string Tile::toString() const {
	sprintf_s(info,"TILE[%d,%d]",row,col);
	return info;
}

Block::Block( enum COMPANY cc ):c(cc){}
Block::Block(){}
bool Block::hasTile( const Tile&  t){
	return tiles.find(t) != tiles.end();
}

void Block::addTile( const Tile&  t){
	tiles.insert( t );
}
//why block would have no company setup at all?
bool Block::hasCompany(){
	return c != -1;
}

bool Block::operator==( const Block& rhs )const{
	return false;
};


void Block::mergeWith( Block other ){
	for( auto it=other.tiles.begin(); it!=other.tiles.end(); ++it ){
		tiles.insert(*it);
	}
}

string Block::toString()const{
	string tilesinfo;
	for( auto it=tiles.begin(); it!=tiles.end(); ++it){
		tilesinfo += it->toString() + " ";
	}
	sprintf_s(info,"BLOCK COMPANY[%s] WITH TILEs[%s]", COMPANYNAME[c], tilesinfo.c_str() );
	return info;
}

StockTable::StockTable(){
	//memset( stockprice, 200,  sizeof( stockprice ) );
	//stockprice[NUMBER_OF_STOCKS]={200,200,200,200,200,200,200};
	//memset( majorbonus, 2000, sizeof( majorbonus ) );
	//memset( minorbonus, 1500, sizeof( minorbonus ) );
	//memset( available,  1,    sizeof( available  ) );
	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		stockprice[i] = 200;
		majorbonus[i] = 2000;
		minorbonus[i] = 1500;
		available[i]  = 1; //meaning if the company could be setup when = 1 and stock not purchasable at mean time
		//stockavail[i] = 0;
	}
	//memset( stockprice, 200,  sizeof( stockprice ) );
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

void Player::getAllocatedTiles(const set<Tile>& t ){
	tiles = t;
}

void Player::addStock( enum COMPANY c, int count){
	stocks[c]+=count;
}

void Player::removeTile( const Tile& t ){
	tiles.erase( t );
}

void Player::addTile( const Tile& t ){
	tiles.insert( t );
}

bool Player::hasTile( const Tile& t )const {
	return tiles.find( t ) != tiles.end();
}

int Player::tileCount()const{
	return tiles.size();
}
bool Player::hasStock( const enum COMPANY c ) const{
	return stocks[c] > 0 ;
}

int Player::getStockAmt( const enum COMPANY c ) const{
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

MergeEvent::MergeEvent( vector<Block>& allblocks, const Tile& newTile ){
	int x = newTile.row;
	int y = newTile.col;
	//memset( adjblocks, 0, sizeof(adjblocks) );
	adjblocks[0]=0,adjblocks[1]=0,adjblocks[2]=0,adjblocks[3]=0;
	if( x-1>=0 )	adjblocks[0] = getBlockWithTile( allblocks, Tile(x-1,y));
	if( y-1>=0 )	adjblocks[1] = getBlockWithTile( allblocks, Tile(x,y-1));
	if( x+1<HEIGH ) adjblocks[2] = getBlockWithTile( allblocks, Tile(x+1,y));
	if( y+1<WIDTH ) adjblocks[3] = getBlockWithTile( allblocks, Tile(x,y+1));

	//vector<Block> sorted_blocks;
	for( int j=0; j<4; j++ ){
		/*
		if( adjblocks[j]!=0 && adjcompanies.find( adjblocks[j]->c ) == adjcompanies.end() ) {
			adjcompanies.insert( adjblocks[j]->c );
			int i=sorted_blocks.size() -1;
			for(  ; i>=0; i-- ){//insertion sort
				if( sorted_blocks[i]->tiles.size() < adjblocks[j]->tiles.size() ){
					sorted_blocks[i+1]= sorted_blocks[i];
				}
				else break;
			}
			//sorted_blocks[i+1] = adjblocks[j];
			sorted_blocks.insert( sorted_blocks.begin()+i+1,adjblocks[j] );
		}*/
		Block* ccb = adjblocks[j];
		if( ccb != 0 ){
			int index = 0;
			for( int i=0; i<sorted_blocks.size(); i++ ){
				if( ccb == sorted_blocks[i] ){
					index = -1 ;
					break;
				}
				if( ccb->tiles.size() < sorted_blocks[i]->tiles.size() ){
					index = i;
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

Block* MergeEvent::getBlockWithTile( vector<Block>& allblocks, Tile& newTile){
	int n = allblocks.size(), i=0;
	for( i=0; i<n; i++ ){
		Block& b = allblocks[i];
		if( b.hasTile( newTile ) ) return &b;
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

	
Game::Game( ){
	initGame();
	gs.pbd = this;
	round = 1;
}

void Game::initGame(){
	int i=0,j=0;
	for( i=0; i<WIDTH; i++ )
		for( j=0; j<HEIGH; j++ )
			alltiles.push_back( Tile(i,j) );
	random_shuffle ( alltiles.begin(), alltiles.end() );
	//StockPriceTable::setupStockPriceTable( stocks )
}

void Game::addPlayer( Player* player ){
	if( players.size() < MAX_PLAYER_SIZE ) players.push_back( player );
}

void Game::allocatePlayerOneTile( Player* p ){
	if( alltiles.size() > 0 ){
		p->addTile( alltiles.back() );
		alltiles.pop_back();
	}
}

void Game::initPlayerWithTiles(){
	vector<string> pids;
	for_each( players.begin(), players.end(),[&pids]( Player* p){
		pids.push_back(p->id);
	});
	for_each( players.begin(), players.end(),[&pids]( Player* p){
		p->pai->fyiPlayerInfos( pids );
	});

	for_each( players.begin(), players.end(),[this]( Player* p){
		for( int j=0; j<NUMBER_OF_TILES_EACH; j++ ){
			this->allocatePlayerOneTile(p);
		}
	});
}

bool Game::isEndOfGame(){
	bool allBlockGT11 = true;
	bool oneBlockGE41 = false;
	int companyNumber = 0;
	bool hasnotile = ( alltiles.size() == 0 );

	for_each( allblocks.begin(), allblocks.end(), [&allBlockGT11,&oneBlockGE41](Block& b){
		if( b.tiles.size() < 11 ) allBlockGT11 = false;
		else if( b.tiles.size() >= 41 ) oneBlockGE41 = true;
	});

	if( allblocks.size() == 0 ) allBlockGT11 = false;

	bool oneplayerhasnotile = true;
	for_each( players.begin(), players.end(), [&oneplayerhasnotile]( Player* p ){
		if( p->tileCount() > 0 ){
			oneplayerhasnotile=false;
		}
	});

	return oneBlockGE41 || allBlockGT11 || oneplayerhasnotile || hasnotile;
}

const Tile Game::askPlayerToPlaceTile(  Player* p ){
	const PlaceTileOrder od = p->pai->decidePlaceTile( gs );
	Tile result = od.t;
	if( od.execute( this, p ) ){
		for_each( players.begin(), players.end(), [&p,&od](Player* pp ){
			pp->pai->fyiPlayerPlaceTileOrder( p->id , od );
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

void Game::askPlayerToSetupCompany(  Player* p, const Tile& t ){
	SetupCompanyOrder od = p->pai->decideSetupCompany( gs );
	if( od.execute( this, p ) ){
		for_each( players.begin(), players.end(), [&p,&od](Player* pp ){
			pp->pai->fyiPlayerSetupCompanyOrder( p->id , od );
		});
	}

	Block nb(od.c);
	nb.addTile( t );
	allblocks.push_back( nb );
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

void Game::doAcquire( Block& AcquiringBlock, Block& AcquiredBlock, Tile& via ){
	gs.AcquiringBlock = AcquiringBlock;
	gs.AcquiredBlock  = AcquiredBlock;
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
	AcquiringBlock.addTile( via );
	for( auto it=allblocks.begin(); it != allblocks.end(); ++it ){//clear the acquired block from the table
		if( &(*it) == &AcquiredBlock){
			allblocks.erase(it);
			break;
		}
	}
	
	
}

void Game::allocateBonusFor( enum COMPANY c , const vector<Player*> shs ){
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
			sprintf_s( info, "PLAYER[%s] shared [%d] major share bonus]", sh->toString().c_str(), cashshare);
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

void Game::runTheGame(){

	initPlayerWithTiles();
	unsigned int i=0,j=0,k=0;
		
	while( !isEndOfGame() ){
		for( i=0; i<players.size(); i++ ){
			Player* player = players[i];
			Tile pt = askPlayerToPlaceTile( player );
			current_tile = pt;

			MergeEvent me( allblocks, pt );
			if( me.isValidMerger() ){
				vector<Block*>& blocks_will_be_merged = me.sorted_blocks;
				while( blocks_will_be_merged.size() > 1 ){
					Block& AcquiringBlock = *blocks_will_be_merged[0];
					Block& AcquiredBlock  = *blocks_will_be_merged[1];
					doAcquire( AcquiringBlock, AcquiredBlock, pt );
					blocks_will_be_merged.erase( find(blocks_will_be_merged.begin(), blocks_will_be_merged.end(), &AcquiredBlock ) );
				}
			}
			else if( me.isNewBlock() ){
				askPlayerToSetupCompany(player, pt);
				askPlayerToBuyStock(player);
			}
			else if( me.isAdjToOneBlock() ){
				Block* adjBlock = me.sorted_blocks[0];
				adjBlock->addTile( pt );
				askPlayerToBuyStock(player);
			}

			allocatePlayerOneTile( player );
			statistics();
			round++;
		}
	}
		
}

void Game::statistics(){
	char board[WIDTH][HEIGH];
	memset( board, '.', sizeof( board ) );
	for( int i=0; i<allblocks.size(); i++ ){
		Block& b = allblocks[i];
		for( auto it=b.tiles.begin(); it!=b.tiles.end(); ++it ){
			board[ it->row ][ it->col ] = COMPANYNAME[b.c][0];
		}
	}
	board[current_tile.row][current_tile.col] = 'X';
	cout << "=============================Round:"<< round << "=================================" << endl;
	cout << "=============================Board Begins=================================" << endl;
	for( int i=0; i<WIDTH; i++ ){
		for( int j=0; j<HEIGH; j++ ){
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
const vector<Tile> GameStatus::getAvailableTiles() const{/*TBD*/
	vector<Tile> atiles = pbd->alltiles;
	return atiles;
}

const vector<Block> GameStatus::getAllBlocks()const{
	vector<Block> ablks = pbd->allblocks;
	return ablks;
}
const vector<COMPANY> GameStatus::getAllAvailableCompanies()const{
	return pbd->stocktable.getAvailableCompanies();
}

const string GameStatus::toString()const{
	char info[1000];
	return info;
}





