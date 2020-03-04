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

END_MESSAGE_MAP()


BOOL CTaskDetailPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str;
	for (int i = 0; i < TASK_NUM; i++)
	{
		str.Format(TEXT("第%d个任务"), i);
		m_TaskItem[i].Create(IDD_TASK_ITEM, this);
		m_TaskItem[i].MoveWindow(0, 100 * i + 1 * i, 500, 100);
		m_TaskItem[i].m_id = i;
		m_TaskItem[i].m_TaskName.SetWindowTextW(str);
		m_TaskItem[i].ShowWindow(SW_SHOWNORMAL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
