// CLogView.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CLogView.h"


// CLogView

IMPLEMENT_DYNCREATE(CLogView, CFormView)

CLogView::CLogView()
	: CFormView(IDD_LOGVIEW)
{

}

CLogView::~CLogView()
{
}

void CLogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogView, CFormView)
//	ON_BN_CLICKED(IDC_BUTTON1, &CLogView::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, &CLogView::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLogView 诊断

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLogView 消息处理程序


//void CLogView::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CRect rect;
//	GetClientRect(&rect);
//	int w = rect.Width();
//	int h = rect.Height();
//	CString str;
//	str.Format(TEXT("%d, %d"), w, h);
//	MessageBox(str);
//}


//void CLogView::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CRect rect;
//	GetWindowRect(&rect);
//	int w = rect.Width();
//	int h = rect.Height();
//	CString str;
//	str.Format(TEXT("%d, %d"), w, h);
//	MessageBox(str);
//}
