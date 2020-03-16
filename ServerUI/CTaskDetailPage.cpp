// CTaskDetailPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "ServerUIDoc.h"
#include "CTaskDetailPage.h"
#include "afxdialogex.h"
#include "CLogView.h"
#include "MainFrm.h"
#include "CTask.h"

// CTaskDetailPage 对话框

IMPLEMENT_DYNAMIC(CTaskDetailPage, CDialogEx)

CTaskDetailPage::CTaskDetailPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK_DETAIL, pParent)
{
	m_LoadedTaskNum = CServer::get_instance().tasks.size();
}

CTaskDetailPage::~CTaskDetailPage()
{

}

void CTaskDetailPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTaskDetailPage, CDialogEx)

	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()

END_MESSAGE_MAP()


BOOL CTaskDetailPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在此添加额外的初始化
	int itemHeight = 80;
	CString str;
	CRect rect;
	GetClientRect(&rect);

	for (int i = 0; i < MAX_TASK_NUM; i++)
	{
		str.Format(TEXT("第%d个任务"), i+1);
		m_TaskItems[i].Create(IDD_TASK_ITEM, this);
		m_TaskItems[i].MoveWindow(0, itemHeight * i + 1 * i, 600, itemHeight);//TODO：确定合适的大小
		m_TaskItems[i].m_id = i;
		m_TaskItems[i].m_TaskName.SetWindowTextW(str);
		m_TaskItems[i].m_ProgessBar.SetRange(0, 100);
		if (i < m_LoadedTaskNum) {//显示已加载的任务，其他隐藏
			m_TaskItems[i].ShowWindow(SW_SHOWNORMAL);
		}
	}

	SetScrollRange(SB_VERT, 0, itemHeight * 10, TRUE); //TODO:设置一个合理的上限
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTaskDetailPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollInfo;
	GetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
	int ScrollStep = 10;
	switch (nSBCode)
	{
	case SB_LINEUP:
		scrollInfo.nPos -= 1;
		if (scrollInfo.nPos < scrollInfo.nMin)
		{
			scrollInfo.nPos = scrollInfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		ScrollWindow(0, 1);
		break;
	case SB_LINEDOWN:
		scrollInfo.nPos += 1;
		if (scrollInfo.nPos > scrollInfo.nMax)
		{
			scrollInfo.nPos = scrollInfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		ScrollWindow(0, -1);
		break;
	case SB_TOP:
		ScrollWindow(0, (scrollInfo.nPos - scrollInfo.nMin) * 1);
		scrollInfo.nPos = scrollInfo.nMin;
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		break;
	case SB_BOTTOM:
		ScrollWindow(0, -(scrollInfo.nMax - scrollInfo.nPos) * 1);
		scrollInfo.nPos = scrollInfo.nMax;
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		break;
	case SB_PAGEUP:
		scrollInfo.nPos -= ScrollStep;
		if (scrollInfo.nPos < scrollInfo.nMin)
		{
			scrollInfo.nPos = scrollInfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		ScrollWindow(0, 1 * ScrollStep);
		break;
	case SB_PAGEDOWN:
		scrollInfo.nPos += ScrollStep;
		if (scrollInfo.nPos > scrollInfo.nMax)
		{
			scrollInfo.nPos = scrollInfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		ScrollWindow(0, -1 * ScrollStep);
		break;
	case SB_ENDSCROLL:
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_THUMBTRACK:
		CString str;
		str.Format(TEXT("nPos1:%d nPos1:%d/n"), scrollInfo.nPos, nPos);
		OutputDebugString(str);
		ScrollWindow(0, (scrollInfo.nPos - nPos));
		scrollInfo.nPos = nPos;
		SetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
		break;
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CTaskDetailPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta == 120)
	{
		OnVScroll(SB_PAGEUP, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
	}
	else if (zDelta == -120)
	{
		OnVScroll(SB_PAGEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CTaskDetailPage::UpdateTaskProgress()
{
	CServer &server = CServer::get_instance();
	uint progress = 0;

	for (int i = 0; i < m_LoadedTaskNum ; i++)
	{
		progress = server.tasks[i].get_simulation_progress();
		m_TaskItems[i].m_ProgessBar.SetPos(progress);
	}
}

void CTaskDetailPage::UpdateShow()
{
	m_LoadedTaskNum = CServer::get_instance().tasks.size();
	for(int i=0; i<m_LoadedTaskNum; i++)
	{
		m_TaskItems[i].ShowWindow(SW_SHOWNORMAL);
	}
}

void CTaskDetailPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1: {
		UpdateTaskProgress();
		break;
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
