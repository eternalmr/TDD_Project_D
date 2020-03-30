#pragma once

// CDisplayView 窗体视图

class CDisplayView : public CFormView
{
	DECLARE_DYNCREATE(CDisplayView)

protected:
	CDisplayView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDisplayView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPLAYVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;

#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedReceiveTaskBtn();

	void UpdateClientInfo();

	CProgressCtrl m_progressBar;
	CStatic m_memoryStatus;
	CStatic m_cpuStatus;
	CStatic m_clientName;
	CStatic m_currentTask;
	CButton m_ReceiveTaskBtn;

private:
	enum TimerSet { ClientInfoUpdateTimer };
};


