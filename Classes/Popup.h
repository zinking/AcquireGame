#ifndef ACQUIRE_POPUP_H
#define ACQUIRE_POPUP_H

#include "cocos2d.h"

USING_NS_CC;

class Popup : public CCLayerColor
{
	ccColor4B bgColor;
	ccColor3B txtColor;
	std::string text;

	CCMenu *menu;
    bool m_bTouchedMenu;

	public:
    Popup();
    ~Popup();
    
    virtual bool init();
    void initDialog();
    
	void setColor(const ccColor4B& bgColor, const ccColor3B& txtColor);
	void setText(const char* text);

    void onEnter();
    void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
	NODE_FUNC(Popup);
    void okMenuItemCallback(CCObject *pSender);
    void cancelMenuItemCallback(CCObject *pSender);
};

#endif // !ACQUIRE_POPUP

