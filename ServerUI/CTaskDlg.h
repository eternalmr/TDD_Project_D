﻿#pragma once

#include "CTabSheet.h"
#include "CTaskOverviewPage.h"
#include "CTaskDetailPage.h"

// CTaskDlg 窗体视图

class CTaskDlg : public CFormView
{
	DECLARE_DYNCREATE(CTaskDlg)

protected:
	CTaskDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CTaskDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK };
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
	CTaskOverviewPage m_overviewPage;


public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CTaskDetailPage m_detailPage;
};


