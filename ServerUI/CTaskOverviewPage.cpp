// CTaskOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskOverviewPage.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "CSelectTasksDlg.h"

enum TimerType
{
	UpdateTaskInfoTimer = 1
};


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
	DDX_Control(pDX, IDC_TOTAL_TASK_NUM, m_TotalTaskNum);
	DDX_Control(pDX, IDC_COMPLETED_TASK_NUM, m_CompletedTaskNum);
	DDX_Control(pDX, IDC_INCOMPUTING_TASK_NUM, m_IncomputingTaskNum);
	DDX_Control(pDX, IDC_UNDO_TASK_NUM, m_UndoTaskNum);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CTaskOverviewPage, CDialogEx)
	ON_BN_CLICKED(IDC_START,    &CTaskOverviewPage::OnBnClickedStart)
	ON_BN_CLICKED(IDC_THREAD,   &CTaskOverviewPage::OnBnClickedThreadBtn)
	ON_BN_CLICKED(IDC_PAUSE,    &CTaskOverviewPage::OnBnClickedPause)
	ON_BN_CLICKED(IDC_CONTINUE, &CTaskOverviewPage::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_STOP,     &CTaskOverviewPage::OnBnClickedStop)
	ON_BN_CLICKED(IDC_LOAD,		&CTaskOverviewPage::OnBnClickedLoad)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTaskOverviewPage 消息处理程序
void CTaskOverviewPage::OnBnClickedThreadBtn()
{
	server.sim_thread = std::thread(&CServer::start_threads, &server);
	AddLog(TEXT("启动线程\r\n"), TLP_NORMAL);
	MessageBox(TEXT("启动线程"));
}

void CTaskOverviewPage::OnBnClickedStart()
{
	server.send_command_to_all_client("start");
	AddLog(TEXT("开始仿真\r\n"), TLP_NORMAL);
	MessageBox(TEXT("开始任务"));
}

void CTaskOverviewPage::OnBnClickedPause()
{
	server.send_command_to_all_client("pause");
	AddLog(TEXT("暂停所有仿真\r\n"), TLP_NORMAL);
	MessageBox(TEXT("暂停任务"));
}


void CTaskOverviewPage::OnBnClickedContinue()
{
	server.send_command_to_all_client("continue");
	AddLog(TEXT("继续仿真\r\n"), TLP_NORMAL);
	MessageBox(TEXT("继续任务"));
}

void CTaskOverviewPage::OnBnClickedStop()
{
	server.send_command_to_all_client("stop");
	AddLog(TEXT("结束仿真\r\n"), TLP_NORMAL);
	MessageBox(TEXT("结束任务"));
}

void CTaskOverviewPage::OnBnClickedLoad()
{
	CSelectTasksDlg dlg;
	dlg.DoModal();
}


BOOL CTaskOverviewPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在此添加额外的初始化
	UpdateTaskInfo();

	SetTimer(UpdateTaskInfoTimer, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTaskOverviewPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case UpdateTaskInfoTimer: {
		UpdateTaskInfo();
		break;
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTaskOverviewPage::UpdateTaskInfo()
{
	CString str;
	int totalNum, completedNum, inComputingNum, undoNum;

	server.get_task_num_info(totalNum,completedNum, inComputingNum, undoNum);

	str.Format(TEXT("方案总数：\r\n %d"), totalNum);
	m_TotalTaskNum.SetWindowTextW(str);

	str.Format(TEXT("已完成：\r\n %d"), completedNum);
	m_CompletedTaskNum.SetWindowTextW(str);

	str.Format(TEXT("计算中：\r\n %d"), inComputingNum);
	m_IncomputingTaskNum.SetWindowTextW(str);

	str.Format(TEXT("未开始：\r\n %d"), undoNum);
	m_UndoTaskNum.SetWindowTextW(str);

	m_ProgressBar.SetRange(0, totalNum);
	m_ProgressBar.SetPos(completedNum);
}
