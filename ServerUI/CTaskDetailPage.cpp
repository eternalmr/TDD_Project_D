// CTaskDetailPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskDetailPage.h"
#include "afxdialogex.h"
#include "CLogView.h"
#include "MainFrm.h"

// CTaskDetailPage 对话框

IMPLEMENT_DYNAMIC(CTaskDetailPage, CDialogEx)

CTaskDetailPage::CTaskDetailPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK_DETAIL, pParent)
{

}

CTaskDetailPage::~CTaskDetailPage()
{
}

void CTaskDetailPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTaskDetailPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTaskDetailPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTaskDetailPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTaskDetailPage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTaskDetailPage::OnBnClickedButton4)
END_MESSAGE_MAP()


// CTaskDetailPage 消息处理程序
HWND CTaskDetailPage::GetLogWndHandle()
{
	CMainFrame*  pMain = (CMainFrame*)AfxGetMainWnd();
	CWnd *pWnd = pMain->m_RightWindowSplitter.GetPane(1, 0);
	return pWnd->GetSafeHwnd(); //TODO: 找个更加直接的获取窗口句柄的方法
}

void CTaskDetailPage::OnBnClickedButton1()
{
	CString *str = new CString(TEXT("Test Debug Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_DEBUG_LOG, (WPARAM)NW_DEBUG_LOG, (LPARAM)str);
}

void CTaskDetailPage::OnBnClickedButton2()
{
	CString *str = new CString(TEXT("Test Detail Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_DETAIL_LOG, (WPARAM)NW_DETAIL_LOG, (LPARAM)str);
}


void CTaskDetailPage::OnBnClickedButton3()
{
	CString *str = new CString(TEXT("Test Normal Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_NORMAL_LOG, (WPARAM)NW_NORMAL_LOG, (LPARAM)str);
}


void CTaskDetailPage::OnBnClickedButton4()
{
	CString *str = new CString(TEXT("Test Error Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_ERROR_LOG, (WPARAM)NW_ERROR_LOG, (LPARAM)str);
}
