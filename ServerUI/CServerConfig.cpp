// CServerConfig.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CServerConfig.h"
#include "afxdialogex.h"


// CServerConfig 对话框

IMPLEMENT_DYNAMIC(CServerConfig, CDialogEx)

CServerConfig::CServerConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG, pParent)
	, m_HeartbeatPort(5555)
	, m_ControlPort(5556)
	, m_TaskPort(5557)
	, m_ResultPort(5558)
{

}

CServerConfig::~CServerConfig()
{
}

void CServerConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEARTBEAT_PORT, m_HeartbeatPort);
	DDV_MinMaxInt(pDX, m_HeartbeatPort, 5000, INT_MAX);
	DDX_Text(pDX, IDC_CONTROL_PORT, m_ControlPort);
	DDV_MinMaxInt(pDX, m_ControlPort, 5000, INT_MAX);
	DDX_Text(pDX, IDC_RESULT_PORT, m_ResultPort);
	DDV_MinMaxInt(pDX, m_ResultPort, 5000, INT_MAX);
	DDX_Text(pDX, IDC_TASK_PORT, m_TaskPort);
	DDV_MinMaxInt(pDX, m_TaskPort, 5000, INT_MAX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
}


BEGIN_MESSAGE_MAP(CServerConfig, CDialogEx)
END_MESSAGE_MAP()


// CServerConfig 消息处理程序


BOOL CServerConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//从Doc类中读取IP和Port的设置
	m_IPAddress.SetAddress(192, 168, 100, 239);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
