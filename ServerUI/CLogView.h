#pragma once

#include <thread>
#include "CTLogEdit.h"

#define NW_DEBUG_LOG  (WM_USER + 200)
#define NW_DETAIL_LOG (WM_USER + 201)
#define NW_NORMAL_LOG (WM_USER + 202)
#define NW_ERROR_LOG  (WM_USER + 203)
// CLogView 窗体视图

class CLogView : public CFormView
{
	DECLARE_DYNCREATE(CLogView)

protected:
	CLogView();           // 动态创建所使用的受保护的构造函数
	virtual ~CLogView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGVIEW };
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
	CTLogEdit m_log;
	std::thread LogThread;
	virtual void OnInitialUpdate();
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
protected:
	afx_msg LRESULT OnNwWritelog(WPARAM wParam, LPARAM lParam);
};


