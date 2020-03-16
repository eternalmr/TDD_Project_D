// CClientItem.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientItem.h"
#include "afxdialogex.h"


// CClientItem 对话框

IMPLEMENT_DYNAMIC(CClientItem, CDialogEx)

CClientItem::CClientItem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_ITEM, pParent)
{

}

CClientItem::~CClientItem()
{
}

void CClientItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENT_NAME, m_ClientName);
	DDX_Control(pDX, IDC_CLIENT_STATUS, m_ClientStatus);
	DDX_Control(pDX, IDC_CLIENT_CPU, m_ClientCPU);
	DDX_Control(pDX, IDC_CLIENT_MEMORY, m_ClientMemory);
	DDX_Control(pDX, IDC_BTN_START_CLIENT, m_StartBtn);
	DDX_Control(pDX, IDC_BTN_PAUSE_CLIENT, m_PauseBtn);
	DDX_Control(pDX, IDC_BTN_STOP_CLIENT, m_StopBtn);
}


BEGIN_MESSAGE_MAP(CClientItem, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_CLIENT, &CClientItem::OnBnClickedBtnStartClient)
	ON_BN_CLICKED(IDC_BTN_PAUSE_CLIENT, &CClientItem::OnBnClickedBtnPauseClient)
	ON_BN_CLICKED(IDC_BTN_STOP_CLIENT, &CClientItem::OnBnClickedBtnStopClient)
	ON_BN_CLICKED(IDC_BTN_CONTINUE_CLIENT, &CClientItem::OnBnClickedBtnContinueClient)
END_MESSAGE_MAP()


// CClientItem 消息处理程序


void CClientItem::OnBnClickedBtnStartClient()
{
	CServer::get_instance().send_command_to_client(m_id, "start");
}


void CClientItem::OnBnClickedBtnPauseClient()
{
	CServer::get_instance().send_command_to_client(m_id, "pause");
}


void CClientItem::OnBnClickedBtnStopClient()
{
	CServer::get_instance().send_command_to_client(m_id, "stop");
}


void CClientItem::OnBnClickedBtnContinueClient()
{
	CServer::get_instance().send_command_to_client(m_id, "continue");
}
