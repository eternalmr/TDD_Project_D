
// MainFrm.h: CMainFrame 类的接口
//
#pragma once
#include "..\Server\CServer.h"

#define NM_TASK     (WM_USER + 100)
#define NM_CLIENT   (WM_USER + 101)
#define NM_THREAD   (WM_USER + 102)
#define NM_START    (WM_USER + 103)
#define NM_PAUSE    (WM_USER + 104)
#define NM_STOP     (WM_USER + 105)
#define NM_CONTINUE (WM_USER + 106)

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


private:
	CSplitterWnd m_WholeWindowSplitter;
	CSplitterWnd m_RightWindowSplitter;
	//CServer server;

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg LRESULT ShiftPage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ControlServer(WPARAM wParam, LPARAM lParam);
};


