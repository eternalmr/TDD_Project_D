﻿// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "ClientUI.h"
#include "CDisplayView.h"
#include "CUserDefineMsg.h"
#include "MainFrm.h"

enum TimerSet
{
	CPUTimer = 1
};

IMPLEMENT_DYNCREATE(CDisplayView, CFormView)

CDisplayView::CDisplayView()
	: CFormView(IDD_DISPLAYVIEW)
	, m_clientName(_T("推演节点："))
	, m_cpuStatus(_T("CPU状态："))
	, m_memoryStatus(_T("内存状态："))
{
	
}

CDisplayView::~CDisplayView()
{

}

void CDisplayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
	DDX_Text(pDX, IDC_MEMORY_STATUS, m_memoryStatus);
	DDX_Text(pDX, IDC_CPU_STATUS, m_cpuStatus);
	DDX_Text(pDX, IDC_CLIENT_ID, m_clientName);
	DDX_Control(pDX, IDC_CURRENT_TASK, m_currentTask);
}

BEGIN_MESSAGE_MAP(CDisplayView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDisplayView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDisplayView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDisplayView::OnBnClickedButton4)
	ON_WM_TIMER()
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



void CDisplayView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1: {
		CString str;
		double CPUStatus = CClient::get_instance().get_cpu_status();
		str.Format(TEXT("CPU状态：%.2lf"), CPUStatus);
		m_cpuStatus = str;
		UpdateData(FALSE);
		break;
	}
	case 2:
		//if (tmp1 >= 200)
		//{
		//	this->KillTimer(2);
		//	MessageBox(_T("定时器2停止！"), NULL, NULL);
		//	return;
		//}
		//Sleep(20);
		//tmp1 += 1;
		//m_value2.Format(_T("%d"), tmp1);
		UpdateData(FALSE);
		break;
	default:
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CDisplayView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SetTimer(1, 100, NULL);
}
