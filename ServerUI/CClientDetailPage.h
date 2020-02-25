#pragma once


// CClientDetailPage 对话框

class CClientDetailPage : public CDialogEx
{
	DECLARE_DYNAMIC(CClientDetailPage)

public:
	CClientDetailPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClientDetailPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DETAIL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
