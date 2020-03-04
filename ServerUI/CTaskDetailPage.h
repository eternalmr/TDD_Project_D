#pragma once
#include "CTaskItem.h"


#define TASK_NUM 5

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
	CTaskItem m_TaskItem[TASK_NUM];

};
