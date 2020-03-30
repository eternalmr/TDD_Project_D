// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CDisplayView.h"


// CDisplayView

IMPLEMENT_DYNCREATE(CDisplayView, CFormView)

CDisplayView::CDisplayView()
	: CFormView(IDD_DISPLAYVIEW)
{

}

CDisplayView::~CDisplayView()
{
}

void CDisplayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplayView, CFormView)

	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDisplayView 诊断

#ifdef _DEBUG
void CDisplayView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisplayView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CDisplayView 消息处理程序



void CDisplayView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// 在此处添加消息处理程序代码
	//ShowScrollBar(SB_BOTH, FALSE);//垂直水平方向的滚动条都不显示。
}


void CDisplayView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	ShowScrollBar(SB_BOTH, FALSE);//垂直水平方向的滚动条都不显示。
}
