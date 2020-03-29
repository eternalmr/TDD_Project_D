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
	,m_LoadedTaskNum(0)
{

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

	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CTaskDetailPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在此添加额外的初始化
	ShowLoadedTaskItems();

	SetTimer(UpdateTaskInfoTimer, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTaskDetailPage::ShowLoadedTaskItems()
{
	CString str;
	CRect rect;

	GetClientRect(&rect);
	int itemHeight = rect.Height()/4;
	int itemWidth = rect.Width()-20;

	str.Format(TEXT("w: %d, h: %d\r\n"), itemWidth, itemHeight);
	OutputDebugString(str);

	m_LoadedTaskNum = server.all_tasks.size();
	SetScrollRange(SB_VERT, 0, itemHeight * m_LoadedTaskNum, TRUE); 

	for (int i = 0; i < m_LoadedTaskNum; i++)
	{
		str.Format(TEXT("第%d个任务"), i + 1);
		m_TaskItems[i].Create(IDD_TASK_ITEM, this);
		m_TaskItems[i].MoveWindow(0, itemHeight * i + 1 * i, itemWidth, itemHeight);
		m_TaskItems[i].m_id = i+1;
		m_TaskItems[i].m_TaskName.SetWindowTextW(str);
		m_TaskItems[i].m_ProgessBar.SetRange(0, 100);
		m_TaskItems[i].ShowWindow(SW_SHOWNORMAL);
	}
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


void CTaskDetailPage::UpdateTaskItemInfo()
{
	uint progress = 0;
	CString str;
	for (int i = 0; i < m_LoadedTaskNum ; i++)
	{
		progress = server.all_tasks[i]->get_simulation_progress();
		m_TaskItems[i].m_ProgessBar.SetPos(progress);
		str.Format(TEXT("%d"), progress);
		str = CString("计算状态：") + str + CString("%");
		m_TaskItems[i].m_TaskComputeStatus.SetWindowTextW(str);
	}
}


void CTaskDetailPage::OnTimer(UINT_PTR nIDEvent)
{
	// 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case UpdateTaskInfoTimer: 
	{
		UpdateTaskItemInfo();
		break;
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTaskDetailPage::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(UpdateTaskInfoTimer);
	OutputDebugString(TEXT("任务信息更新计时器已停止。\r\n"));
}


void CTaskDetailPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 当载入任务数量过多是，变化界面会很卡
	if (::IsWindow(m_TaskItems[0].GetSafeHwnd()))
	{
		int itemHeight = cy / 4;
		int itemWidth = cx;
		for (int i = 0; i < m_LoadedTaskNum ; i++) {
			m_TaskItems[i].MoveWindow(0, itemHeight * i + 1 * i, itemWidth, itemHeight);
		}
	}
}
