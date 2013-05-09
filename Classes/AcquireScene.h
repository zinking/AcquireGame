#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Game\definitions.h"
#include <vector>
#include "AppMacros.h"
#include "Popup.h"
struct Game;
struct Player;
class ATileSprite;
class ATileLabel;

class AcquireScene : public cocos2d::CCLayer
{
	Game* pGame;
	Player* pCur;
	//ATileSprite* ats[HEIGH][WIDTH];
	ATileLabel* ats[HEIGH][WIDTH];
	std::vector<cocos2d::CCSprite*> pcon;
public:
    virtual bool init();  
	void initGameLogic();
	void initGameUI();
	
	void showStatusPopup();

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
