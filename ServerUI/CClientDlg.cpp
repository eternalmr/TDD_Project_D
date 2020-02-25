// CClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientDlg.h"


// CClientDlg

IMPLEMENT_DYNCREATE(CClientDlg, CFormView)

CClientDlg::CClientDlg()
	: CFormView(IDD_CLIENT)
{

}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENT_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CClientDlg, CFormView)
END_MESSAGE_MAP()


// CClientDlg 诊断

#ifdef _DEBUG
void CClientDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientDlg 消息处理程序


void CClientDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_tab.AddPage(TEXT("节点概况"), &overviewPage, IDD_CLIENT_OVERVIEW, true);
	m_tab.AddPage(TEXT("节点详情"), &detailPage, IDD_CLIENT_DETAIL, true);
	m_tab.CreatePage();
	m_tab.Show(0);

}
