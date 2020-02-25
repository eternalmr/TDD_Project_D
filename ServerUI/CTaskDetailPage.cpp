// CTaskDetailPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskDetailPage.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CTaskDetailPage 消息处理程序
