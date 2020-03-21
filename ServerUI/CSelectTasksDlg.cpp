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

	int num = 10; // TODO: Add new tasks according to selection
	for (int i = 0; i < num; i++) {
		server.add_new_task(i + 1);
	}
	pTaskDlg->detailPage.ShowLoadedTaskItems();

	MessageBox(TEXT("已添加10个新任务"));
}


void CSelectTasksDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CTaskDlg* pTaskDlg = (CTaskDlg*)pMain->m_RightWindowSplitter.GetPane(0, 0);

	int num = 20; // TODO: Add new tasks according to selection
	for (int i = 10; i < num; i++) {
		server.add_new_task(i + 1);
	}
	//pTaskDlg->detailPage.ShowLoadedTaskItems();

	MessageBox(TEXT("又添加10个新任务"));
}
