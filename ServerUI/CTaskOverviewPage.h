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
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedThreadBtn();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedStop();

	HWND GetLogWndHandle();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedLoad();
};
