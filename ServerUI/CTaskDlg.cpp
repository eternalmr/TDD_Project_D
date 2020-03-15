﻿// CTaskDlg.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CTaskDlg.h"


// CTaskDlg

IMPLEMENT_DYNCREATE(CTaskDlg, CFormView)

CTaskDlg::CTaskDlg()
	: CFormView(IDD_TASK)
{

}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CTaskDlg, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTaskDlg 诊断

#ifdef _DEBUG
void CTaskDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTaskDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTaskDlg 消息处理程序


void CTaskDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect);

	m_tab.MoveWindow(0, 0, rect.Width(), rect.Height());

	

	m_tab.AddPage(TEXT("任务概况"), &overviewPage, IDD_TASK_OVERVIEW, true);
	m_tab.AddPage(TEXT("任务详情"), &detailPage, IDD_TASK_DETAIL, true);
	m_tab.CreatePage();
	//overviewPage.MoveWindow(0, 0, rect.Width(), rect.Height());
	//detailPage.MoveWindow(0, 0, rect.Width(), rect.Height());
	m_tab.Show(0);

	//详情页更新内部数据
	//detailPage.UpdateShow(10);
}


void CTaskDlg::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ShowScrollBar(SB_BOTH, FALSE);//垂直水平方向的滚动条都不显示。
}

//void CTaskDlg::myInit(int num)
//{
//	detailPage.myInit(num);
//}