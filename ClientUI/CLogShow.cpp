// CTLogEdit.cpp: 实现文件
//

#include "pch.h"
#include "CLogShow.h"

COLORREF TLP_DEBUG_COLOR = RGB(0, 0, 0);   //black
COLORREF TLP_DETAIL_COLOR = RGB(0, 255, 0);  //green
COLORREF TLP_NORMAL_COLOR = RGB(0, 0, 255);  //blue
COLORREF TLP_ERROR_COLOR = RGB(255, 0, 0);   //red

// CTLogEdit

IMPLEMENT_DYNCREATE(CLogShow, CRichEditCtrl)

CLogShow::CLogShow() :
	m_bRun(TRUE),
	m_nLineLimit(30),
	m_bAutoScroll(TRUE)
{
	InitializeCriticalSection(&m_csTLogEdit);
}

CLogShow::~CLogShow()
{

}

CLogShow& CLogShow::GetInstance()
{
	static CLogShow logger;
	return logger;
}

BEGIN_MESSAGE_MAP(CLogShow, CRichEditCtrl)
END_MESSAGE_MAP()


// CTLogEdit 诊断

#ifdef _DEBUG
void CLogShow::AssertValid() const
{
	CRichEditCtrl::AssertValid();
}

#ifndef _WIN32_WCE
void CLogShow::Dump(CDumpContext& dc) const
{
	CRichEditCtrl::Dump(dc);
}

void CLogShow::SetLogLineColor(long lPos, LOG_LEVEL tlp)
{
	SetSel(lPos, lPos);
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(CHARFORMAT2));
	cf.dwMask |= CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	switch (tlp)
	{
	case TLP_DEBUG:
	{
		cf.crTextColor = TLP_DEBUG_COLOR;
		break;
	}
	case TLP_DETAIL:
	{
		cf.crTextColor = TLP_DETAIL_COLOR;
		break;
	}
	case TLP_NORMAL:
	{
		cf.crTextColor = TLP_NORMAL_COLOR;
		break;
	}
	case TLP_ERROR:
	{
		cf.crTextColor = TLP_ERROR_COLOR;
		break;
	}
	}
	SetSelectionCharFormat(cf);
}

void CLogShow::AddLine(LPCTSTR lpTLogLine, LOG_LEVEL tlp)
{
	PTLOGITEM pItem = new TLOGITEM;
	pItem->line = lpTLogLine;
	pItem->tlp = tlp;
	EnterCriticalSection(&m_csTLogEdit);
	m_TLogItemQue.AddTail(pItem);
	LeaveCriticalSection(&m_csTLogEdit);
}

void CLogShow::LimitLine()
{
	if (GetLineCount() - 1 > m_nLineLimit)
	{
		SetSel(0, LineLength(0) + 1);
		Clear();
	}
}


int CLogShow::ShowLogOnBoard()
{
	PTLOGITEM pItem;
	long pos;
	while (m_bRun) {
		Sleep(1000);
		while (!m_TLogItemQue.IsEmpty())
		{
			EnterCriticalSection(&m_csTLogEdit);
			pItem = (PTLOGITEM)m_TLogItemQue.RemoveHead();
			LeaveCriticalSection(&m_csTLogEdit);
			pos = GetTextLength();
			SetLogLineColor(pos, pItem->tlp);
			ReplaceSel(pItem->line);
			LimitLine();
			delete pItem;
			if (m_bAutoScroll)
			{
				SendMessage(WM_VSCROLL, SB_BOTTOM);
			}
		}
	}
	AfxMessageBox(TEXT("log线程已退出"));
	return 0;
}


#endif
#endif //_DEBUG


// CTLogEdit 消息处理程序
