// CTaskOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskOverviewPage.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CTaskOverviewPage 消息处理程序
