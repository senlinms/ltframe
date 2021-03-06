#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
#include "UIContextMenu.h"
UIClass* UIClassC=0;
DEFINEFUNCTION DragWindowFun(LTExecState* es)
{
	UIClassC->DragWindow();
	return JsUndefined();
}

DEFINEFUNCTION QuitAppFun(LTExecState* es)
{
	UIClassC->QuitApp();
	return JsUndefined();
}
DEFINEFUNCTION MinWindowFun(LTExecState* es)
{
	UIClassC->MinWindow();
	return JsUndefined();
}

DEFINEFUNCTION CreateMenuFun(LTExecState* es)
{
	int x = JsValue2Int(es,GetJsParameter(es,0));
	int y = JsValue2Int(es,GetJsParameter(es,1));
	int cx = JsValue2Int(es,GetJsParameter(es,2));
	int cy = JsValue2Int(es,GetJsParameter(es,3));
    UIClassC->CreateMenu(x,y,cx,cy);
	return JsUndefined();
}
UIClass::UIClass()
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 700,wh=500;
	OleInitialize(0);


	ltskinview= new LTFrameView(this,L"LTFrame-ModelWindow",NULL,WS_POPUP | WS_VISIBLE,0,0,ww,wh,0);
	UIClassC =this;
	ltskinview->BindUserFunction("DragWindow",DragWindowFun,0);
	ltskinview->BindUserFunction("QuitApp",QuitAppFun,0);
	ltskinview->BindUserFunction("MinWindow",MinWindowFun,0);
	ltskinview->BindUserFunction("CreateMenu",CreateMenuFun,4);

	SetWindowText(ltskinview->windowHandle(),L"LTFrame-ModelWindow");	


	wstring path = wstring(applicationpath)+L"./template/ModelWindow.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	ltskinview->MessageLoop();
}


UIClass::~UIClass(void)
{
	OleUninitialize();
	free(applicationpath);
	delete ltskinview;
}
void UIClass::DragWindow()
{
	ReleaseCapture();
	POINT DownPoint;
	GetCursorPos(&DownPoint);
	SendMessage(ltskinview->windowHandle(),WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(DownPoint.x,DownPoint.y));
}
void UIClass::CreateMenu(int x,int y,int cx,int cy)
{
	UIContextMenu * uic =new UIContextMenu();
	uic->CreateContextMenu(ltskinview->windowHandle(),x,y,cx,cy);
}

void UIClass::QuitApp()
{
	ltskinview->CloseWindow();
}
void UIClass::MinWindow()
{
	::ShowWindow(ltskinview->windowHandle(),SW_MINIMIZE);	
}
LRESULT UIClass::HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);

}
LPCWSTR UIClass::GetWindowClassName()
{
	return L"Plugin";
}
