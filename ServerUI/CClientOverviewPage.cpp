// CClientOverviewPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientOverviewPage.h"
#include "afxdialogex.h"



// CClientOverviewPage 对话框

IMPLEMENT_DYNAMIC(CClientOverviewPage, CDialogEx)

CClientOverviewPage::CClientOverviewPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_OVERVIEW, pParent)
{

}

CClientOverviewPage::~CClientOverviewPage()
{
}

void CClientOverviewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BREAKDOWN_CLIENT_NUM_BTN, m_BreakdownClientNum);
	DDX_Control(pDX, IDC_FREE_CLIENT_NUM_BTN, m_FreeClientNum);
	DDX_Control(pDX, IDC_INCOMPUTING_CLIENT_NUM_BTN, m_IncomputingClientNum);
	DDX_Control(pDX, IDC_TOTAL_CLIENT_NUM_BTN, m_TotalClientNum);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CClientOverviewPage, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CClientOverviewPage 消息处理程序
BOOL CClientOverviewPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在此添加额外的初始化
	UpdateClientInfo();

	SetTimer(UpdateClientInfoTimer, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CClientOverviewPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case UpdateClientInfoTimer: {
		UpdateClientInfo();
		break;
	}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CClientOverviewPage::UpdateClientInfo()
{
	CString str;
	int totalNum, freeNum, inComputingNum, breakdownNum;

	//void CServer::get_client_num_info(int &nTotal, int &nIncomputing, int &nFree, int &nBreakdown)

	server.get_client_num_info(totalNum, inComputingNum, freeNum, breakdownNum);

	str.Format(TEXT("节点总数：\r\n %d"), totalNum);
	m_TotalClientNum.SetWindowTextW(str);

	str.Format(TEXT("计算中：\r\n %d"), inComputingNum);
	m_IncomputingClientNum.SetWindowTextW(str);

	str.Format(TEXT("空闲中：\r\n %d"), freeNum);
	m_FreeClientNum.SetWindowTextW(str);

	str.Format(TEXT("故障：\r\n %d"), breakdownNum);
	m_BreakdownClientNum.SetWindowTextW(str);

	m_ProgressBar.SetRange(0, totalNum);
	m_ProgressBar.SetPos(freeNum + inComputingNum);
}



void CClientOverviewPage::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	KillTimer(UpdateClientInfoTimer);
	OutputDebugString(TEXT("Client数量计时器已停止。\r\n"));

}
