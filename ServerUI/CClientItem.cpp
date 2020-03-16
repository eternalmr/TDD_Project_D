// CClientItem.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientItem.h"
#include "afxdialogex.h"

#define RandNum(minValue, maxValue)((rand() % (maxValue - minValue + 1)) + minValue)

// CClientItem 对话框

IMPLEMENT_DYNAMIC(CClientItem, CDialogEx)

CClientItem::CClientItem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_ITEM, pParent)
{

}

CClientItem::~CClientItem()
{
}

void CClientItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENT_NAME, m_ClientName);
	DDX_Control(pDX, IDC_CLIENT_STATUS, m_ClientStatus);
	DDX_Control(pDX, IDC_CLIENT_CPU, m_ClientCPU);
	DDX_Control(pDX, IDC_CLIENT_MEMORY, m_ClientMemory);
	DDX_Control(pDX, IDC_BTN_START_CLIENT, m_StartBtn);
	DDX_Control(pDX, IDC_BTN_PAUSE_CLIENT, m_PauseBtn);
	DDX_Control(pDX, IDC_BTN_STOP_CLIENT, m_StopBtn);
}


BEGIN_MESSAGE_MAP(CClientItem, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_CLIENT, &CClientItem::OnBnClickedBtnStartClient)
	ON_BN_CLICKED(IDC_BTN_PAUSE_CLIENT, &CClientItem::OnBnClickedBtnPauseClient)
	ON_BN_CLICKED(IDC_BTN_STOP_CLIENT, &CClientItem::OnBnClickedBtnStopClient)
	ON_BN_CLICKED(IDC_BTN_CONTINUE_CLIENT, &CClientItem::OnBnClickedBtnContinueClient)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientItem 消息处理程序


void CClientItem::OnBnClickedBtnStartClient()
{
	server.send_command_to_client(m_id, "start");
}


void CClientItem::OnBnClickedBtnPauseClient()
{
	server.send_command_to_client(m_id, "pause");
}


void CClientItem::OnBnClickedBtnStopClient()
{
	server.send_command_to_client(m_id, "stop");
}


void CClientItem::OnBnClickedBtnContinueClient()
{
	server.send_command_to_client(m_id, "continue");
}


void CClientItem::OnTimer(UINT_PTR nIDEvent)
{
	// 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1: {
		UpdateClientInfo();
		break;
	}
	default:
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}

void CClientItem::UpdateClientInfo()
{
	CString str;
	//str.Format(TEXT("%.2lf"), 80);
	//str = CString("状态： 计算中（任务1）") + str + CString("%");
	//m_ClientStatus.SetWindowTextW(str);

	str.Format(TEXT("%d"), RandNum(50,60));
	str = CString("CPU：") + str + CString("%");
	m_ClientCPU.SetWindowTextW(str);

	str.Format(TEXT("%d"), RandNum(70,80));
	str = CString("内存：") + str + CString("%");
	m_ClientMemory.SetWindowTextW(str);

}


BOOL CClientItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在此添加额外的初始化
	UpdateClientInfo();

	SetTimer(1, 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
