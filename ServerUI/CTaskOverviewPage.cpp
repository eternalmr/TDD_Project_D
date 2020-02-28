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
