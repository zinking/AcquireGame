#pragma once

static const int WIDTH = 9;
static const int HEIGH = 6;
static const int NUMBER_OF_STOCKS=5;
static const int MAXNUMBER_OF_STOCKS=7;
static const int NUMBER_OF_ATileS_EACH=6;
static const int BLOCK_SIZE = 40;
static const int FONT_SIZE = 12;
static const int MAXAI = 2;
static const int NUM_STOCK_PRICE=8;

typedef unsigned int UINT;

#define sprintf_s sprintf

//typedef enum{	WORLDWIDE,	SACKSON,	FESTIVAL,	IMPERIAL,	AMERICAN,	CONTINENTAL,	TOWER} COMPANY;
enum COMPANY{	WORLDWIDE,	SACKSON,	FESTIVAL,	IMPERIAL,	AMERICAN,	CONTINENTAL,	TOWER, EMPTY = -1} ;
enum GAMESTAGE{ TO_PLACE_TILE,TILE_PLACED, TO_SETUP_COMPANY,COMPANY_SETUP, 
	TO_BUY_STOCK, STOCK_BOUGHT, TO_SELL_STOCK, STOCK_SOLD, TO_CONVERT_STOCK, STOCK_CONVERTED, ROUND_END, IN_OPERATION };
