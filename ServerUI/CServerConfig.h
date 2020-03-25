#pragma once

#include "ServerUIDoc.h"

// CServerConfig 对话框

class CServerConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CServerConfig)

public:
	CServerConfig(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CServerConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_HeartbeatPort;
	int m_ControlPort;
	int m_ResultPort;
	int m_TaskPort;
	CIPAddressCtrl m_IPAddress;

	CServerUIDoc* pDoc;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	void ReadConfigFile();
	BOOL WriteConfig(char* configName);
};
