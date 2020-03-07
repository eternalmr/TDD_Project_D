// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CDisplayView.h"
#include "CUserDefineMsg.h"
#include "MainFrm.h"
// CDisplayView

IMPLEMENT_DYNCREATE(CDisplayView, CFormView)

CDisplayView::CDisplayView()
	: CFormView(IDD_DISPLAYVIEW)
{

}

CDisplayView::~CDisplayView()
{
}

void CDisplayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplayView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDisplayView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDisplayView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDisplayView::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDisplayView 诊断

#ifdef _DEBUG
void CDisplayView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisplayView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDisplayView 消息处理程序
HWND CDisplayView::GetLogWndHandle()
{
	CMainFrame*  pMain = (CMainFrame*)AfxGetMainWnd();
	CWnd *pWnd = pMain->m_splitter.GetPane(1, 0);
	return pWnd->GetSafeHwnd(); //TODO: 找个更加直接的获取窗口句柄的方法
}

void CDisplayView::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString *str = new CString(TEXT("Test Debug Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_DEBUG_LOG, (WPARAM)NW_DEBUG_LOG, (LPARAM)str);
	CClientTest &client = CClientTest::getInstance();
	client.startSim();
}


void CDisplayView::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString *str = new CString(TEXT("Test Debug Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_DETAIL_LOG, (WPARAM)NW_DETAIL_LOG, (LPARAM)str);
}

void CDisplayView::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString *str = new CString(TEXT("Test Debug Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_NORMAL_LOG, (WPARAM)NW_NORMAL_LOG, (LPARAM)str);
}

void CDisplayView::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString *str = new CString(TEXT("Test Debug Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_ERROR_LOG, (WPARAM)NW_ERROR_LOG, (LPARAM)str);
}
