#include "ATileSprite.h"
#include "Game\Board.h"
#include "AppMacros.h"
#include "ccMacros.h"
USING_NS_CC;


ATileSprite* ATileSprite::create(){
	ATileSprite *pSprite = new ATileSprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
void ATileSprite::initTile( int rr, int cc, short sz ){
	r = rr;
	c = cc;
	size = sz;
	CCTexture2D *tex;

	char row= 'A'+r;
    char caption[100];
	sprintf_s( caption,"%c%d",row,c+1);
    tex = new CCTexture2D();
    tex->initWithString( caption,
                        "Helvetica",
                        size/4 * CC_CONTENT_SCALE_FACTOR(),
                        CCSize(size*1.5,size*1.5), 
                        kCCTextAlignmentCenter,
                        kCCVerticalTextAlignmentCenter);
	
    this->setTexture(tex);
    tex->release();
	
	CCRect rect = CCRectZero;
    rect.size = m_pobTexture->getContentSize();
    this->setTextureRect(rect);

}

void ATileSprite::draw(void)
{
	CCSprite::draw();
	int hfsz = size/2;
	CCPoint vt[4] = {ccp(-hfsz, -hfsz), ccp(+hfsz, -hfsz), ccp(+hfsz, +hfsz), ccp(-hfsz, +hfsz)};
	glLineWidth(1); 
    //glColor4ub(0, 0, 255, 255);
	ccDrawColor4B( 0, 0, 255, 255 );
	ccDrawPoly( vt, 4, true); 

}
void ATileLabel::draw(){
	CCNode::draw();
	CCSize s = label->boundingBox().size;
	ccDrawColor4B( 0, 102, 255, 255 );
	//ccPointSize(3);
    ccDrawRect(label->boundingBox().origin, ccpAdd(label->boundingBox().origin, (ccp(s.width,s.height))));
}
void ATileLabel::initTile( int rr, int cc ){
	char row= 'A'+rr;
    char caption[100];
	sprintf_s( caption,"%c%d",row,cc+1);
    label = CCLabelTTF::create( caption, "Arial", BLOCK_SIZE/2.5f, CCSize( BLOCK_SIZE,BLOCK_SIZE), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
	//label->initWithFile("dw.png");
	this->addChild( label );
}

extern char COMPANYSHORTNAME[NUMBER_OF_STOCKS][2];
extern ccColor3B COMPANYCOLOR[NUMBER_OF_STOCKS];
extern ccColor4B COMPANYCOLOR4[NUMBER_OF_STOCKS+1];

void ATileLabel::updateCaption( COMPANY c ){
	//char COMPANYSHORTNAME[NUMBER_OF_STOCKS][2]={"W","S","F","I","A","C","T"};
	const char* name = ( c== EMPTY ? "?":COMPANYSHORTNAME[c] );
	label->setString( name );
	//label->setColor( COMPANYCOLOR[c] );
	ccColor4B color = ( c== EMPTY ? COMPANYCOLOR4[7]: COMPANYCOLOR4[c] );
    CCLayerColor *color_layer = CCLayerColor::create(color);
    color_layer->setPosition(ccp(0, 0)); 
    color_layer->setContentSize(CCSize(BLOCK_SIZE,BLOCK_SIZE)); 
	label->removeAllChildren();
    label->addChild(color_layer, 0);

}