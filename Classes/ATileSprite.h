#pragma once
#include "cocos2d.h"
#include "Game\definitions.h"
#include <string>
struct ATile;
class ATileSprite :public cocos2d::CCSprite{
	int r;
	int c;
	COMPANY cc;
	short size;
public:
	~ATileSprite(void){}
	void initTile( int rr, int cc, short size );
	static ATileSprite* create();
	void draw();
};

class ATileLabel:public cocos2d::CCNode{
public:
	cocos2d::CCLabelTTF* label;
	CREATE_FUNC(ATileLabel);
	void draw();
	void updateCaption( COMPANY c );
	void initTile( int rr, int cc );
};

