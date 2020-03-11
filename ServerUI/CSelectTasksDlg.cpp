// CSelectTasksDlg.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CSelectTasksDlg.h"
#include "afxdialogex.h"


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
	// TODO: 在此添加控件通知处理程序代码
	CServer::get_instance().add_tasks(10);

	MessageBox(TEXT("已添加10个新任务"));
	CDialogEx::OnOK();
}
