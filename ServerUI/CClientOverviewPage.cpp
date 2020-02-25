// CClientOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientOverviewPage.h"
#include "afxdialogex.h"


// CClientOverviewPage 对话框

IMPLEMENT_DYNAMIC(CClientOverviewPage, CDialogEx)

CClientOverviewPage::CClientOverviewPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_OVERVIEW, pParent)
{

}

CClientOverviewPage::~CClientOverviewPage()
{
}

void CClientOverviewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientOverviewPage, CDialogEx)
END_MESSAGE_MAP()


// CClientOverviewPage 消息处理程序
