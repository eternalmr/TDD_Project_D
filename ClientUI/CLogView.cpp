// CLogView.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CLogView.h"
#include "CUserDefineMsg.h"


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
	DDX_Control(pDX, IDC_RICHEDIT21, CTLogEdit::GetInstance());
}

BEGIN_MESSAGE_MAP(CLogView, CFormView)

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

	// TODO: 在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect);
	CTLogEdit &logger = CTLogEdit::GetInstance();

	logger.MoveWindow(0, 0, rect.Width(),rect.Height());
	LogThread = std::thread(&CTLogEdit::TLogEditThreadSTL, &logger); //启动日志接受线程
	LogThread.detach();
}

