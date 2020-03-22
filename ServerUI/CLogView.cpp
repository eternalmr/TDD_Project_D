// CLogView.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CLogView.h"

// CLogView

IMPLEMENT_DYNCREATE(CLogView, CFormView)

CLogView::CLogView()
	: CFormView(IDD_LOGVIEW)
{

}

CLogView::~CLogView()
{
}

void CLogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGBOARD, CLogShow::GetInstance());
}

BEGIN_MESSAGE_MAP(CLogView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLogView 诊断

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CLogView 消息处理程序

void CLogView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rect;
	GetClientRect(&rect);

	logger.MoveWindow(0, 0, rect.Width(), rect.Height());
	logger.m_LogThread = std::thread(&CLogShow::TLogEditThreadSTL, &logger); //启动日志接受线程
}


void CLogView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ShowScrollBar(SB_BOTH, FALSE);//垂直水平方向的滚动条都不显示。
}
