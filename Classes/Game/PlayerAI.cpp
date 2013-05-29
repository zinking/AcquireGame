#include "PlayerAI.h"
#include "order.h"
#include "Board.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <cmath>
#include <algorithm>

//AI cannot change PLAYER status
const ConvertStockOrder DefaultAI::decideDoStockConversion(const GameStatus& bs ){
	COMPANY c = bs.getAcquiredBlock()->c;
	ConvertStockOrder od( c , pplayer->getStockAmt( c ) );
	return od;
}
const PlaceATileOrder DefaultAI::decidePlaceATile( const GameStatus& bs ){
	const ATile& t = *( pplayer->ATiles.begin() );
	PlaceATileOrder od( t );
	return od;
}
const BuyStockOrder DefaultAI::decideBuyStocks( const GameStatus& bs){
	srand (time(NULL));
	COMPANY c = (COMPANY)(rand()%NUMBER_OF_STOCKS);
	BuyStockOrder od( c, 1 );
	return od;
}
const SellStockOrder DefaultAI::decideSellStock( const GameStatus& bs){
	srand (time(NULL));
	COMPANY c = (COMPANY)(rand()%NUMBER_OF_STOCKS);
	SellStockOrder od( c, 1 );
	return od;
}
const SetupCompanyOrder DefaultAI::decideSetupCompany( const GameStatus& bs ){
	vector<COMPANY> cs = bs.getAllAvailableCompanies() ;
	COMPANY c;
	cs.size() > 0 ? c=cs[0]:c=(COMPANY)0;
	SetupCompanyOrder od(c);
	return od;
}

////////////////////////////////////////////////////////////////////////////////////////
//AI cannot change PLAYER status

ZHENWStockPreferenceComparator::ZHENWStockPreferenceComparator( const GameStatus& bs ){
	
	pGame = &bs;
	
	UINT max_size = 0;
	const vector<Block>& allbocks = pGame->getAllBlocks();
	Block pbiggest;
	for( auto it=allbocks.begin(); it!=allbocks.end(); ++it){
		if( it->ATiles.size() > max_size ){
			max_size = it->ATiles.size();
			pbiggest = *it;
		}
	}
}
//const Block ZHENWStockPreferenceComparator::getBiggestBlock() const{
//	UINT max_size = 0;
//	const vector<Block>& allbocks = pGame->getAllBlocks();
//	Block pbiggest;
//	for( auto it=allbocks.begin(); it!=allbocks.end(); ++it){
//		if( it->ATiles.size() > max_size ){
//			max_size = it->ATiles.size();
//			pbiggest = *it;
//		}
//	}
//	return pbiggest;
//}
bool ZHENWStockPreferenceComparator::operator()( const Block& b1, const Block& b2 ){
	int b1price = pGame->getStockPrice( b1.c );
	int b2price = pGame->getStockPrice( b2.c );
	//Block biggest = getBiggestBlock();
	Block* pbiggest = &biggest;
	int d1 = ZHENWAI::calculateBlock2BlockDistance(b1,*pbiggest);
	int d2 = ZHENWAI::calculateBlock2BlockDistance(b2,*pbiggest);

	return d1==d2 ? (b1price<b2price):(d1<d2);
}

int ZHENWAI::calculateTile2BlockDistance( const ATile& t, const Block& b ){
	int mindist = INT_MAX;
	for( auto it= b.ATiles.begin(); it!=b.ATiles.end(); ++it ){
		int dist = abs( t.row - it->row ) + abs( t.col -it->col );
		mindist = min( dist, mindist );
	}
	return mindist;
}
int ZHENWAI::calculateBlock2BlockDistance( const Block& b1, const Block& b2 ){
	int mindist = INT_MAX;
	for( auto it = b1.ATiles.begin(); it!=b1.ATiles.end(); ++it ){
		int dist = ZHENWAI::calculateTile2BlockDistance( *it, b2 );
		mindist = min( dist, mindist );
	}
	return mindist;
}

bool ZHENWAI::isTileCreatingNewCompany( const ATile& t, const GameStatus& bs ){
	const vector<Block>& allbocks = bs.getAllBlocks();
	for( auto it=allbocks.begin(); it!=allbocks.end(); ++it){
		if( it->c == EMPTY && (calculateTile2BlockDistance(t,*it)==1) ){
			return true;
		}
	}
	return false;
}

bool ZHENWAI::isTileNext2PlayerBlock( const ATile& t, const GameStatus& bs, const Player& p ){
	const vector<Block>& ab = bs.getAllBlocks();
	for( auto it = ab.begin(); it!=ab.end(); ++it ){
		if( it->c == EMPTY || !p.hasStock( it->c ) ) continue;
		if( calculateTile2BlockDistance(t,*it) == 1 ) return true;
	}
	return false;
}
const ConvertStockOrder ZHENWAI::decideDoStockConversion(const GameStatus& bs ){
	COMPANY c = bs.getAcquiredBlock()->c;
	ConvertStockOrder od( c , pplayer->getStockAmt( c ) );
	return od;
}
const PlaceATileOrder ZHENWAI::decidePlaceATile( const GameStatus& bs ){
	int maxrank = 0;
	ATile t = *pplayer->ATiles.begin();
	for( auto it=pplayer->ATiles.begin(); it!=pplayer->ATiles.end(); ++it ){
		ATile tt = *it;
		int rank = 0;
		if( isTileNext2PlayerBlock(tt,bs,*pplayer)) rank += 15;
		if( isTileCreatingNewCompany(tt,bs) ) rank += 5;

		if( rank > maxrank ){
			maxrank = rank;
			t = tt;
		}
	}
	PlaceATileOrder od( t );
	return od;
}
const BuyStockOrder ZHENWAI::decideBuyStocks( const GameStatus& bs){
	COMPANY c = WORLDWIDE;
	const vector<Block>& allblocks = bs.getAllBlocks();
	ZHENWStockPreferenceComparator cmp = ZHENWStockPreferenceComparator(bs);
	Block* pbiggest = &cmp.biggest;

	vector<Block> blocks;
	for( auto it=allblocks.begin(); it!=allblocks.end(); ++it ){
		if( it->c != EMPTY && it->c != pbiggest->c ){
			blocks.push_back( *it );
		}
	}

	if( blocks.size() == 0   ) {
		c = pbiggest == NULL ? WORLDWIDE:pbiggest->c;
		//c = cmp.pbiggest->c;
	}
	else{
		const Block ch = *min_element( blocks.begin(), blocks.end(), cmp );
		c = ch.c;
	}

	BuyStockOrder od( c, 3 );
	return od;
}
const SellStockOrder ZHENWAI::decideSellStock( const GameStatus& bs){
	for( int i=0; i<NUMBER_OF_STOCKS; i++ ){
		if( pplayer->stocks[i] > 0 ){
			COMPANY c = (COMPANY)i;
			return SellStockOrder( c, 1 );
		}
	}
	return SellStockOrder( WORLDWIDE, 1 );
}
const SetupCompanyOrder ZHENWAI::decideSetupCompany( const GameStatus& bs ){
	COMPANY  ec = WORLDWIDE;
	int max_price = 0;
	vector<COMPANY> cs = bs.getAllAvailableCompanies() ;
	for( auto it=cs.begin(); it!=cs.end(); ++it ){
		int p = bs.getStockPrice( *it );
		if( p > max_price ){
			ec = *it;
			max_price = p;
		}
	}

	SetupCompanyOrder od(ec);
	return od;
}
	
///////////////////////////////////////////////////////////////////////
bool HOMAI::isTilesConnected( const ATile& t1, const ATile& t2 ){
	if( t1.col == t2.col ){
		return 
			( t2.row >=1 && ( t1.row == t2.row-1 )) ||
			( t2.row <= WIDTH-2 && ( t1.row == t2.row+1 ));
	}
	else{
		return
			( t2.col >=1 && ( t1.col == t2.col-1 )) ||
			( t2.col <= WIDTH-2 && ( t1.col == t2.col+1 ));
	}
}
void HOMAI::rearrage_sequence(const GameStatus& bs){
	vector<ATile> original_sequence;
	for( auto it=pplayer->ATiles.begin(); it != pplayer->ATiles.end(); ++it ){
		original_sequence.push_back( *it );
	}
	set<ATile> connected_tiles ;
	for( int i=0; i<original_sequence.size(); i++ ){
		ATile& t1 = original_sequence[i];
		for( int j=i+1; j<original_sequence.size(); j++ ){
			ATile& t2 = original_sequence[j];
			if( isTilesConnected(t1,t2)){
				connected_tiles.insert( t1 );
				connected_tiles.insert( t2 );
			}
		}
	}

	set<ATile> tileCreatingNewCompany;
	set<ATile> tileEnhanceMyCompanies;

	for( int i=0; i<original_sequence.size(); i++ ){
		ATile& t1=original_sequence[i];
		vector<Block> allblocks = bs.getAllBlocks();
		for( auto it=allblocks.begin(); it!=allblocks.end(); ++it ){
			Block b = *it;
			if( b.hasCompany() ){
				if( pplayer->stocks[b.c] > 0 ){
					tileEnhanceMyCompanies.insert( t1 );
				}
			}

			for( auto it=b.ATiles.begin(); it!=b.ATiles.end(); ++it ){
				const ATile& t2 = *it;
				if( isTilesConnected( t1,t2) ){
					tileCreatingNewCompany.insert( t1 );
				}
			}
		}
	}

	//copy( connected_tiles.begin(), connected_tiles.end(), tileCreatingNewCompany.begin() );
	//copy( original_sequence.begin(), original_sequence.end(), tileCreatingNewCompany.begin() );
	//copy( tileEnhanceMyCompanies.begin(), tileEnhanceMyCompanies.end(), tileCreatingNewCompany.begin() );
	tileCreatingNewCompany.insert( connected_tiles.begin(), connected_tiles.end() );
	tileCreatingNewCompany.insert( original_sequence.begin(), original_sequence.end() );
		tileCreatingNewCompany.insert( tileEnhanceMyCompanies.begin(), tileEnhanceMyCompanies.end() );

	tile_sequence.empty();
	for( auto it=tileCreatingNewCompany.begin(); it!=tileCreatingNewCompany.end(); ++it ){
		tile_sequence.push( *it );
	}
}

int  HOMAI::lookup_companysize( const GameStatus& bs, COMPANY c ){
	vector<Block> allblocks = bs.getAllBlocks();
	for( auto it=allblocks.begin(); it!=allblocks.end(); ++it ){
		Block b = *it;
		if( b.c == c ){
			return b.ATiles.size(); 
		}
	}
	return 0;
}

const ConvertStockOrder HOMAI::decideDoStockConversion(const GameStatus& bs ){

	COMPANY c = bs.getAcquiredBlock()->c;
	COMPANY fc = bs.getAcquiringBlock()->c;
	int number = pplayer->stocks[c];
	if( number > 1 ){
		int company_size = lookup_companysize( bs, c );
		int fcompany_size = lookup_companysize( bs, fc ) + company_size + 1;
		// finalprice = STOCK_PRICE_TABLE.lookupprice( fc, fcsize).get_price();
		// availableshares = bs.getStocks()[fc]
		// toconvert = ( currentprice*2 <= finalprice ) || ( finalcompanize > 15 );
		/*
		   if( toConvert ){
			   while( number>1 && availableshares>0 ){
			   orders.add( new stockvoncersionorder(company) )
			   number -=2;
			   availableshares--;
			   }
			}
			while( number>0 ){
				orders.add( new sellstockorder(company) );
				number -= 1;
		    }
			order[] os = new order( orders.size() )
			orders.toarray(os);
			return os;
		*/
		
	}
	ConvertStockOrder od( c , pplayer->getStockAmt( c ) );
	return od;
}
const PlaceATileOrder HOMAI::decidePlaceATile( const GameStatus& bs ){
	rearrage_sequence(bs);
	ATile t = tile_sequence.front();
	tile_sequence.pop();
	PlaceATileOrder od( t );
	return od;
}
const BuyStockOrder HOMAI::decideBuyStocks( const GameStatus& bs){
	vector<COMPANY> c1 ;
	vector<COMPANY> c2 ;
	vector<Block> allblocks = bs.getAllBlocks();
	for( auto it=allblocks.begin(); it!=allblocks.end(); ++it ){
		Block b = *it;
		if( b.hasCompany() ){
			if( b.ATiles.size() <=6 ){
				c1.push_back( b.c );
			}
			else{
				c2.push_back( b.c );
			}
		}
	}

	//copy( c2.begin(), c2.end(), c1.begin() );
	c1.insert( c1.end(), c2.begin(), c2.end() );

	/*
	vector<BuyStockOrder> orders;
	if( c1.size() > 0 ){
	    bool toContinue = true;
		while( toContinue ){
		    if( orders.size() >= 3 ){
			     break;
		    }
			int index = (int) ( c1.size() * sqrt( random()*16/4) );
			int company = c1.get( index );
			int price = StockPriceTable.lookupPrice(company).getPrice();
			if( player.getcash() >= price ){
			    orders.add( new BuyStockOrder( company,1) );
		    }
			else{
			    break;
			}
		}
	}
	*/

	COMPANY c = c1.size() > 0 ? c1[0]:WORLDWIDE;

	BuyStockOrder od( c, 3 );
	return od;
}
const SellStockOrder HOMAI::decideSellStock( const GameStatus& bs){

	//return SellStockOrder( WORLDWIDE, 1 );
	return DefaultAI::decideSellStock(bs);
}
const SetupCompanyOrder HOMAI::decideSetupCompany( const GameStatus& bs ){

	return DefaultAI::decideSetupCompany(bs);
}

PlayerAI* AIFactory::getAI( AITYPE t, const char* name ){
	switch( t ){
	case NAIVE_AI:
		return new DefaultAI( name );
		break;
	case ZHEN_AI:
		return new ZHENWAI( name );
		break;
	case HOM_AI:
		return new HOMAI( name );
		break;
	}
	return NULL;
}
