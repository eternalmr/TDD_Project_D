// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CDisplayView.h"
#include "CUserDefineMsg.h"
#include "MainFrm.h"


IMPLEMENT_DYNCREATE(CDisplayView, CFormView)

CDisplayView::CDisplayView()
	: CFormView(IDD_DISPLAYVIEW)
	, m_clientName(_T("推演节点："))
{
	
}

CDisplayView::~CDisplayView()
{

}

void CDisplayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
	DDX_Control(pDX, IDC_MEMORY_STATUS, m_memoryStatus);
	DDX_Control(pDX, IDC_CPU_STATUS, m_cpuStatus);
	DDX_Text(pDX, IDC_CLIENT_ID, m_clientName);
	DDX_Control(pDX, IDC_CURRENT_TASK, m_currentTask);
	DDX_Control(pDX, IDC_CONFIRMID_BTN, m_confirmIdBtn);
	DDX_Control(pDX, IDC_CLIENTID_EDIT, m_ClientIdInput);
}

BEGIN_MESSAGE_MAP(CDisplayView, CFormView)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CONFIRMID_BTN, &CDisplayView::OnBnClickedConfirmidBtn)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDisplayView::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDisplayView 诊断

#ifdef _DEBUG
void CDisplayView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisplayView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CDisplayView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_ClientIdInput.SetWindowTextW(TEXT("1"));
	m_progressBar.SetRange(0, 100);

	SetTimer(ClientInfoUpdateTimer, 1000, NULL);//启动CPU、内存状态刷新计时器
}


void CDisplayView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ClientInfoUpdateTimer: {
		UpdateClientInfo();
		break;
	}
	case 2: {
		break;
	}
	default:
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

void CDisplayView::UpdateClientInfo()
{
	CString str;

	str.Format(TEXT("%.2lf"), client.get_cpu_status());
	str = CString("CPU状态：") + str + CString("%");
	m_cpuStatus.SetWindowTextW(str);

	str.Format(TEXT("%.2lf"), client.get_memoery_status());
	str = CString("内存状态：") + str + CString("%");
	m_memoryStatus.SetWindowTextW(str);

	if (client.get_task_id() == 0) {
		str.Format(TEXT("当前计算任务：暂无"));
	}
	else {
		str.Format(TEXT("当前计算任务：任务%d"), client.get_task_id());
	}
	m_currentTask.SetWindowTextW(str);

	m_progressBar.SetPos(client.get_progress());
}


void CDisplayView::OnBnClickedConfirmidBtn()
{
	UpdateData(TRUE);
	CString str;
	m_ClientIdInput.GetWindowTextW(str);
	client.set_id(_ttoi(str));

	m_clientName = CString(TEXT("推演节点：")) + str;
	client.task_thread = std::thread(&CClient::receive_tasks, &client);
	client.simulation_thread = std::thread(&CClient::wrap_simulation_process, &client, 0);
	client.control_thread = std::thread(&CClient::receive_command, &client);

	client.task_thread.detach();
	client.control_thread.detach();
	client.simulation_thread.detach();

	UpdateData(FALSE);
	m_confirmIdBtn.ShowWindow(FALSE);
	m_ClientIdInput.ShowWindow(FALSE);
}


void CDisplayView::OnDestroy()
{
	CFormView::OnDestroy();

	//在此处添加消息处理程序代码
	KillTimer(ClientInfoUpdateTimer);
	OutputDebugString(TEXT("已终止client更新计时器\r\n"));
}


void CDisplayView::OnBnClickedButton1() //server有新任务
{
	client.server_has_no_pending_tasks = false;
}


void CDisplayView::OnBnClickedButton2() //server没有新任务
{
	client.server_has_no_pending_tasks = true;
}
