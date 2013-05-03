#include "Board.h"
#include "PlayerAI.h"
#include "definitions.h"
#include <iostream>
using namespace std;
void test_placeADJTile(){
	Block b;
	//enum COMPANY c=(COMPANY)0;
	b.addTile( Tile(0,0) );
	//b.c = (enum COMPANY)0;
	Game g;
	g.allblocks.push_back( b );
	MergeEvent me( g.allblocks, Tile(0,1) );
	//cout << << endl;
	cout << "Expected 1, Actual:" << me.sorted_blocks.size() << endl;
	Block b2;
	enum COMPANY c=(COMPANY)1;
	b2.addTile( Tile(1,1) );
	b2.addTile( Tile(2,1) );
	g.allblocks.push_back( b2 );
	MergeEvent me2( g.allblocks, Tile(0,1) );
	cout << "Expected 2, Actual:" << me2.sorted_blocks.size() << endl;
	cout << "Expected 1st Block Size:2 , Actual:"<< me2.sorted_blocks[0]->tiles.size() << endl;

	g.allblocks.clear();
	Block b3;
	b3.addTile( Tile( 0, 0 ) );
	b3.addTile( Tile( 0, 1 ) );
	b3.addTile( Tile( 0, 2 ) );
	b3.addTile( Tile( 1, 2 ) );
	b3.addTile( Tile( 2, 2 ) );
	b3.addTile( Tile( 2, 1 ) );
	b3.addTile( Tile( 2, 0 ) );
	b3.addTile( Tile( 1, 0 ) );
	g.allblocks.push_back( b3 );
	MergeEvent me3( g.allblocks, Tile( 1,1 ) );
	cout << "Expected 1 Actual:" << me3.sorted_blocks.size() << endl;
	cout << "Expected 4 equivalent address:" 
		 << me3.adjblocks[0] << " " 
		 << me3.adjblocks[1] << " "
		 << me3.adjblocks[2] << " "
		 << me3.adjblocks[3] << endl;
		

}
int main1(){
	test_placeADJTile();
	system("pause");
	return 0;
}

int main(){
	Game g;
	DefaultAI ai1("N1");
	DefaultAI ai2("N2");

	Player a( & ai1 );
	ai1.setPlayer( &a );

	Player b( & ai2 );
	ai2.setPlayer( &b );

	g.addPlayer( &a );
	g.addPlayer( &b );

	g.runTheGame();

	system("pause");

	return 0;
}