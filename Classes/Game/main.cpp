#include "Board.h"
#include "PlayerAI.h"
#include "definitions.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;
void test_assert( const char* msg, int expected, int actual ){
	char buf[1000];
	sprintf_s(buf,"%s: Expected[%d], Actual[%d]", msg, expected, actual );
	cout << buf << endl;
	if( expected != actual ){
		exit(1);
	}
}
void test_placeADJATile(){
	{
		//TEST 0 ?
		Game g;
		MergeEvent me( g.allblocks, ATile(0,0) );
		test_assert("TEST0 ALLBLOCKS BEFORE  ", 0, g.allblocks.size() );
		me.setupCompany( EMPTY );
		test_assert("TEST0 ALLBLOCKS AFTER   ", 1, g.allblocks.size() );
		cout << endl;
	}
	{
		//TEST 0.1 ?X => WW
		Game g;
		Block b2;
		b2.addATile( ATile(0,0));
		b2.c = EMPTY;
		g.allblocks.push_back( b2 );
		MergeEvent me( g.allblocks, ATile(0,1) );
		test_assert("TEST0.1 ALLBLOCKS BEFORE  ", 1, g.allblocks.size() );
		test_assert("TEST0.1 BLOCK COMPANY BEFORE  ", EMPTY, g.allblocks[0].c );
		me.setupCompany( WORLDWIDE );
		test_assert("TEST0.1 ALLBLOCKS AFTER   ", 1, g.allblocks.size() );
		test_assert("TEST0.1 BLOCK COMPANY AFTER   ", WORLDWIDE, g.allblocks[0].c );
		cout << endl;
	}
	{
		//         0?      W 
		//TEST 0.2 ?X? => WWW
		//          ?      W
		Game g;
		Block b2,b3,b4,b5;
		b2.addATile( ATile(0,1));
		b3.addATile( ATile(1,2));
		b4.addATile( ATile(2,1));
		b5.addATile( ATile(1,0));
		b2.c = EMPTY;b3.c = EMPTY; b4.c=EMPTY; b5.c= EMPTY;
		g.allblocks.push_back( b2 );
		g.allblocks.push_back( b3 );
		g.allblocks.push_back( b4 );
		g.allblocks.push_back( b5 );
		MergeEvent me( g.allblocks, ATile(1,1) );
		test_assert("TEST0.2 ALLBLOCKS BEFORE  ", 4, g.allblocks.size() );
		test_assert("TEST0.2 BLOCK COMPANY BEFORE  ", EMPTY, g.allblocks[0].c );
		test_assert("TEST0.2 ISOLAT BLOKS, before ", 4, me.isolat_blocks.size() );
		me.setupCompany( WORLDWIDE );
		test_assert("TEST0.2 ISOLAT BLOKS, after  ", 0, me.isolat_blocks.size() );
		test_assert("TEST0.2 ALLBLOCKS AFTER   ", 1, g.allblocks.size() );
		test_assert("TEST0.2 BLOCK COMPANY AFTER   ", WORLDWIDE, g.allblocks[0].c );
		cout << endl;
	}

	{
		//TEST 1 WWX=> WWW
		Block b;
		b.addATile( ATile(0,0) );
		b.addATile( ATile(0,1) );
		b.c = WORLDWIDE;
		Game g;
		g.allblocks.push_back( b );
		MergeEvent me( g.allblocks, ATile(0,2) );
		test_assert("TEST1 BLOCK BEFORE ", 2, g.allblocks[0].ATiles.size() );
		test_assert("TEST1 SORTED BLOKS ", 1, me.sorted_blocks.size() );
		test_assert("TEST1 ISOLAT BLOKS ", 0, me.isolat_blocks.size() );
		me.mergeNewTileWithExistingCompany();
		test_assert("TEST1 BLOCK AFTER  ", 3, g.allblocks[0].ATiles.size() );
		cout << endl;
	}
	
	{
		//TEST 2 WWX=> WWW
		//         ?     W 
		Block b;
		b.addATile( ATile(0,0) );
		b.addATile( ATile(0,1) );
		b.c = WORLDWIDE;
		Block b2;
		b2.addATile( ATile(1,2));
		b2.c = EMPTY;
		Game g;
		g.allblocks.push_back( b );
		g.allblocks.push_back( b2 );
		MergeEvent me( g.allblocks, ATile(0,2) );
		test_assert("TEST2 BLOCK BEFORE ", 2, g.allblocks[0].ATiles.size() );
		test_assert("TEST2 SORTED BLOKS ", 1, me.sorted_blocks.size() );
		test_assert("TEST2 ISOLAT BLOKS ", 1, me.isolat_blocks.size() );
		test_assert("TEST2 ALLBLOCKS BEFORE  ", 2, g.allblocks.size() );
		me.mergeNewTileWithExistingCompany();
		test_assert("TEST2 BLOCK AFTER  ", 4, g.allblocks[0].ATiles.size() );
		test_assert("TEST2 ALLBLOCKS AFTER  ", 1, g.allblocks.size() );
		cout << endl;
	}

	{
		//         ?      W  
		//TEST 3 WWX?=> WWWW
		//         ?      W 
		Block b;
		b.addATile( ATile(1,0) );
		b.addATile( ATile(1,1) );
		b.c = WORLDWIDE;
		Block b2;
		b2.addATile( ATile(1,3));
		b2.c = EMPTY;
		Block b3;
		b3.addATile( ATile(0,2));
		b3.c = EMPTY;
		Block b4;
		b4.addATile( ATile(2,2));
		b4.c = EMPTY;
		Game g;
		g.allblocks.push_back( b );
		g.allblocks.push_back( b2 );
		g.allblocks.push_back( b3 );
		g.allblocks.push_back( b4 );
		MergeEvent me( g.allblocks, ATile(1,2) );
		test_assert("TEST3 BLOCK BEFORE ", 2, g.allblocks[0].ATiles.size() );
		test_assert("TEST3 SORTED BLOKS ", 1, me.sorted_blocks.size() );
		test_assert("TEST3 ISOLAT BLOKS ", 3, me.isolat_blocks.size() );
		test_assert("TEST3 ALLBLOCKS BEFORE  ", 4, g.allblocks.size() );
		me.mergeNewTileWithExistingCompany();
		test_assert("TEST3 BLOCK AFTER  ", 6, g.allblocks[0].ATiles.size() );
		test_assert("TEST3 ALLBLOCKS AFTER  ", 1, g.allblocks.size() );
		cout << endl;
	}
	{
		// WWW
		//   XCC
		Game g;
		Block b4;
		b4.addATile( ATile( 0, 0 ) );
		b4.addATile( ATile( 0, 1 ) );
		b4.addATile( ATile( 0, 2 ) );
		b4.c = WORLDWIDE;
		g.allblocks.push_back( b4 );
		Block b5;
		b5.addATile( ATile( 1, 3 ) );
		b5.addATile( ATile( 1, 4 ) );
		b5.c = SACKSON;
		g.allblocks.push_back( b5 );
		MergeEvent me4( g.allblocks, ATile( 1,2 ) );
		//cout << "BK Expected 2 Actual:" << me4.sorted_blocks.size() << endl;
		//cout << "BK Expected 1 Actual:" << me4.sorted_blocks.size() << endl;
		test_assert("TEST4 ALLBLOCKS BEFORE  ", 2, g.allblocks.size() );
		me4.mergeCompanyWithCompany();
		me4.cleanUp();
		test_assert("TEST4 ALLBLOCKS AFTER   ", 1, g.allblocks.size() );
	}

	{
		//WWW    WWW
		//WxW => WWW
		//WWW    WWW
		Game g;
		Block b3;
		b3.addATile( ATile( 0, 0 ) );
		b3.addATile( ATile( 0, 1 ) );
		b3.addATile( ATile( 0, 2 ) );
		b3.addATile( ATile( 1, 2 ) );
		b3.addATile( ATile( 2, 2 ) );
		b3.addATile( ATile( 2, 1 ) );
		b3.addATile( ATile( 2, 0 ) );
		b3.addATile( ATile( 1, 0 ) );
		b3.c = WORLDWIDE;
		g.allblocks.push_back( b3 );
		MergeEvent me3( g.allblocks, ATile( 1,1 ) );
		cout << "Expected 1 Actual:" << me3.sorted_blocks.size() << endl;
		cout << "Expected 4 equivalent address:" 
			 << me3.adjblocks[0] << " " 
			 << me3.adjblocks[1] << " "
			 << me3.adjblocks[2] << " "
			 << me3.adjblocks[3] << endl;
	}
		
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

	//g.runTheGameForAI();
	//g.runTheGameForHuman();
	g.runTheGameLoop();

	system("pause");

	return 0;

	return 0;
}