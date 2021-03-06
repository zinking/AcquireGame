#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("AcquireGame");
    //eglView->setFrameSize(2048, 1536);
	eglView->setFrameSize(1280, 720);
	//eglView->setFrameSize(480, 320);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    //eglView->setFrameZoomFactor(0.4f);
	//eglView->setFrameZoomFactor(480.0f/1280.0f);
    return CCApplication::sharedApplication()->run();
}
