#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Game\definitions.h"
#include <vector>
#include "AppMacros.h"
struct Game;
class ATileSprite;
class ATileLabel;

class AcquireScene : public cocos2d::CCLayer
{
	Game* pGame;
	//ATileSprite* ats[HEIGH][WIDTH];
	ATileLabel* ats[HEIGH][WIDTH];
	std::vector<CCNode*> pcon;
public:
    virtual bool init();  
	void initGameLogic();
	void initGameUI();

	void updateGameRender();
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuClickCallBack(CCObject* pSender);
   
    CREATE_FUNC(AcquireScene);
	//LAYER_NODE_FUNC(TouchableLayer);  
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){ return true; }
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
	
};

#endif // __HELLOWORLD_SCENE_H__