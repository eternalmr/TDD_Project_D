
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "ServerUI.h"
#include "ServerUIDoc.h"
#include "MainFrm.h"

#include "CSelectView.h"
#include "CDisplayView.h"
#include "CLogView.h"
#include "CClientDlg.h"
#include "CTaskDlg.h"
#include "CServerConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

	ON_MESSAGE(NM_TASK,	  ShiftPage)
	ON_MESSAGE(NM_CLIENT, ShiftPage)

	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_CONFIG_SET, &CMainFrame::OnSetConfig)
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
	:m_bWholeWndIsSplitted(FALSE)
	,m_bRightWndIsSplitted(FALSE)
	,m_pSelectView(nullptr)
	,m_pLogView(nullptr)
	,m_pDisplayView(nullptr)
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

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

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
	m_bWholeWndIsSplitted = m_WholeWindowSplitter.CreateStatic(this, 1, 2);     // 切分成左右两部分
	
	if (!m_bWholeWndIsSplitted)
		return m_bWholeWndIsSplitted;
	
	m_bRightWndIsSplitted = m_RightWindowSplitter.CreateStatic(
										&m_WholeWindowSplitter, 2, 1, // 将右侧分为上下两部分
										WS_CHILD | WS_VISIBLE, 
										m_WholeWindowSplitter.IdFromRowCol(0, 1));
	if (!m_bRightWndIsSplitted)
		return m_bRightWndIsSplitted;

	m_WholeWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(0,0), pContext);
	m_RightWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CDisplayView), CSize(0,0), pContext);
	m_RightWindowSplitter.CreateView(1, 0, RUNTIME_CLASS(CLogView), CSize(0,0), pContext);

	// 保存分割窗口的指针
	m_pSelectView = m_WholeWindowSplitter.GetPane(0, 0);
	m_pDisplayView = m_RightWindowSplitter.GetPane(0, 0);
	m_pLogView = m_RightWindowSplitter.GetPane(1, 0);

	return TRUE; 
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED) return;

	CRect rect;
	GetClientRect(&rect); //获得用户窗口的矩形坐标
	int w = rect.Width();
	int h = rect.Height();

	if (m_bWholeWndIsSplitted) {
		m_WholeWindowSplitter.SetColumnInfo(0, int(0.15*w), 10);
		m_WholeWindowSplitter.SetColumnInfo(1, int(0.85*w), 10);
		m_WholeWindowSplitter.SetRowInfo(0, h, 10);
	}

	if (m_bRightWndIsSplitted) {
		m_RightWindowSplitter.SetColumnInfo(0, int(0.85*w), 10);
		m_RightWindowSplitter.SetRowInfo(0, int(0.6*h), 10);
		m_RightWindowSplitter.SetRowInfo(1, int(0.4*h), 10);
	}

	RecalcLayout();
}

// 切换任务和节点页
LRESULT CMainFrame::ShiftPage(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetClientRect(&rect);
	int w = rect.Width();
	int h = rect.Height();

	CCreateContext Context;
	if (wParam == NM_TASK)
	{
		Context.m_pCurrentDoc = (CServerUIDoc*)GetActiveDocument();
		Context.m_pNewViewClass = RUNTIME_CLASS(CTaskDlg); //新建视图的动态创建类
		Context.m_pCurrentFrame = this;//当前框架窗口
		Context.m_pLastView = (CFormView *)m_RightWindowSplitter.GetPane(0, 0); //原先的视图类，通常在切分窗口的视图类中使用（splitterWnd）
		m_RightWindowSplitter.DeleteView(0, 0);
		m_RightWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CTaskDlg), CSize(int(0.85*w), int(0.6*h)), &Context);
		CTaskDlg *pNewView = (CTaskDlg*)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_RightWindowSplitter.SetActivePane(0, 0);
	}
	else if (wParam == NM_CLIENT)
	{
		Context.m_pCurrentDoc = (CServerUIDoc*)GetActiveDocument();
		Context.m_pNewViewClass = RUNTIME_CLASS(CClientDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView *)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.DeleteView(0, 0);
		m_RightWindowSplitter.CreateView(0, 0, RUNTIME_CLASS(CClientDlg), CSize(int(0.85*w), int(0.6*h)), &Context);
		CClientDlg *pNewView = (CClientDlg*)m_RightWindowSplitter.GetPane(0, 0);
		m_RightWindowSplitter.RecalcLayout();
		pNewView->OnInitialUpdate();
		m_RightWindowSplitter.SetActivePane(0, 0);
	}

	return 0;
}




void CMainFrame::OnClose()
{
	// 在此添加消息处理程序代码和/或调用默认值
	int ReturnID = MessageBox(TEXT("您确定要退出程序吗？"),
		TEXT("关闭server"), MB_OKCANCEL);

	//TODO: 判断是否有计算任务仍然在执行
	if (ReturnID == IDCANCEL) {
		return;
	}

	CFrameWnd::OnClose();
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// 在此处添加消息处理程序代码
	logger.m_bRun = FALSE;
	server.exit();
}


void CMainFrame::OnSetConfig()
{
	CServerConfig dlg;
	dlg.DoModal();
}
