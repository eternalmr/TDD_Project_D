
// MainFrm.h: CMainFrame 类的接口
//
#pragma once
//#include "CServer.h"
#include "CUserMessage.h"


class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	CSplitterWnd m_WholeWindowSplitter;
	CSplitterWnd m_RightWindowSplitter;
	CWnd* m_pSelectView;
	CWnd* m_pLogView;
	CWnd* m_pDisplayView;
	
private:
	BOOL m_bWholeWndIsSplitted;
	BOOL m_bRightWndIsSplitted;
	std::thread sim_thread;

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg LRESULT ShiftPage(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


