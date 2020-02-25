#pragma once


// CClientOverviewPage 对话框

class CClientOverviewPage : public CDialogEx
{
	DECLARE_DYNAMIC(CClientOverviewPage)

public:
	CClientOverviewPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClientOverviewPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_OVERVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
