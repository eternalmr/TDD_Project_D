// CTaskOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskOverviewPage.h"
#include "afxdialogex.h"
#include "..\Server\CServer.h"

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
	ON_BN_CLICKED(IDC_BUTTON1, &CTaskOverviewPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CTaskOverviewPage::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CTaskOverviewPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTaskOverviewPage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTaskOverviewPage::OnBnClickedButton4)
END_MESSAGE_MAP()


// CTaskOverviewPage 消息处理程序
void CTaskOverviewPage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	server.send_command_to_all_client("start");
}


void CTaskOverviewPage::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//CServer server;
	server.add_tasks(10);

	//std::thread      task_thread(&CServer::assign_tasks, &server);
	//std::thread    result_thread(&CServer::collect_result, &server, REPEAT_FOREVER);
	//std::thread heartbeat_thread(&CServer::receive_heartbeat, &server, REPEAT_FOREVER);

	sim_thread = std::thread(&CServer::start_threads, &server);


	//std::thread      task_thread(&CServer::assign_tasks, &server);
	//std::thread    result_thread(&CServer::collect_result, &server, REPEAT_FOREVER);
	//std::thread heartbeat_thread(&CServer::receive_heartbeat, &server, REPEAT_FOREVER);

	//if (task_thread.joinable()) task_thread.join();
	//if (result_thread.joinable()) result_thread.join();
	//if (heartbeat_thread.joinable()) heartbeat_thread.join();

	//std::thread run_sim(&CServer::start_threads, &server);
	//run_sim.join();
}


void CTaskOverviewPage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	server.send_command_to_all_client("pause");
}


void CTaskOverviewPage::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	server.send_command_to_all_client("continue");
}


void CTaskOverviewPage::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	server.send_command_to_all_client("stop");
}
