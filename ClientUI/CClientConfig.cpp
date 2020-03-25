// CClientConfig.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CClientConfig.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CClientConfig 对话框

IMPLEMENT_DYNAMIC(CClientConfig, CDialogEx)

CClientConfig::CClientConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG, pParent)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pDoc = (CClientUIDoc *)pMain->GetActiveDocument();
}

CClientConfig::~CClientConfig()
{
}

void CClientConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLIENT_ID, m_ClientID);
	DDV_MinMaxInt(pDX, m_ClientID, 1, 1000);
	DDX_Text(pDX, IDC_CONTROL_PORT, m_ControlPort);
	DDX_Text(pDX, IDC_HEARTBEAT_PORT, m_HeartbeatPort);
	DDV_MinMaxInt(pDX, m_HeartbeatPort, 1024, INT_MAX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Text(pDX, IDC_RESULT_PORT, m_ResultPort);
	DDV_MinMaxInt(pDX, m_ResultPort, 1024, INT_MAX);
	DDX_Text(pDX, IDC_TASK_PORT, m_TaskPort);
	DDV_MinMaxInt(pDX, m_TaskPort, 1024, INT_MAX);
}


void CClientConfig::ReadConfigFile()
{
	pDoc->ReadConfigFile();
	m_ClientID = pDoc->m_ClientID;
	m_HeartbeatPort = pDoc->m_HeartbeatPort;
	m_ControlPort = pDoc->m_ControlPort;
	m_TaskPort = pDoc->m_TaskPort;
	m_ResultPort = pDoc->m_ResultPort;
	m_IPAddress.SetAddress(ntohl(inet_addr(CT2A(pDoc->ipAddress))));
	UpdateData(FALSE);
}

BOOL CClientConfig::WriteConfig(char* configName)
{
	CString str;
	CString configNameStr("client configuration");
	CString filePath(".\\client.config");

	if (strcmp(configName, "ClientID") == 0) {
		str.Format(TEXT("%d"), m_ClientID);
	}
	else if (strcmp(configName, "HeartbeatPort") == 0) {
		str.Format(TEXT("%d"), m_HeartbeatPort);
	}
	else if (strcmp(configName, "ControlPort") == 0) {
		str.Format(TEXT("%d"), m_ControlPort);
	}
	else if (strcmp(configName, "TaskPort") == 0) {
		str.Format(TEXT("%d"), m_TaskPort);
	}
	else if (strcmp(configName, "ResultPort") == 0) {
		str.Format(TEXT("%d"), m_ResultPort);
	}
	else if (strcmp(configName, "IPAddress") == 0) {
		BYTE f0, f1, f2, f3;
		m_IPAddress.GetAddress(f0, f1, f2, f3);
		str.Format(TEXT("%d%s%d%s%d%s%d"), f0, ".", f1, ".", f2, ".", f3);
	}
	else {
		return FALSE;
	}

	BOOL flag = WritePrivateProfileString(configNameStr, CString(configName), str, filePath);
	return flag;
}

BEGIN_MESSAGE_MAP(CClientConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CClientConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CClientConfig 消息处理程序


BOOL CClientConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ReadConfigFile();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CClientConfig::OnBnClickedOk()
{
	UpdateData(TRUE);

	//CString filePath(".\\server.config");
	//DeleteFile(filePath);

	BOOL Result1 = WriteConfig("ClientID");
	BOOL Result2 = WriteConfig("IPAddress");
	BOOL Result3 = WriteConfig("HeartbeatPort");
	BOOL Result4 = WriteConfig("ControlPort");
	BOOL Result5 = WriteConfig("TaskPort");
	BOOL Result6 = WriteConfig("ResultPort");


	if (Result1&&Result2&&Result3&&Result4&&Result5&&Result6) {
		MessageBox(TEXT("保存配置成功！"));
	}

	CDialogEx::OnOK();
}
