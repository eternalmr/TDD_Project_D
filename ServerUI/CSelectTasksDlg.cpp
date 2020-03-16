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
END_MESSAGE_MAP()


// CSelectTasksDlg 消息处理程序

void CSelectTasksDlg::OnBnClickedOk()
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CTaskDlg* pTaskDlg = (CTaskDlg*)pMain->m_RightWindowSplitter.GetPane(0, 0);
	
	int num = 10; // TODO: Add new tasks according to selection
	for (int i = 0; i < num ; i++) {
		CServer::get_instance().add_new_task(i+1);
	}
	pTaskDlg->detailPage.UpdateShow();

	MessageBox(TEXT("已添加10个新任务"));
	CDialogEx::OnOK();
}
