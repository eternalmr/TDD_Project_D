// CTaskItem.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskItem.h"
#include "afxdialogex.h"


// CTaskItem 对话框

IMPLEMENT_DYNAMIC(CTaskItem, CDialogEx)

CTaskItem::CTaskItem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK_ITEM, pParent)
{

}

CTaskItem::~CTaskItem()
{
}

void CTaskItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_NAME, m_TaskName);
	DDX_Control(pDX, IDC_TASK_COMPUTE_STATUS, m_TaskComputeStatus);
	DDX_Control(pDX, IDC_TASK_STORE_STATUS, m_TaskStoreStatus);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgessBar);
}


BEGIN_MESSAGE_MAP(CTaskItem, CDialogEx)
END_MESSAGE_MAP()


// CTaskItem 消息处理程序


BOOL CTaskItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProgessBar.ShowWindow(FALSE); //初始不显示进度条

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
