// CClientDetailPage.cpp: 实现文件
//

#include "pch.h"
#include "ServerUI.h"
#include "CClientDetailPage.h"
#include "afxdialogex.h"


// CClientDetailPage 对话框

IMPLEMENT_DYNAMIC(CClientDetailPage, CDialogEx)

CClientDetailPage::CClientDetailPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DETAIL, pParent)
{

}

CClientDetailPage::~CClientDetailPage()
{
}

void CClientDetailPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientDetailPage, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CClientDetailPage 消息处理程序


BOOL CClientDetailPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RECT rect;
	GetClientRect(&rect);
	SetScrollRange(SB_VERT, 0, 1000, TRUE);

	CString str;
	for (int i = 0; i < CLIENT_NUM; i++)
	{
		str.Format(TEXT("节点%d："), i);
		m_ClientItems[i].Create(IDD_CLIENT_ITEM, this);
		m_ClientItems[i].MoveWindow(0, 120 * i + 1 * i, 500, 120);
		m_ClientItems[i].m_id = i;
		m_ClientItems[i].m_ClientName.SetWindowTextW(str);
		m_ClientItems[i].ShowWindow(SW_SHOWNORMAL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CClientDetailPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


BOOL CClientDetailPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
