
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "ClientUI.h"

#include "MainFrm.h"
#include "CLogView.h"
#include "CDisplayView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
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
	m_isSplitted = FALSE;
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

	SetTitle(TEXT("by 湖南麟淇"));

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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CRect rect;
	GetClientRect(&rect);
	int w = rect.Width();
	int h = rect.Height();

	m_isSplitted = m_splitter.CreateStatic(this, 2, 1);

	if (m_isSplitted)
	{
		m_splitter.CreateView(0, 0, RUNTIME_CLASS(CDisplayView), CSize(w, (int)(0.5*h)), pContext);//左侧是CSelectView的实例，大小为200X600
		m_splitter.CreateView(1, 0, RUNTIME_CLASS(CLogView), CSize(w, (int)(0.5*h)), pContext);//右侧是CDisplayView的实例，大小为600X600
	}
	
	//保存上下分割后的两个子窗口的指针
	m_pDisplayView = m_splitter.GetPane(0, 0);
	m_pLogView = m_splitter.GetPane(1, 0);

	return m_isSplitted;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	if (m_isSplitted) {
		m_splitter.SetColumnInfo(0, rect.Width(), 10);
		m_splitter.SetRowInfo(0, int(rect.Height()*0.6), 10);
		m_splitter.SetRowInfo(1, int(rect.Height()*0.4), 10);
		m_splitter.RecalcLayout();
	}
}


void CMainFrame::OnClose()
{
	// 在此添加消息处理程序代码和/或调用默认值
	int ReturnID =  MessageBox(TEXT("您确定要退出程序吗？"),
								TEXT("关闭client"), MB_OKCANCEL);

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
	//MessageBox(TEXT("执行退出线程等操作"));

	logger.m_bRun = FALSE;
	client.exit();
	//client.disconnect_to_ip_address();

}
