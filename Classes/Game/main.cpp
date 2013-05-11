#include "Board.h"
#include "PlayerAI.h"
#include "definitions.h"
#include <iostream>
using namespace std;
void test_placeADJATile(){
	Block b;
	b.addATile( ATile(0,0) );
	Game g;
	g.allblocks.push_back( b );
	MergeEvent me( g.allblocks, ATile(0,1) );
	cout << "Expected 1, Actual:" << me.sorted_blocks.size() << endl;
	Block b2;
	enum COMPANY c=(COMPANY)1;
	b2.addATile( ATile(1,1) );
	b2.addATile( ATile(2,1) );
	g.allblocks.push_back( b2 );
	MergeEvent me2( g.allblocks, ATile(0,1) );
	cout << "Expected 2, Actual:" << me2.sorted_blocks.size() << endl;
	cout << "Expected 1st Block Size:2 , Actual:"<< me2.sorted_blocks[0]->ATiles.size() << endl;

	g.allblocks.clear();
	Block b3;
	b3.addATile( ATile( 0, 0 ) );
	b3.addATile( ATile( 0, 1 ) );
	b3.addATile( ATile( 0, 2 ) );
	b3.addATile( ATile( 1, 2 ) );
	b3.addATile( ATile( 2, 2 ) );
	b3.addATile( ATile( 2, 1 ) );
	b3.addATile( ATile( 2, 0 ) );
	b3.addATile( ATile( 1, 0 ) );
	g.allblocks.push_back( b3 );
	MergeEvent me3( g.allblocks, ATile( 1,1 ) );
	cout << "Expected 1 Actual:" << me3.sorted_blocks.size() << endl;
	cout << "Expected 4 equivalent address:" 
		 << me3.adjblocks[0] << " " 
		 << me3.adjblocks[1] << " "
		 << me3.adjblocks[2] << " "
		 << me3.adjblocks[3] << endl;

	g.allblocks.clear();
	Block b4;
	b4.addATile( ATile( 0, 0 ) );
	b4.c = WORLDWIDE;
	g.allblocks.push_back( b4 );
	Block b5;
	b5.addATile( ATile( 1, 1 ) );
	b5.c = SACKSON;
	g.allblocks.push_back( b5 );
	MergeEvent me4( g.allblocks, ATile( 0,1 ) );
	cout << "BK Expected 2 Actual:" << me4.sorted_blocks.size() << endl;
	g.doAcquire( me4 );
	cout << "BK Expected 1 Actual:" << me4.sorted_blocks.size() << endl;
		
}
int main1(){
	test_placeADJATile();
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

	g.runTheGameForAI();
	//g.runTheGameForHuman();

	system("pause");

	return 0;

	return 0;
}