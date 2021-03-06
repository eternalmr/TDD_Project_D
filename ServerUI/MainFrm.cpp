﻿
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "ServerUI.h"

#include "MainFrm.h"

#include "CSelectView.h"
#include "CDisplayView.h"
#include "CLogView.h"
#include "CClientDlg.h"
#include "CTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

	ON_MESSAGE(NM_TASK,	  OnMyChange)
	ON_MESSAGE(NM_CLIENT, OnMyChange)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	SetTitle(TEXT(" By 湖南麟淇"));
	//TODO : 这里的高度和宽度可能需要固定下来
	//int nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度，不包括下面任务栏    
	//int nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度，不包括下面任务栏

	//int nWidth  = GetSystemMetrics(SM_CXFULLSCREEN);
	//int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	int nWidth = 960;
	int nHeight = 600;

	MoveWindow(0, 0, nWidth, nHeight);//设置窗口大小
	CenterWindow();           //居中窗口

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	//cs.cx = 1440;
	//cs.cy = 900;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	//// TODO: 在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect); //获得用户窗口的矩形坐标
	//GetWindowRect(&rect);
	int w = rect.Width();
	int h = rect.Height();

	//CString str;
	//str.Format(TEXT("w = %d, h = %d"), w, h);
	//MessageBox(str);

	m_WholeWindowSplitter.CreateStatic(this, 1, 2);     // 切分成左右两部分
	m_RightWindowSplitter.CreateStatic(&m_WholeWindowSplitter, 2, 1, // 将右侧分为上下两部分
											WS_CHILD | WS_VISIBLE, 
										m_WholeWindowSplitter.IdFromRowCol(0, 1));

	m_WholeWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(0.15*w, h), pContext);//左侧是CSelectView的实例，大小为200X600
	m_RightWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CDisplayView), CSize(0.85*w, 0.6*h), pContext);//右侧是CDisplayView的实例，大小为760X600
	m_RightWindowSplitter.CreateView(1, 0, RUNTIME_CLASS(CLogView), CSize(0.85*w, 0.4*h), pContext);//右侧是CDisplayView的实例，大小为760X600
	
	m_WholeWindowSplitter.SetColumnInfo(0, 0.15*w, 100);
	m_WholeWindowSplitter.SetColumnInfo(1, 0.85*w, 100);
	m_WholeWindowSplitter.SetRowInfo(0, h, 100);

	m_RightWindowSplitter.SetColumnInfo(0, 0.85*w, 100);
	m_RightWindowSplitter.SetRowInfo(0, 0.6*h, 100);
	m_RightWindowSplitter.SetRowInfo(1, 0.4*h, 100);

	RecalcLayout();

	return TRUE; //不使用默认拆分
}

LRESULT CMainFrame::OnMyChange(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetClientRect(&rect);
	int w = rect.Width();
	int h = rect.Height();

	CCreateContext Context;
	if (wParam == NM_TASK)
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CTaskDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView *)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.DeleteView(0, 0);
		m_RightWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CTaskDlg), CSize(0.85*w, 0.6*h), &Context);
		CTaskDlg *pNewView = (CTaskDlg*)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_RightWindowSplitter.SetActivePane(0, 0);
	}
	else if (wParam == NM_CLIENT)
	{
		Context.m_pNewViewClass = RUNTIME_CLASS(CClientDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView *)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.DeleteView(0, 0);
		m_RightWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CClientDlg), CSize(0.85*w, 0.6*h), &Context);
		CClientDlg *pNewView = (CClientDlg*)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_RightWindowSplitter.SetActivePane(0, 0);
	}

	return 0;
}
