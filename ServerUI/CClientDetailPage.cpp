// CClientDetailPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientDetailPage.h"
#include "afxdialogex.h"


// CClientDetailPage 对话框

IMPLEMENT_DYNAMIC(CClientDetailPage, CDialogEx)

CClientDetailPage::CClientDetailPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DETAIL, pParent)
{

}

CClientDetailPage::~CClientDetailPage()
{
}

void CClientDetailPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientDetailPage, CDialogEx)
END_MESSAGE_MAP()


// CClientDetailPage 消息处理程序
