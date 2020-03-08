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
	AddLog(TEXT("启动线程\r\n"), TLP_NORMAL);
}

void CTaskOverviewPage::OnBnClickedStart()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_START, (WPARAM)NM_START, (LPARAM)0);
	AddLog(TEXT("开始仿真\r\n"), TLP_NORMAL);
}

void CTaskOverviewPage::OnBnClickedPause()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_PAUSE, (WPARAM)NM_PAUSE, (LPARAM)0);
	AddLog(TEXT("暂停所有仿真\r\n"), TLP_NORMAL);
}


void CTaskOverviewPage::OnBnClickedContinue()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_CONTINUE, (WPARAM)NM_CONTINUE, (LPARAM)0);
	AddLog(TEXT("继续仿真\r\n"), TLP_NORMAL);
}

void CTaskOverviewPage::OnBnClickedStop()
{
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_STOP, (WPARAM)NM_STOP, (LPARAM)0);
	AddLog(TEXT("结束仿真\r\n"), TLP_NORMAL);
}



//日志测试函数
void CTaskOverviewPage::OnBnClickedButton1()
{
	AddLog(TEXT("测试Debug\r\n"), TLP_DEBUG);
}

void CTaskOverviewPage::OnBnClickedButton2()
{
	AddLog(TEXT("测试Detail\r\n"), TLP_DETAIL);
}


void CTaskOverviewPage::OnBnClickedButton3()
{
	AddLog(TEXT("测试Normal\r\n"), TLP_NORMAL);
}


void CTaskOverviewPage::OnBnClickedButton4()
{
	AddLog(TEXT("测试Error\r\n"), TLP_ERROR);
}
