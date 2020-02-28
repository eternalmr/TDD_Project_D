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
//	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayView::OnBnClickedButton1)
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


//void CDisplayView::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	//下边两个函数获取的是显示屏幕的大小，但不包括任务栏等区域
//	//int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
//	//int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
//	//下边这两个函数获取的是真正屏幕的大小：屏幕分辨率
//	//int nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
//	//int nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度
//	//CString strScreen;
//	//strScreen.Format(L"%d,%d", nWidth, nHeight);
//	//MessageBox(strScreen);
//
//	//对话框窗体大小及其屏幕坐标
//	//CRect rectDlg;
//	////法1：
//	////GetClientRect(rectDlg);//获得窗体的大小
//	////法2：
//	//GetWindowRect(rectDlg);//获得窗体在屏幕上的位置
//	//ScreenToClient(rectDlg);
//	//CString strDlg;
//	//strDlg.Format(L"%d,%d,%d,%d", rectDlg.left, rectDlg.top, rectDlg.Width(), rectDlg.Height());
//	//MessageBox(strDlg);
//
//	CRect rect;
//	GetClientRect(&rect);
//	int w = rect.Width();
//	int h = rect.Height();
//	CString str;
//	str.Format(TEXT("%d, %d"), w, h);
//	MessageBox(str);
//}
