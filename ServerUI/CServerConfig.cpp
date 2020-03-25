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
	DDV_MinMaxInt(pDX, m_HeartbeatPort, 1024, 65535);
	DDX_Text(pDX, IDC_CONTROL_PORT, m_ControlPort);
	DDV_MinMaxInt(pDX, m_ControlPort, 1024, 65535);
	DDX_Text(pDX, IDC_RESULT_PORT, m_ResultPort);
	DDV_MinMaxInt(pDX, m_ResultPort, 1024, 65535);
	DDX_Text(pDX, IDC_TASK_PORT, m_TaskPort);
	DDV_MinMaxInt(pDX, m_TaskPort, 1024, 65535);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
}


BEGIN_MESSAGE_MAP(CServerConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CServerConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerConfig 消息处理程序


BOOL CServerConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//从Doc类中读取IP和Port的设置
	ReadConfigFile();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CServerConfig::OnBnClickedOk()
{
	UpdateData(TRUE);

	//CString filePath(".\\server.config");
	//DeleteFile(filePath);

	BOOL Result1 = WriteConfig("IPAddress");
	BOOL Result2 = WriteConfig("HeartbeatPort");
	BOOL Result3 = WriteConfig("ControlPort");
	BOOL Result4 = WriteConfig("TaskPort");
	BOOL Result5 = WriteConfig("ResultPort");

	if (Result1&&Result2&&Result3&&Result4&&Result5){
		MessageBox(TEXT("保存配置成功！"));
	}

	CDialogEx::OnOK();
}

void CServerConfig::ReadConfigFile()
{
	CString configNameStr("server configuration");
	CString filePath(".\\server.config");

	int result = GetPrivateProfileInt(configNameStr, CString("HeartbeatPort1"), 5555, filePath);
	m_HeartbeatPort = result;

	result = GetPrivateProfileInt(configNameStr, CString("ControlPort"), 5556, filePath);
	m_ControlPort = result;

	result = GetPrivateProfileInt(configNameStr, CString("TaskPort"), 5557, filePath);
	m_TaskPort = result;

	result = GetPrivateProfileInt(configNameStr, CString("ResultPort"), 5558, filePath);
	m_ResultPort = result;

	CString ipAddress;
	GetPrivateProfileString(configNameStr, CString("IPAddress"), CString("127.0.0.1"), ipAddress.GetBuffer(MAX_PATH), MAX_PATH, filePath);
	m_IPAddress.SetAddress(ntohl(inet_addr(CT2A(ipAddress))));

	UpdateData(FALSE);
}

BOOL CServerConfig::WriteConfig(char* configName)
{
	CString str;
	CString configNameStr("server configuration");
	CString filePath(".\\server.config");

	if (strcmp(configName, "HeartbeatPort") == 0) {
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
