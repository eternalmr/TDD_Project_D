// CTaskDlg.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskDlg.h"


// CTaskDlg

IMPLEMENT_DYNCREATE(CTaskDlg, CFormView)

CTaskDlg::CTaskDlg()
	: CFormView(IDD_TASK)
	,m_bDlgIsCreated(FALSE)
{

}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CTaskDlg, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTaskDlg 诊断

#ifdef _DEBUG
void CTaskDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTaskDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTaskDlg 消息处理程序


void CTaskDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// 在此添加专用代码和/或调用基类
	m_bDlgIsCreated = TRUE;

	CRect rect;
	GetClientRect(&rect);
	m_tab.MoveWindow(0, 0, rect.Width(), rect.Height());

	m_tab.AddPage(TEXT("任务概况"), &m_overviewPage, IDD_TASK_OVERVIEW, true);
	m_tab.AddPage(TEXT("任务详情"), &m_detailPage, IDD_TASK_DETAIL, true);
	m_tab.CreatePage();
	m_tab.Show(0);
}


void CTaskDlg::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (m_bDlgIsCreated) {
		CRect rect;
		GetClientRect(&rect);
		m_tab.MoveWindow(0, 0, rect.Width(), rect.Height());
		m_overviewPage.MoveWindow(0, 21, rect.Width(), rect.Height()-21);
		m_detailPage.MoveWindow(0, 21, rect.Width(), rect.Height()-21);
	}

	ShowScrollBar(SB_BOTH, FALSE);//垂直水平方向的滚动条都不显示。
}