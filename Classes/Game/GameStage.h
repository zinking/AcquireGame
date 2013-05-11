#ifndef __GAME_STAGE_COMMAND_H__
#define __GAME_STAGE_COMMAND_H__

struct Player;
struct Game;

class GameCommand{
protected:
	Game* pGame;
	Player* pPlayer;
public:
	GameCommand( Player* p, Game* g):pGame(g),pPlayer(p){}
	virtual bool run()=0;
};

class PlaceTileCommand:public GameCommand{
public:
	PlaceTileCommand( Player* p, Game* g):GameCommand(p,g){}
	bool run();
};

class MergeBlockCommand:public GameCommand{
public:
	MergeBlockCommand( Player* p, Game* g):GameCommand(p,g){}
	bool run();
};

class SetupCommpanyCommand:public GameCommand{
public:
	SetupCommpanyCommand( Player* p, Game* g):GameCommand(p,g){}
	bool run();
};

class BuyStockCommand:public GameCommand{
public:
	BuyStockCommand( Player* p, Game* g):GameCommand(p,g){}
	bool run();
};

class SellStockCommand:public GameCommand{
public:
	SellStockCommand( Player* p, Game* g):GameCommand(p,g){}
	bool run();
};

class ConvertStockCommand:public GameCommand{
public:
	ConvertStockCommand( Player* p, Game* g):GameCommand(p,g){}
	bool run();
};


#endif