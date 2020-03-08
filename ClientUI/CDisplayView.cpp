// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CDisplayView.h"
#include "CUserDefineMsg.h"
#include "MainFrm.h"
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
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
}

BEGIN_MESSAGE_MAP(CDisplayView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDisplayView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDisplayView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDisplayView::OnBnClickedButton4)
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

void CDisplayView::OnBnClickedButton1()
{
	CLogShow::GetInstance().AddLine(TEXT("Test Debug Log\r\n"), TLP_DEBUG);
}

void CDisplayView::OnBnClickedButton2()
{
	CLogShow::GetInstance().AddLine(TEXT("Test Debug Log\r\n"), TLP_DETAIL);
}

void CDisplayView::OnBnClickedButton3()
{
	CLogShow::GetInstance().AddLine(TEXT("Test Normal Log\r\n"), TLP_NORMAL);
}

void CDisplayView::OnBnClickedButton4()
{
	CLogShow::GetInstance().AddLine(TEXT("Test Error Log\r\n"), TLP_ERROR);
}
