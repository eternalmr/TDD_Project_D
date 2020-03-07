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
	DDX_Control(pDX, IDC_RICHEDIT21, m_log);
}

BEGIN_MESSAGE_MAP(CLogView, CFormView)
	ON_MESSAGE(NW_DEBUG_LOG, &CLogView::OnNwWritelog)
	ON_MESSAGE(NW_DETAIL_LOG, &CLogView::OnNwWritelog)
	ON_MESSAGE(NW_NORMAL_LOG, &CLogView::OnNwWritelog)
	ON_MESSAGE(NW_ERROR_LOG, &CLogView::OnNwWritelog)

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
	m_log.MoveWindow(0, 0, rect.Width(),rect.Height());

	LogThread = std::thread(&CTLogEdit::TLogEditThreadSTL, &m_log); //启动日志接受线程

}


afx_msg LRESULT CLogView::OnNwWritelog(WPARAM wParam, LPARAM lParam)
{
	CString *str = (CString*)lParam;
	if (wParam == NW_DEBUG_LOG) {
		m_log.AddLine(*str, TLP_DEBUG);
	}
	if (wParam == NW_DETAIL_LOG) {
		m_log.AddLine(*str, TLP_DETAIL);
	}
	if (wParam == NW_NORMAL_LOG) {
		m_log.AddLine(*str, TLP_NORMAL);
	}
	if (wParam == NW_ERROR_LOG) {
		m_log.AddLine(*str, TLP_ERROR);
	}
	delete str;

	return 0;
}

