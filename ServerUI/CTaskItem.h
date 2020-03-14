#pragma once


// CTaskItem 对话框

class CTaskItem : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskItem)

public:
	CTaskItem(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTaskItem();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK_ITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_TaskName;
	CStatic m_TaskComputeStatus;
	CStatic m_TaskStoreStatus;
	CProgressCtrl m_ProgessBar;
	int m_id;
	BOOL m_isShow;

public:
	virtual BOOL OnInitDialog();
};
