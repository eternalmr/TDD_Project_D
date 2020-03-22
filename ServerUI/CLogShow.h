#pragma once

#define AddLog(str,level) (CLogShow::GetInstance().AddLine(str,level))

extern COLORREF TLP_DEBUG_COLOR;
extern COLORREF TLP_DETAIL_COLOR;
extern COLORREF TLP_NORMAL_COLOR;
extern COLORREF TLP_ERROR_COLOR;

enum  LOG_LEVEL
{
	TLP_DEBUG = 0,
	TLP_DETAIL,
	TLP_NORMAL,
	TLP_ERROR
};

typedef struct _TLOGITEM
{
	CString line;
	LOG_LEVEL tlp;
} TLOGITEM, *PTLOGITEM;

// log显示单例类
class CLogShow : public CRichEditCtrl
{
	DECLARE_DYNCREATE(CLogShow)

private:
	CLogShow();           // 将Log类声明成一个单例类
	virtual ~CLogShow();

public:
	static CLogShow& GetInstance(); 

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
	void SetLogLineColor(long lPos, LOG_LEVEL level);
	void AddLine(LPCTSTR lpTLogLine, LOG_LEVEL level = TLP_NORMAL);
	void LimitLine();
	int TLogEditThreadSTL();

	BOOL m_bRun;
	std::thread m_LogThread;
private:
	CRITICAL_SECTION m_csTLogEdit;
	CPtrList m_TLogItemQue;
	int m_nLineLimit;
	BOOL m_bAutoScroll;
};


