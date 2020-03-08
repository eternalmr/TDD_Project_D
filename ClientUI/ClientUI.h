
// ClientUI.h: ClientUI 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号
#include "CLogShow.h"

// CClientUIApp:
// 有关此类的实现，请参阅 ClientUI.cpp
//

class CClientUIApp : public CWinApp
{
public:
	CClientUIApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	//CClientTest &client;
};

extern CClientUIApp theApp;
