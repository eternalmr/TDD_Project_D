// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CDisplayView.h"


IMPLEMENT_DYNCREATE(CDisplayView, CFormView)

CDisplayView::CDisplayView()
	: CFormView(IDD_DISPLAYVIEW)
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
	DDX_Control(pDX, IDC_CLIENT_ID, m_clientName);
	DDX_Control(pDX, IDC_CURRENT_TASK, m_currentTask);
}

BEGIN_MESSAGE_MAP(CDisplayView, CFormView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayView::OnBnClickedHasTask)
	ON_BN_CLICKED(IDC_BUTTON2, &CDisplayView::OnBnClickedStopGettingTask)
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

	CString str;
	str.Format(TEXT("推演节点%d："), client.get_id());
	m_clientName.SetWindowTextW(str);

	m_progressBar.SetRange(0, 100);
	SetTimer(ClientInfoUpdateTimer, 1000, NULL);//启动CPU、内存状态刷新计时器

	client.start_threads();//启动心跳、任务、控制、仿真线程
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



void CDisplayView::OnDestroy()
{
	CFormView::OnDestroy();

	//在此处添加消息处理程序代码
	KillTimer(ClientInfoUpdateTimer);
	OutputDebugString(TEXT("已终止client更新计时器\r\n"));
}


void CDisplayView::OnBnClickedHasTask() //server有新任务
{
	client.server_has_no_pending_tasks = false;
}


void CDisplayView::OnBnClickedStopGettingTask() //server没有新任务
{
	client.server_has_no_pending_tasks = true;
}
