#pragma once

// CTaskOverviewPage 对话框

class CTaskOverviewPage : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskOverviewPage)

public:
	CTaskOverviewPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTaskOverviewPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK_OVERVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedThreadBtn();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void UpdateTaskInfo();

	CButton m_TotalTaskNum;
	CButton m_CompletedTaskNum;
	CButton m_IncomputingTaskNum;
	CButton m_UndoTaskNum;
	CProgressCtrl m_ProgressBar;
};
