// CSelectTasksDlg.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CSelectTasksDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "CTaskDlg.h"
#include "ServerUIDoc.h"

// CSelectTasksDlg 对话框

IMPLEMENT_DYNAMIC(CSelectTasksDlg, CDialogEx)

CSelectTasksDlg::CSelectTasksDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECT_TASKS, pParent)
{

}

CSelectTasksDlg::~CSelectTasksDlg()
{
}

void CSelectTasksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_LIST, m_TaskSelectList);
}


BEGIN_MESSAGE_MAP(CSelectTasksDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectTasksDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CSelectTasksDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSelectTasksDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSelectTasksDlg 消息处理程序

void CSelectTasksDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CSelectTasksDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CTaskDlg* pTaskDlg = (CTaskDlg*)pMain->m_RightWindowSplitter.GetPane(0, 0);

	int num = 100; // TODO: Add new tasks according to selection
	for (int i = 0; i < num; i++) {
		server.add_new_task(i + 1);
	}
	pTaskDlg->m_detailPage.ShowLoadedTaskItems();
	server.send_command_to_all_client("newTask");
	MessageBox(TEXT("已添加100个新任务"));
}


void CSelectTasksDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CTaskDlg* pTaskDlg = (CTaskDlg*)pMain->m_RightWindowSplitter.GetPane(0, 0);

	int num = 200; // TODO: Add new tasks according to selection
	for (int i = 100; i < num; i++) {
		server.add_new_task(i + 1);
	}
	//pTaskDlg->detailPage.ShowLoadedTaskItems();
	server.send_command_to_all_client("newTask");
	MessageBox(TEXT("又添加100个新任务"));
}


BOOL CSelectTasksDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	 // TODO: Add extra initialization here   
	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_TaskSelectList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_TaskSelectList.SetExtendedStyle(m_TaskSelectList.GetExtendedStyle() 
									 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
									 | LVS_EX_CHECKBOXES | LVS_EX_HEADERDRAGDROP);

	// 为列表视图控件添加三列   
	m_TaskSelectList.InsertColumn(0, _T("任务ID"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_TaskSelectList.InsertColumn(1, _T("任务名称"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_TaskSelectList.InsertColumn(2, _T("任务参数"), LVCFMT_CENTER, rect.Width() / 3, 2);

	CString str;

	for (int i = 0; i <= 20; i++) {
		m_TaskSelectList.InsertItem(i, _T(""));
		str.Format(_T("%d"), i);
		m_TaskSelectList.SetItemText(i, 0, str);
		str.Format(_T("任务%d"), i);
		m_TaskSelectList.SetItemText(i, 1, str);
		str.Format(_T("input = %d"), i);
		m_TaskSelectList.SetItemText(i, 2, str);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
