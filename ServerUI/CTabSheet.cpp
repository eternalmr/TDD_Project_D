//#include "..\stdafx.h"
#include "pch.h"
//#include "stdafx.h"
#include "CTabSheet.h"
 
CTabSheet::CTabSheet()
{
    m_nPageNum = 0;
    m_nCurrentPage = 0;
    m_bLock = false;
}
 
CTabSheet::~CTabSheet()
{
}
 
 
BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：增加页面
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
BOOL CTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT ID, bool bEnable)
{
    if (MAXPAGE == m_nPageNum)
        return FALSE;
 
    m_nPageNum ++;
 
    m_tsPage[m_nPageNum-1].p = pDialog;
    m_tsPage[m_nPageNum-1].nIDD = ID;
    m_tsPage[m_nPageNum-1].Title = title;
    m_tsPage[m_nPageNum-1].bEnable = bEnable;
 
    return TRUE;
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：设置页面
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
void CTabSheet::SetRect()
{
    CRect               tabRect, itemRect;
    int                 nX, nY, nXc, nYc;
    int                 i;
 
    GetClientRect(&tabRect);
    GetItemRect(0, &itemRect);
 
    nX = itemRect.left;
    nY = itemRect.bottom + 1;
    nXc = tabRect.right - itemRect.left - 3;
    nYc = tabRect.bottom - nY - 2;
 
    for (i = 0; i < m_nPageNum; i ++)
    {   
        if (i == m_nCurrentPage)
        {
            m_tsPage[i].p->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
        }
        else
        {
            m_tsPage[i].p->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
        }
    }
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：创建分页对话框
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
void CTabSheet::CreatePage()
{
    int                 i;
 
    for (i = 0; i < m_nPageNum; i ++)
    {
        m_tsPage[i].p->Create(m_tsPage[i].nIDD, this);
        //InsertItem(i, m_tsPage[i].Title);
        //CTabCtrl::InsertItem(TCIF_TEXT, i, m_Title[i], 0, 0);
    }
}
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：显示
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
BOOL CTabSheet::Show(int nItem)
{
    int                 i;
 
    if (nItem < 0 || nItem >= m_nPageNum)
        return FALSE;
 
    if (!m_tsPage[nItem].bEnable)
        return FALSE;
 
    for (i = 0; i < m_nPageNum; i ++)
    {
        //m_tsPage[i].p->Create(m_tsPage[i].nIDD, this);
        InsertItem(i, m_tsPage[i].Title);
        //CTabCtrl::InsertItem(TCIF_TEXT, i, m_Title[i], 0, 0);
    }
 
    for (i = 0; i < m_nPageNum; i ++)
    {   
        if (i == m_nCurrentPage)
        {
            m_tsPage[i].p->ShowWindow(SW_SHOW);
        }
        else
        {
            m_tsPage[i].p->ShowWindow(SW_HIDE);
        }
    }
 
    SetRect();
    SetCurSel(nItem);
 
    return TRUE;
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：隐藏
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
void CTabSheet::Hide()
{
    int                 i;
    
    for (i = 0; i < m_nPageNum; i ++)
    {
        m_tsPage[i].p->ShowWindow(SW_HIDE);
    }
    
    DeleteAllItems();
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：点击鼠标左键
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
void CTabSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
    int                 nItem;
    int                 i;
    CRect               rc;
 
    if (m_bLock)
        return;
 
    for (i = 0; i < GetItemCount(); i++)
    {
        GetItemRect(i, rc);
        if (rc.PtInRect(point))
        {
            nItem = i;
        }
    }
 
    if (!m_tsPage[nItem].bEnable)
    {
        return;
    }
    
    CTabCtrl::OnLButtonDown(nFlags, point);
 
    
    if (m_nCurrentPage != nItem)
    {
        m_tsPage[m_nCurrentPage].p->ShowWindow(SW_HIDE);
        m_nCurrentPage = nItem;     
        m_tsPage[m_nCurrentPage].p->ShowWindow(SW_SHOW);
        //m_pPages[m_nCurrentPage]->SetFocus();
    }
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：设置当前页面选择
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
int CTabSheet::SetCurSel(int nItem)
{
    if (nItem < 0 || nItem >= m_nPageNum)
        return -1;
 
    if (!m_tsPage[nItem].bEnable)
    {
        return -1;
    }
 
    CTabCtrl::SetCurSel(nItem);
 
    if (!m_bLock)
    {
        if (m_nCurrentPage != nItem )
        {
            m_tsPage[m_nCurrentPage].p->ShowWindow(SW_HIDE);
            m_nCurrentPage = nItem;
            m_tsPage[m_nCurrentPage].p->ShowWindow(SW_SHOW);
            
            //m_pPages[m_nCurrentPage]->SetFocus();
        }
    }
 
    return nItem;
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：锁定
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
void CTabSheet::Lock()
{
    m_bLock = true;
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：解锁
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
void CTabSheet::UnLock()
{
    m_bLock = false;
}
 
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
//描述：设置页面属性
//参数：
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□//
BOOL CTabSheet::SetPage(LPCTSTR title, bool bEnable)
{
    BOOL                bRet;
    int                 i;
 
    bRet = FALSE;
 
    for (i = 0; i < m_nPageNum; i ++)
    {
        if (m_tsPage[i].Title == title)
        {
            m_tsPage[i].bEnable = bEnable;
            bRet = TRUE;
        }
    }
    
    return bRet;
}
 
BOOL CTabSheet::SetPage(int nItem, bool bEnable)
{
    if (nItem < 0 || nItem >= m_nPageNum)
        return FALSE;
    
    m_tsPage[nItem].bEnable = bEnable;
 
    return TRUE;
}