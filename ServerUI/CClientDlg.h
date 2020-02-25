#pragma once
#include "CTabSheet.h"
#include "CClientOverviewPage.h"
#include "CClientDetailPage.h"


// CClientDlg 窗体视图

class CClientDlg : public CFormView
{
	DECLARE_DYNCREATE(CClientDlg)

protected:
	CClientDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CClientDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT };
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
private:
	CTabSheet m_tab;
	CClientOverviewPage overviewPage;
	CClientDetailPage detailPage;
public:
	virtual void OnInitialUpdate();
};


