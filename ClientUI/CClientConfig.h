#pragma once
#pragma warning(disable:4996)

#include "ClientUIDoc.h"

// CClientConfig 对话框

class CClientConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CClientConfig)

public:
	CClientConfig(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClientConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_ClientID;
	int m_ControlPort;
	int m_HeartbeatPort;
	CIPAddressCtrl m_IPAddress;
	int m_ResultPort;
	int m_TaskPort;
	CClientUIDoc* pDoc;

	void ReadConfigFile();
	BOOL WriteConfig(char* configName);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
