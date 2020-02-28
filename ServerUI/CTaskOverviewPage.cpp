// CTaskOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskOverviewPage.h"
#include "afxdialogex.h"
#include "..\Server\CServer.h"
#include "MainFrm.h"

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
}


BEGIN_MESSAGE_MAP(CTaskOverviewPage, CDialogEx)
	ON_BN_CLICKED(IDC_START, &CTaskOverviewPage::OnBnClickedStart)
	ON_BN_CLICKED(IDC_THREAD, &CTaskOverviewPage::OnBnClickedThreadBtn)
	ON_BN_CLICKED(IDC_BUTTON2, &CTaskOverviewPage::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON3, &CTaskOverviewPage::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON4, &CTaskOverviewPage::OnBnClickedStop)
END_MESSAGE_MAP()


// CTaskOverviewPage 消息处理程序
void CTaskOverviewPage::OnBnClickedThreadBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//CServer server;
	server.add_tasks(10);

	//std::thread      task_thread(&CServer::assign_tasks, &server);
	//std::thread    result_thread(&CServer::collect_result, &server, REPEAT_FOREVER);
	//std::thread heartbeat_thread(&CServer::receive_heartbeat, &server, REPEAT_FOREVER);

	sim_thread = std::thread(&CServer::start_threads, &server);

	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_THREAD, (WPARAM)NM_THREAD, (LPARAM)0);
	
	//std::thread      task_thread(&CServer::assign_tasks, &server);
	//std::thread    result_thread(&CServer::collect_result, &server, REPEAT_FOREVER);
	//std::thread heartbeat_thread(&CServer::receive_heartbeat, &server, REPEAT_FOREVER);

	//if (task_thread.joinable()) task_thread.join();
	//if (result_thread.joinable()) result_thread.join();
	//if (heartbeat_thread.joinable()) heartbeat_thread.join();

	//std::thread run_sim(&CServer::start_threads, &server);
	//run_sim.join();
}

void CTaskOverviewPage::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码

	server.send_command_to_all_client("start");
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_START, (WPARAM)NM_START, (LPARAM)0);

}

void CTaskOverviewPage::OnBnClickedPause()
{
	// TODO: 在此添加控件通知处理程序代码
	server.send_command_to_all_client("pause");
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_PAUSE, (WPARAM)NM_PAUSE, (LPARAM)0);

}


void CTaskOverviewPage::OnBnClickedContinue()
{
	// TODO: 在此添加控件通知处理程序代码
	server.send_command_to_all_client("continue");
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_CONTINUE, (WPARAM)NM_CONTINUE, (LPARAM)0);

}


void CTaskOverviewPage::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	server.send_command_to_all_client("stop");
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_STOP, (WPARAM)NM_STOP, (LPARAM)0);

}
