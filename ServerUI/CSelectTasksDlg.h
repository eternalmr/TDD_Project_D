#pragma once


// CSelectTasksDlg 对话框

class CSelectTasksDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectTasksDlg)

public:
	CSelectTasksDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelectTasksDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_TASKS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
