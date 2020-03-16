#pragma once


// CClientItem 对话框

class CClientItem : public CDialogEx
{
	DECLARE_DYNAMIC(CClientItem)

public:
	CClientItem(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClientItem();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_ITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ClientName;
	CStatic m_ClientStatus;
	CStatic m_ClientCPU;
	CStatic m_ClientMemory;
	CButton m_StartBtn;
	CButton m_PauseBtn;
	CButton m_StopBtn;
	UINT	m_id;

	afx_msg void OnBnClickedBtnStartClient();
	afx_msg void OnBnClickedBtnPauseClient();
	afx_msg void OnBnClickedBtnStopClient();
	afx_msg void OnBnClickedBtnContinueClient();
};
