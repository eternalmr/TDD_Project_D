#pragma once
//#include "CTLog.h"

extern COLORREF TLP_DEBUG_COLOR;
extern COLORREF TLP_DETAIL_COLOR;
extern COLORREF TLP_NORMAL_COLOR;
extern COLORREF TLP_ERROR_COLOR;

// CTLogEdit 视图

enum  TLOGPRIORITY
{
	TLP_DEBUG = 0,
	TLP_DETAIL,
	TLP_NORMAL,
	TLP_ERROR
};

typedef struct _TLOGITEM
{
	CString line;
	TLOGPRIORITY tlp;
} TLOGITEM, *PTLOGITEM;

class CTLogEdit : public CRichEditCtrl
{
	DECLARE_DYNCREATE(CTLogEdit)

protected:

private:
	CTLogEdit();           // 动态创建所使用的受保护的构造函数
	virtual ~CTLogEdit();

public:
	static CTLogEdit& GetInstance();


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetLogLineColor(long lPos, TLOGPRIORITY tlp);
	void AddLine(LPCTSTR lpTLogLine, TLOGPRIORITY tlp = TLP_NORMAL);
	void LimitLine();
	int TLogEditThreadSTL();

	BOOL m_bRun;

private:
	CRITICAL_SECTION m_csTLogEdit;
	CPtrList m_TLogItemQue;
	int m_nLineLimit;
	BOOL m_bAutoScroll;
};


