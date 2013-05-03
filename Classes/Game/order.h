struct GameStatus;
struct Player;
struct Tile;
struct Game;

struct Order{
	virtual bool execute( Game* b, Player* p)const{ return false; }
	virtual bool isValid( const GameStatus& bs, const Player& from )const{ return false; }
	virtual string toString()=0;
};


struct PlaceTileOrder:Order{
	const Tile& t;
	PlaceTileOrder( const Tile& tt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct SellStockOrder:Order{
	enum COMPANY c;
	int  count;
	SellStockOrder( enum COMPANY cc, int amt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct ConvertStockOrder:Order{
	enum COMPANY c;
	int  count;
	ConvertStockOrder( enum COMPANY cc, int amt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct BuyStockOrder:Order{
	enum COMPANY c;
	int  count;
	BuyStockOrder( enum COMPANY cc, int amt );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};

struct SetupCompanyOrder:Order{
	const COMPANY c;
	SetupCompanyOrder( const COMPANY c );
	string toString();
	bool isValid( const GameStatus& gs, const Player& from )const;
	bool execute( Game* g, Player* p )const;
};











