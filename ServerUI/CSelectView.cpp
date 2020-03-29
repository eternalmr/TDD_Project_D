// CSelectView.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CSelectView.h"
#include "MainFrm.h"

// CSelectView

IMPLEMENT_DYNCREATE(CSelectView, CTreeView)

CSelectView::CSelectView()
{

}

CSelectView::~CSelectView()
{
}

BEGIN_MESSAGE_MAP(CSelectView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSelectView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CSelectView 诊断

#ifdef _DEBUG
void CSelectView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CSelectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSelectView 消息处理程序


void CSelectView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//1. 准备图标
	HICON icons[2];
	icons[0] = AfxGetApp()->LoadIconW(IDI_CLIENT);//若有多个图标，则设置一个数组
	icons[1] = AfxGetApp()->LoadIconW(IDI_TASK);//若有多个图标，则设置一个数组

	m_imageList.Create(30, 30, ILC_COLOR32, 2, 2);
	m_imageList.Add(icons[0]);
	m_imageList.Add(icons[1]);
	m_treeCtrl = &GetTreeCtrl();
	m_treeCtrl->SetImageList(&m_imageList, TVSIL_NORMAL);

	//2. 插入节点
	m_treeCtrl->InsertItem(TEXT("推演任务"), 1, 1, NULL);
	m_treeCtrl->InsertItem(TEXT("推演节点"), 0, 0, NULL);
	
}

void CSelectView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM item = m_treeCtrl->GetSelectedItem();
	CString str = m_treeCtrl->GetItemText(item);

	if (str == TEXT("推演节点"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_CLIENT, (WPARAM)NM_CLIENT, (LPARAM)0);
	}
	else if (str == TEXT("推演任务"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_TASK, (WPARAM)NM_TASK, (LPARAM)0);
	}
}
