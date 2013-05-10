#include "Popup.h"

Popup::Popup()
{
	bgColor = ccc4(195, 217, 255, 200);
	txtColor = ccc3(255, 234, 0);
	text = "Some Info";
}

Popup::~Popup()
{
}

void Popup::setColor(const ccColor4B& bgColor, const ccColor3B& txtColor)
{
	this->bgColor = bgColor;
	this->txtColor = txtColor;
}

void Popup::setText(const char* text)
{
	this->text = text;
}

bool Popup::init()
{
	auto ready = false;
	do 
	{
		if(!CCLayerColor::initWithColor(bgColor))
			break;

		this->initDialog();
		ready = true;
	}while(0);
	return ready;
}

void Popup::initDialog()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCLabelTTF *label = CCLabelTTF::create(text.c_str(), "Arial", 20);
    label->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));
	label->setColor(txtColor);
    this->addChild(label);
    
	CCMenuItemFont::setFontName("Arial");
	CCMenuItemFont::setFontSize(20);

    okMenuItem = CCMenuItemFont::create("OK", this, menu_selector(Popup::okMenuItemCallback));
    okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 50));
	okMenuItem->setColor(txtColor);
    
    CCMenuItemFont *cancelMenuItem = CCMenuItemFont::create("Cancel", this, menu_selector(Popup::cancelMenuItemCallback));
    cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 50));
    cancelMenuItem->setColor(txtColor);

    menu = CCMenu::create(okMenuItem, cancelMenuItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);

	//okMenuItem->set
}


void Popup::onEnter()
{
    CCLayerColor::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -127, true);
}

void Popup::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool Popup::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_bTouchedMenu = menu->ccTouchBegan(pTouch, pEvent);
    
    return true;
}

void Popup::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bTouchedMenu) {
        menu->ccTouchMoved(pTouch, pEvent);
    }
}

void Popup::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bTouchedMenu) {
        menu->ccTouchEnded(pTouch, pEvent);
        m_bTouchedMenu = false;
    }
}

void Popup::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bTouchedMenu) {
        menu->ccTouchEnded(pTouch, pEvent);
        m_bTouchedMenu = false;
    }
}

void Popup::okMenuItemCallback(cocos2d::CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void Popup::cancelMenuItemCallback(cocos2d::CCObject *pSender)
{
    this->removeFromParentAndCleanup(false);
}