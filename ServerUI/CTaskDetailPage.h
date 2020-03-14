#pragma once
#include "CTaskItem.h"


#define MAX_TASK_NUM 20

// CTaskDetailPage 对话框

class CTaskDetailPage : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskDetailPage)

public:
	CTaskDetailPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTaskDetailPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK_DETAIL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTaskItem m_TaskItems[MAX_TASK_NUM];
	CPtrList m_TaskList;

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void AddNewTaskItem(Task *ptask);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};
