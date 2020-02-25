#pragma once
 
#define MAXPAGE 16
 
typedef struct TAB_SHEET_PAGE
{
    LPCTSTR             Title;
    UINT                nIDD;
    CDialog             *p;             
    bool                bEnable;
} TABSHEETPAGE;
 
class CTabSheet : public CTabCtrl
{
public:
    CTabSheet();
    virtual ~CTabSheet();
 
    DECLARE_MESSAGE_MAP()
 
protected:
    afx_msg void        OnLButtonDown(UINT nFlags, CPoint point);
 
public:
    int                 SetCurSel(int nItem);
    BOOL                Show(int nItem);
    void                SetRect();
    void                Lock();
    void                UnLock();
    BOOL                AddPage(LPCTSTR title, CDialog *pDialog, UINT ID, bool bEnable);
    void                CreatePage();
 
    void                Hide();
    BOOL                SetPage(LPCTSTR title, bool bEnable);
    BOOL                SetPage(int nItem, bool bEnable);
 
protected:
    BOOL                m_bLock;
 
    TAB_SHEET_PAGE      m_tsPage[MAXPAGE];
    
    int                 m_nPageNum;
    int                 m_nCurrentPage;
};
 