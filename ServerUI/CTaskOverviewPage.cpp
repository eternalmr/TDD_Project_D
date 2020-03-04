// CTaskOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskOverviewPage.h"
#include "afxdialogex.h"
#include "..\Server\CServer.h"
#include "MainFrm.h"

// CTaskOverviewPage 对话框

IMPLEMENT_DYNAMIC(CTaskOverviewPage, CDialogEx)

CTaskOverviewPage::CTaskOverviewPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK_OVERVIEW, pParent)
{

}

CTaskOverviewPage::~CTaskOverviewPage()
{
}

void CTaskOverviewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTaskOverviewPage, CDialogEx)
	ON_BN_CLICKED(IDC_START,    &CTaskOverviewPage::OnBnClickedStart)
	ON_BN_CLICKED(IDC_THREAD,   &CTaskOverviewPage::OnBnClickedThreadBtn)
	ON_BN_CLICKED(IDC_PAUSE,    &CTaskOverviewPage::OnBnClickedPause)
	ON_BN_CLICKED(IDC_CONTINUE, &CTaskOverviewPage::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_STOP,     &CTaskOverviewPage::OnBnClickedStop)

	//日志测试函数
	ON_BN_CLICKED(IDC_BUTTON1, &CTaskOverviewPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTaskOverviewPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTaskOverviewPage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTaskOverviewPage::OnBnClickedButton4)
END_MESSAGE_MAP()


// CTaskOverviewPage 消息处理程序
void CTaskOverviewPage::OnBnClickedThreadBtn()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_THREAD, (WPARAM)NM_THREAD, (LPARAM)0);
}

void CTaskOverviewPage::OnBnClickedStart()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_START, (WPARAM)NM_START, (LPARAM)0);
}

void CTaskOverviewPage::OnBnClickedPause()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_PAUSE, (WPARAM)NM_PAUSE, (LPARAM)0);
}


void CTaskOverviewPage::OnBnClickedContinue()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_CONTINUE, (WPARAM)NM_CONTINUE, (LPARAM)0);
}

void CTaskOverviewPage::OnBnClickedStop()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_STOP, (WPARAM)NM_STOP, (LPARAM)0);
}



//日志测试函数
HWND CTaskOverviewPage::GetLogWndHandle()
{
	CMainFrame*  pMain = (CMainFrame*)AfxGetMainWnd();
	CWnd *pWnd = pMain->m_RightWindowSplitter.GetPane(1, 0);
	return pWnd->GetSafeHwnd(); //TODO: 找个更加直接的获取窗口句柄的方法
}

void CTaskOverviewPage::OnBnClickedButton1()
{
	CString *str = new CString(TEXT("Test Debug Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_DEBUG_LOG, (WPARAM)NW_DEBUG_LOG, (LPARAM)str);
}

void CTaskOverviewPage::OnBnClickedButton2()
{
	CString *str = new CString(TEXT("Test Detail Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_DETAIL_LOG, (WPARAM)NW_DETAIL_LOG, (LPARAM)str);
}


void CTaskOverviewPage::OnBnClickedButton3()
{
	CString *str = new CString(TEXT("Test Normal Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_NORMAL_LOG, (WPARAM)NW_NORMAL_LOG, (LPARAM)str);
}


void CTaskOverviewPage::OnBnClickedButton4()
{
	CString *str = new CString(TEXT("Test Error Log\r\n"));
	::PostMessage(GetLogWndHandle(), NW_ERROR_LOG, (WPARAM)NW_ERROR_LOG, (LPARAM)str);
}
